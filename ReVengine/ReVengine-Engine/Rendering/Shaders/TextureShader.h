#pragma once

#include "d3d11.h"
#include <string>

#include "wrl.h"
#include "BaseShader.h"

namespace wrl = Microsoft::WRL;

namespace Rev {
	class Texture;
}

namespace Rev
{
	class TextureShader final : public Rev::BaseShader
	{
	private:
		struct MatrixBufferType
		{
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX projection;
		};

	public:
		TextureShader();
		~TextureShader();

		void InitShader();
		void SetShader(const DirectX::XMMATRIX modelMatrix, const DirectX::XMMATRIX viewMatrix, const DirectX::XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture) override; //Every frame

		std::string GetVertexByteCode() { return m_VertexBytecode; }
		wrl::ComPtr<ID3D11Buffer> GetMatrixBuffer() { return m_MatrixBuffer; }

	private:
		void SetupShaderBuffers();
		void SetupSampler();

	private:

		wrl::ComPtr<ID3D11Buffer> m_MatrixBuffer;

		wrl::ComPtr<ID3D11SamplerState> m_SamplerState;

		std::string m_VertexFile = "../engine_resources/shaders/VertexShader.hlsl";
		std::string m_PixelFile = "../engine_resources/shaders/PixelShader.hlsl";
	};
}