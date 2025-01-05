#include "TextureShader.h"
#include "Rendering/Texture.h"
#include "Rev_CoreSystems.h"

using namespace Rev;

TextureShader::TextureShader() :
	BaseShader(Rev::Rev_CoreSystems::pRevRender.get()->GetDevice(), Rev::Rev_CoreSystems::pRevRender.get()->GetDeviceContext())
{
	LoadShaders(m_VertexFile, m_PixelFile);
	InitShader();
}

TextureShader::~TextureShader()
{

}

void TextureShader::InitShader()
{
	SetupInputLayer();
	SetupShaderBuffers();
	SetupSampler();
}

void TextureShader::SetupShaderBuffers()
{
	D3D11_BUFFER_DESC matrixBuffer_DESC{};
	matrixBuffer_DESC.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBuffer_DESC.Usage = D3D11_USAGE_DYNAMIC;
	matrixBuffer_DESC.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBuffer_DESC.MiscFlags = 0;
	matrixBuffer_DESC.ByteWidth = sizeof(MatrixBufferType);
	matrixBuffer_DESC.StructureByteStride = 0;
	m_Device->CreateBuffer(&matrixBuffer_DESC, NULL, &m_MatrixBuffer);
}
void TextureShader::SetupSampler()
{
	D3D11_SAMPLER_DESC samplerDesc = {};

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 1.0f;
	samplerDesc.BorderColor[2] = 1.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;

	[[maybe_unused]] HRESULT result = m_Device->CreateSamplerState(&samplerDesc, &m_SamplerState);

	assert(SUCCEEDED(result));
}

void TextureShader::SetShader(const DirectX::XMMATRIX modelMatrix, const DirectX::XMMATRIX viewMatrix, const DirectX::XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	DirectX::XMMATRIX transWorld = DirectX::XMMatrixTranspose(modelMatrix);
	DirectX::XMMATRIX transView = DirectX::XMMatrixTranspose(viewMatrix);
	DirectX::XMMATRIX transProj = DirectX::XMMatrixTranspose(projectionMatrix);

	D3D11_MAPPED_SUBRESOURCE msr;
	m_DeviceContext->Map(m_MatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	MatrixBufferType* data = (MatrixBufferType*)msr.pData;
	data->world = transWorld;
	data->view = transView;
	data->projection = transProj;
	m_DeviceContext->Unmap(m_MatrixBuffer.Get(), 0);

	m_DeviceContext->VSSetConstantBuffers(0, 1, m_MatrixBuffer.GetAddressOf());

	m_DeviceContext->PSSetShaderResources(0, 1, &texture);
	m_DeviceContext->PSSetSamplers(0, 1, m_SamplerState.GetAddressOf());
}
