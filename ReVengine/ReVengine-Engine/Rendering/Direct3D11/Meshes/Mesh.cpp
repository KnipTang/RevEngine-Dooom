#include "Mesh.h"
#include "d3d11.h"
#include "Rendering/Direct3D11/WindowHandler.h"
#include "Utils/Vertex.h"
#include "Rendering/Shaders/BaseShader.h"

using namespace RevDev;

uint32_t Mesh::s_MeshIDCounter = 0;

Mesh::Mesh(ID3D11Device* pDevice) :
	m_Device{pDevice},
	m_IndiceCount{},
	m_MeshID{s_MeshIDCounter++}
{
}

Mesh::~Mesh()
{

}

void Mesh::SetupVertexBuffer(const std::vector<Vertex> vertices)
{
	D3D11_BUFFER_DESC vertexBuffer_DESC{ 0 };
	vertexBuffer_DESC.BindFlags = D3D11_BIND_VERTEX_BUFFER; //Type of vertex buffer
	vertexBuffer_DESC.Usage = D3D11_USAGE_DEFAULT; //How buffer communicates with gpu (if the gpu can also write back to the cpu or not)
	vertexBuffer_DESC.CPUAccessFlags = 0;
	vertexBuffer_DESC.MiscFlags = 0;
	vertexBuffer_DESC.ByteWidth = UINT(vertices.size() * sizeof(Vertex));
	vertexBuffer_DESC.StructureByteStride = sizeof(Vertex);

	//The data of the vertex
	D3D11_SUBRESOURCE_DATA subResc_DATA{ 0 };
	subResc_DATA.pSysMem = vertices.data();

	HRESULT result = m_Device->CreateBuffer(&vertexBuffer_DESC, &subResc_DATA, &m_VertexBuffer);
	assert(SUCCEEDED(result));
}

void Mesh::SetupIndexBuffer(const std::vector<unsigned short> indices) 
{
	D3D11_BUFFER_DESC indexBuffer_DESC{};
	indexBuffer_DESC.BindFlags = D3D11_BIND_INDEX_BUFFER; //Type of vertex buffer
	indexBuffer_DESC.Usage = D3D11_USAGE_DEFAULT; //How buffer communicates with gpu (if the gpu can also write back to the cpu or not)
	indexBuffer_DESC.CPUAccessFlags = 0;
	indexBuffer_DESC.MiscFlags = 0;
	indexBuffer_DESC.ByteWidth = UINT(indices.size() * sizeof(unsigned short));
	indexBuffer_DESC.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA subRescIndex_DATA{ 0 };
	subRescIndex_DATA.pSysMem = indices.data();

	HRESULT result = m_Device->CreateBuffer(&indexBuffer_DESC, &subRescIndex_DATA, &m_IndexBuffer);
	assert(SUCCEEDED(result));

	m_IndiceCount = UINT(indices.size());
}