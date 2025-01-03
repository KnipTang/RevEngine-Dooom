#pragma once

#include <memory>
#include <functional>
#include <DirectXMath.h>
#include "Camera/Camera.h"
#include "glm/matrix.hpp"
#include "d3d11.h"
#include "SDL_scancode.h"
#include "SDL_events.h"
#include <set>

namespace Rev
{
	class TextureShader;
}

namespace RevDev
{
	class WindowHandler_D3D11;
	class Mesh;
	class ImGuiSetup;
}

struct SDL_Window;
struct Vertex;

namespace RevDev
{
	class RenderWindow
	{
	public:
		RenderWindow();
		~RenderWindow();

		bool InitWindow(int windowWidth, int windowHeight, float nearZ, float farZ);

		uint32_t AddMesh(const std::vector<Vertex> vertices, const std::vector<unsigned short> indices);
		void DrawMesh(uint32_t meshId);

		bool UpdateWindow();

		void RipWindow();

		void LoadTexture();

		const DirectX::XMMATRIX getProjectionMatrix();

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();

		RevDev::ImGuiSetup* GetImGuiHandle() { return m_ImGui.get(); }
	
	private:

		std::unique_ptr<WindowHandler_D3D11> m_CreatorGod;
		std::unique_ptr<RevDev::ImGuiSetup> m_ImGui;

		//The window we'll be rendering to
		std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>> m_Window;

		std::vector<std::unique_ptr<Mesh>> m_Meshes;

		//The size of each vertex in mem, this way the gpu knows how many bytes there are in each vertex
		uint32_t m_VertexStride;
		//OffSet between vertecies
		uint32_t m_VertexOffset = 0;

		int m_WindowWidth;
		int m_WindowHeight;

		DirectX::XMMATRIX m_ProjectionMatrix;

		std::set<SDL_Scancode> m_LastPressedKeys;
		std::set<Uint8> m_LastPressedMouse;
	};
}