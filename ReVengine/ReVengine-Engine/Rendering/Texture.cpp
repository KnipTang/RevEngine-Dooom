#include "Texture.h"
#include "Rev_CoreSystems.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <SDL_messagebox.h>

using namespace Rev;

Texture::Texture(ID3D11Device* pDevice, std::string path)
{
	LoadTexture(path.c_str());
	SetupTexture(pDevice);
	SetupShaderResourceView(pDevice);
}

Texture::~Texture()
{

}

void Texture::LoadTexture(const char* path)
{
	m_TextureDate = std::make_unique<TextureData>();

	m_ImageData = stbi_load(
		path,
		&m_TextureDate->ImageWidth,
		&m_TextureDate->ImageHeight,
		&m_TextureDate->ImageChannels, 
		m_TextureDate->ImageDesiredChannels
	);
	if (m_ImageData == nullptr)
	{
		std::string message = "Texture's path\"" + std::string(path) + "\" not found!";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Resource Manager", message.c_str(), nullptr);
		assert(m_ImageData);
	}
}

void Texture::SetupTexture(ID3D11Device* pDevice)
{
	auto&& data = GetTextureDate();
	auto&& imageData = GetImageData();

	int ImagePitch = data->ImageWidth * 4;

	D3D11_TEXTURE2D_DESC ImageTextureDesc = {};

	ImageTextureDesc.Width = (UINT)data->ImageWidth;
	ImageTextureDesc.Height = (UINT)data->ImageHeight;
	ImageTextureDesc.MipLevels = 1;
	ImageTextureDesc.ArraySize = 1;
	ImageTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	ImageTextureDesc.SampleDesc.Count = 1;
	ImageTextureDesc.SampleDesc.Quality = 0;
	ImageTextureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ImageTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA ImageSubresourceData = {};

	ImageSubresourceData.pSysMem = imageData;
	ImageSubresourceData.SysMemPitch = ImagePitch;

	HRESULT result = pDevice->CreateTexture2D(&ImageTextureDesc,
		&ImageSubresourceData,
		&m_ImageTexture
	);

	assert(SUCCEEDED(result));
}

void Texture::SetupShaderResourceView(ID3D11Device* pDevice)
{
	HRESULT result = pDevice->CreateShaderResourceView(m_ImageTexture.Get(),
		nullptr,
		&m_ImageShaderResourceView
	);
	assert(SUCCEEDED(result));
}