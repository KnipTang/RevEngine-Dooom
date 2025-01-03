#pragma once

#include <memory>
#include <functional>
#include <string>
#include "wrl.h"
#include <d3d11.h>

namespace wrl = Microsoft::WRL;

namespace Rev
{
	struct TextureData
	{
		int ImageWidth = 0;
		int ImageHeight = 0;
		int ImageChannels = 0;
		int ImageDesiredChannels = 4;
	};

	class Texture
	{
	public:
		Texture(ID3D11Device* pDevice, std::string path);
		~Texture();

		TextureData* GetTextureDate() { return m_TextureDate.get(); }
		unsigned char* GetImageData() { return m_ImageData; }

		ID3D11ShaderResourceView* GetShaderResourceView() { return m_ImageShaderResourceView.Get(); }

	private:
		void LoadTexture(const char* path);

		void SetupTexture(ID3D11Device* pDevice);
		void SetupShaderResourceView(ID3D11Device* pDevice);
	private:
		unsigned char* m_ImageData;
		std::unique_ptr<TextureData> m_TextureDate;

		wrl::ComPtr<ID3D11Texture2D> m_ImageTexture;
		wrl::ComPtr<ID3D11ShaderResourceView> m_ImageShaderResourceView;
	};
}