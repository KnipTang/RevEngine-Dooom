#pragma once

#include <vector>
#include "wrl.h"
#include "d3d11.h"
#include <memory>
#undef PlaySound

namespace wrl = Microsoft::WRL;

struct Vertex;

namespace Rev
{
	class Texture;
	class TextureShader;
}

namespace RevDev
{
	class Mesh
	{
	public:
		Mesh(ID3D11Device* pDevice);
		~Mesh();

		void setupVertexBuffer(const std::vector<Vertex> vertices);
		void setupIndexBuffer(const std::vector<unsigned short> indices);

		const uint32_t GetID() { return meshID; }
		wrl::ComPtr<ID3D11Buffer> GetVertexBuffer() { return m_VertexBuffer; }
		wrl::ComPtr<ID3D11Buffer> GetIndexBuffer() { return m_IndexBuffer; }
		const UINT GetIndiceCount() { return m_IndiceCount; }

	private:
		ID3D11Device* m_Device;

		wrl::ComPtr<ID3D11Buffer> m_VertexBuffer;
		wrl::ComPtr<ID3D11Buffer> m_IndexBuffer;
		UINT m_IndiceCount;

		static uint32_t meshIDCounter;
		uint32_t meshID;
	};
}