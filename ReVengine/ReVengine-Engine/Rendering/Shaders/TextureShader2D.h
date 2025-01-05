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
	class TextureShader2D final : public Rev::BaseShader
	{
	private:
		struct MatrixBufferType
		{
			DirectX::XMMATRIX ortho;
		};
	public:
		TextureShader2D();
		~TextureShader2D();

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

		std::string m_VertexFile = "../engine_resources/shaders/UI_vs.hlsl";
		std::string m_PixelFile = "../engine_resources/shaders/PixelShader.hlsl";
	};
}