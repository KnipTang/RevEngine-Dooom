#include "BaseShader.h"
#include <d3dcompiler.h>

using namespace Rev;

BaseShader::BaseShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext) :
	m_Device{ pDevice },
	m_DeviceContext{ pDeviceContext }
{
}

void Rev::BaseShader::SetupInputLayer()
{
	const D3D11_INPUT_ELEMENT_DESC inputElement_DESC[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	m_Device->CreateInputLayout(inputElement_DESC, std::size(inputElement_DESC), m_VertexBytecode.c_str(), m_VertexBytecode.size(), &m_InputLayer);
}

void BaseShader::SetShaderStages()
{
	m_DeviceContext->IASetInputLayout(m_InputLayer.Get());

	m_DeviceContext->VSSetShader(m_VertexShader.Get(), 0, 0);
	m_DeviceContext->PSSetShader(m_PixelShader.Get(), 0, 0);
}

void BaseShader::LoadShaders(std::string vertexFile, std::string pixelFile)
{
	std::wstring wideVertexFile = std::wstring(vertexFile.begin(), vertexFile.end());
	ID3DBlob* vsBlob = nullptr;
	HRESULT hr = CompileShader(wideVertexFile.c_str(), "vs_main", "vs_5_0", &vsBlob);
	if (FAILED(hr))
	{
		printf("Failed compiling vertex shader %08X\n", hr);
	}

	std::wstring widePixelFile = std::wstring(pixelFile.begin(), pixelFile.end());
	ID3DBlob* psBlob = nullptr;
	hr = CompileShader(widePixelFile.c_str(), "ps_main", "ps_5_0", &psBlob);
	if (FAILED(hr))
	{
		printf("Failed compiling pixel shader %08X\n", hr);
	}

	m_VertexBytecode.assign((char*)vsBlob->GetBufferPointer(), (char*)vsBlob->GetBufferPointer() + vsBlob->GetBufferSize());

	hr = m_Device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_VertexShader);
	if (FAILED(hr))
	{
		printf("Failed creating vertex shader %08X\n", hr);
	}
	m_DeviceContext->VSSetShader(m_VertexShader.Get(), 0, 0);

	hr = m_Device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_PixelShader);
	if (FAILED(hr))
	{
		printf("Failed creating pixel shader %08X\n", hr);
	}
}
HRESULT BaseShader::CompileShader(LPCWSTR srcFile, LPCSTR entryPoint, LPCSTR profile, ID3DBlob** blob)
{
	if (!srcFile || !entryPoint || !profile || !blob)
		return E_INVALIDARG;

	*blob = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif

	const D3D_SHADER_MACRO defines[] =
	{
		"EXAMPLE_DEFINE", "1",
		NULL, NULL
	};

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint, profile,
		flags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (shaderBlob)
			shaderBlob->Release();

		return hr;
	}

	*blob = shaderBlob;

	return hr;
}