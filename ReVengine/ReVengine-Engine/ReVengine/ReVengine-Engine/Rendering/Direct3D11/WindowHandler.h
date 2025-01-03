#pragma once

#include "d3d11.h"
#include "SDL.h"
#include <SDL_syswm.h>
#include <string>
#include "wrl.h"
#include <chrono>
#include <DirectXMath.h>
#include <vector>
#include <memory>

//Using comPtr's to manage windows objects in a smart way
namespace wrl = Microsoft::WRL;

namespace RevDev
{
	class WindowHandler_D3D11
	{
	public:
		WindowHandler_D3D11(SDL_Window* window, int windowWidth, int windowHeight);
		~WindowHandler_D3D11();

		void Setup();

		void updateWindow();

		ID3D11Device* GetDevice() { return pDevice.Get(); }
		ID3D11DeviceContext* GetDeviceContext() { return pDeviceContext.Get(); }

	private:
		void setupPipeline();

		void setupDeviceAndSwap();
		void SetupRenderTargetAndStencelBuffer(); //Set backbuffer to pass to and create rendertargetview
		void SetupRasterizerState();

		void clearBuffer(float backgroundColour[4]); //Set background color and clear back buffer
	private:
		//The head of directX From device all the other achitecture gets created and linked to.
		wrl::ComPtr<ID3D11Device> pDevice = NULL; //A device is used to create resources and to enumerate the capabilities of a display adapter

		//Used to execute commands to the gpu, draw calls,.. // Sets the settings of the pipeline(What shaders to use, what options (on those shaders))
		wrl::ComPtr<ID3D11DeviceContext> pDeviceContext = NULL; //A device context contains the circumstance or setting in which a device is used. More specifically, a device context is used to set pipeline state and generate rendering commands using the resources owned by a device.
	
		//Swaps between textures, once the back is done calculating, the back becomes the front and the process starts again.
		wrl::ComPtr<IDXGISwapChain> pSwapChain = NULL;

		//Used to specifiy which pixel is being targeted atm
		wrl::ComPtr<ID3D11RenderTargetView> pRenderTargetView = NULL;

		//For Z-Buffering
		wrl::ComPtr<ID3D11DepthStencilView> pDepthStencilView;

		//Holds both back and front chain. (front is what the user sees, back is what is being calculated before the user sees it)
		wrl::ComPtr<ID3D11Texture2D> pFramebuffer;

		//Windows window handle.
		HWND hwnd;
		int m_WindowWidth;
		int m_WindowHeight;

		float m_BackgroundColour[4] = { 0x64 / 255.0f, 0x95 / 255.0f, 0xED / 255.0f, 0.0f };
	};
}
