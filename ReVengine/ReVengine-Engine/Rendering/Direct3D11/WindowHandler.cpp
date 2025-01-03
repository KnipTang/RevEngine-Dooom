#include "Rendering/Direct3D11/WindowHandler.h"
#include <cassert>
#include "SDL.h"
#include <SDL_syswm.h>
#include <string>
#include "filesystem"
#include "fstream"
#include "DirectXMath.h"
#include "Utils/Vertex.h"
#include "Rendering/Direct3D11/Meshes/Mesh.h"
#include "Rendering/Texture.h"
#include <d3dcompiler.h>
#include "Rendering/Shaders/TextureShader.h"


using namespace RevDev;

WindowHandler_D3D11::WindowHandler_D3D11(SDL_Window* window, int windowWidth, int windowHeight)
{
	SDL_SysWMinfo wmInfo{};
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(window, &wmInfo);
	hwnd = wmInfo.info.win.window;

	m_WindowWidth = windowWidth;
	m_WindowHeight = windowHeight;
}

WindowHandler_D3D11::~WindowHandler_D3D11()
{

}

void RevDev::WindowHandler_D3D11::Setup()
{
	setupDeviceAndSwap();
	SetupRenderTargetAndStencelBuffer();
	SetupRasterizerState();

	setupPipeline();
}

void WindowHandler_D3D11::updateWindow()
{
	pSwapChain->Present(1, 0);
	clearBuffer(m_BackgroundColour);
}

void WindowHandler_D3D11::clearBuffer(float backgroundColour[4])
{
	pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), backgroundColour);
	pDeviceContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1,0);
}

void WindowHandler_D3D11::setupPipeline()
{
	//Set type of rendering (point, line (strip), triangle (strip),.... Strip -> 0,1,2,3,4... Non-Strip = (0 - 1), (1 - 2), (5 - 0),...
	pDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	wrl::ComPtr<ID3D11BlendState> blendState;
	pDevice->CreateBlendState(&blendDesc, blendState.GetAddressOf());

	float blendColor[4];
	blendColor[0] = 0.0f;
	blendColor[1] = 0.0f;
	blendColor[2] = 0.0f;
	blendColor[3] = 0.0f;
	pDeviceContext->OMSetBlendState(blendState.Get(), blendColor, 0xffffffff);
	
	//Config viewport -> pixelshader target (renderTarget) From ndc to render view
	D3D11_VIEWPORT viewPort{};
	viewPort.Width = float(m_WindowWidth);
	viewPort.Height = float(m_WindowHeight);
	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;

	pDeviceContext->RSSetViewports(1, &viewPort);
}

void WindowHandler_D3D11::setupDeviceAndSwap()
{
	//Configure the desc(options) for the swapchain pointer
	DXGI_SWAP_CHAIN_DESC swapChainDESC = { 0 };
	ZeroMemory(&swapChainDESC, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDESC.BufferDesc.Width = 0;
	swapChainDESC.BufferDesc.Height = 0; //Width and height unspecified
	swapChainDESC.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDESC.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDESC.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDESC.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDESC.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDESC.SampleDesc.Count = 1;
	swapChainDESC.SampleDesc.Quality = 0; //Just means at this point no Anti Aliasing
	swapChainDESC.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //Use this buffer as render target
	swapChainDESC.BufferCount = 1; //one back buffer and one front
	swapChainDESC.OutputWindow = hwnd; //What window to output on
	swapChainDESC.Windowed = true;
	swapChainDESC.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDESC.Flags = 0;


	D3D_FEATURE_LEVEL feature_level;
	UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	//Creates swapchain, device and device context
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		flags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDESC,
		&pSwapChain,
		&pDevice,
		&feature_level,
		&pDeviceContext);
	assert(S_OK == hr && pSwapChain && pDevice && pDeviceContext);
}

void WindowHandler_D3D11::SetupRenderTargetAndStencelBuffer()
{
	HRESULT hr = pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(void**)&pFramebuffer);
	assert(SUCCEEDED(hr));

	hr = pDevice->CreateRenderTargetView(
		pFramebuffer.Get(),
		nullptr,
		&pRenderTargetView);
	assert(SUCCEEDED(hr));

	//pFramebuffer->Release();
	//Z buffer
	D3D11_DEPTH_STENCIL_DESC dsDesc{};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	pDevice->CreateDepthStencilState(&dsDesc, &pDSState);
	pDeviceContext->OMSetDepthStencilState(pDSState.Get(), 0);

	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth{};
	descDepth.Width = 700;
	descDepth.Height = 500;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStectilView = {};
	descDepthStectilView.Format = DXGI_FORMAT_D32_FLOAT;
	descDepthStectilView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDepthStectilView.Texture2D.MipSlice = 0u;
	pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDepthStectilView, &pDepthStencilView);

	// bind depth stensil view to OM
	pDeviceContext->OMSetRenderTargets(1u, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());
}

void WindowHandler_D3D11::SetupRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = TRUE;
	rasterizerDesc.DepthClipEnable = TRUE;

	wrl::ComPtr<ID3D11RasterizerState> rasterizerState;
	pDevice->CreateRasterizerState(&rasterizerDesc, &rasterizerState);

	pDeviceContext->RSSetState(rasterizerState.Get());
}