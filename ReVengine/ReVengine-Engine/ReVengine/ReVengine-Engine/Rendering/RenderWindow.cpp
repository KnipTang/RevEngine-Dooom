#include "RenderWindow.h"
#include "ReVengine.h"
#include "SDL.h"
#include "d3d11.h"
#include "Direct3D11/WindowHandler.h"
#include <memory>
#include "Utils/Vertex.h"
#include <algorithm>
#include "Texture.h"
#include "Rendering/Direct3D11/Meshes/Mesh.h"
#include "Rendering/Shaders/TextureShader.h"
#include <Rev_CoreSystems.h>
#include <iostream>
#include "HUD/ImGui/ImGuiSetup.h"

#undef min
#undef max

using namespace RevDev;

RenderWindow::RenderWindow()
{
    m_VertexStride = sizeof(Vertex);
}

RenderWindow::~RenderWindow()
{
    m_CreatorGod.reset();
}

bool RenderWindow::InitWindow(int windowWidth, int windowHeight, float nearZ, float farZ) {
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    else
    {
        m_WindowWidth = windowWidth;
        m_WindowHeight = windowHeight;

        m_ProjectionMatrix = DirectX::XMMatrixPerspectiveLH(
            1.f,
            std::min(float(m_WindowWidth), float(m_WindowHeight)) / std::max(float(m_WindowWidth), float(m_WindowHeight)),
            nearZ,
            farZ
        );

        //Create window
        m_Window = std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>>(
            SDL_CreateWindow("WINDOW OF GODS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_WindowWidth, m_WindowHeight, SDL_WINDOW_SHOWN),
            SDL_DestroyWindow
        );

        if (m_Window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }

        //SDL_WarpMouseInWindow(m_Window.get(), m_WindowWidth / 2, m_WindowHeight / 2);
        SDL_SetRelativeMouseMode(SDL_TRUE);
        //SDL_ShowCursor(SDL_ENABLE);

        {
            m_CreatorGod = std::make_unique<WindowHandler_D3D11>(m_Window.get(), m_WindowWidth, m_WindowHeight);
            m_CreatorGod->Setup();
        }
        {
            //Imgui
            m_ImGui = std::make_unique<RevDev::ImGuiSetup>();
            m_ImGui->InitWindow(m_WindowWidth, m_WindowHeight);
            m_ImGui->InitForSDL2(*m_Window.get());
            m_ImGui->InitFor3D11(*m_CreatorGod->GetDevice(), *m_CreatorGod->GetDeviceContext());
        }

        return true;
    }
}

uint32_t RenderWindow::AddMesh(const std::vector<Vertex> vertices, const std::vector<unsigned short> indices)
{
    RevDev::Mesh& mesh = *m_Meshes.emplace_back(std::make_unique<Mesh>(m_CreatorGod->GetDevice())).get();

    mesh.setupVertexBuffer(vertices);
    mesh.setupIndexBuffer(indices);

    return mesh.GetID();
}

void RenderWindow::DrawMesh(uint32_t meshId)
{
    ID3D11DeviceContext* pDeviceContext = m_CreatorGod->GetDeviceContext();

    auto&& mesh = m_Meshes.at(meshId);

    pDeviceContext->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer().GetAddressOf(), &m_VertexStride, &m_VertexOffset);
    pDeviceContext->IASetIndexBuffer(mesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

    pDeviceContext->DrawIndexed(mesh->GetIndiceCount(), 0, 0);
}

bool RenderWindow::UpdateWindow()
{
    m_ImGui->Update();
    //To get window to stay up
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_KEYDOWN:
            m_LastPressedKeys.insert(e.key.keysym.scancode);
            break;
        case SDL_KEYUP:
            m_LastPressedKeys.erase(e.key.keysym.scancode);
            break;
        case SDL_MOUSEBUTTONDOWN:
            m_LastPressedMouse.insert(e.button.button);
            break;
        case SDL_MOUSEBUTTONUP:
            m_LastPressedMouse.erase(e.button.button);
            break;

        case SDL_MOUSEMOTION:
            break;

        case SDL_QUIT:
                return true;
            break;
        }
    }    

    for (const auto& key : m_LastPressedKeys)
    {
        Rev::Rev_CoreSystems::pInputManager->HandleKeyDown(key);
    }
    for (const auto& mouse : m_LastPressedMouse)
    {
        Rev::Rev_CoreSystems::pInputManager->HandleMouseKeyDown(mouse);
    }

    int xRel, yRel;
    SDL_GetRelativeMouseState(&xRel, &yRel);
    Rev::Rev_CoreSystems::pInputManager->HandleMouseRelativeMotion(xRel, yRel);

    m_CreatorGod->updateWindow();

    return false;
}

void RenderWindow::RipWindow()
{
    //Destroy window
    SDL_DestroyWindow(m_Window.get());

    //Quit SDL subsystems
    SDL_Quit();
}

const DirectX::XMMATRIX RenderWindow::getProjectionMatrix()
{
    return m_ProjectionMatrix;
}

ID3D11Device* RenderWindow::GetDevice()
{
    return m_CreatorGod->GetDevice();
}
ID3D11DeviceContext* RenderWindow::GetDeviceContext()
{
    return m_CreatorGod->GetDeviceContext();
}