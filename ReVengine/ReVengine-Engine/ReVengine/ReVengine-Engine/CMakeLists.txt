add_library(ReVengine-Engine
    "ReVengine.cpp"
    "Rev_CoreSystems.cpp"
    "Rendering/RenderWindow.cpp"
    "Rendering/Texture.cpp"
    "Rendering/Direct3D11/Meshes/Mesh.cpp"
    "Rendering/Shaders/BaseShader.cpp"
    "Rendering/Shaders/TextureShader.cpp"
    "Rendering/Shaders/TextureShader2D.cpp" 
    "Rendering/Direct3D11/WindowHandler.cpp" 
    "Rendering/Camera/Camera.cpp"
    "Sound/Rev_Sound.cpp"
    "Sound/Rev_SoundImpl.cpp"
    "Sound/SoLoud/Rev_SoLoud.cpp"
    "GameObjects/GameObject.cpp"
    "GameObjects/BaseComponent.cpp"
    "GameObjects/Components/CompTransform.cpp"
    "GameObjects/Components/CompRender.cpp"
    "GameObjects/Components/CompInput.cpp" 
    "GameObjects/Components/CompCamera.cpp" 
    "GameObjects/Components/CompCollision.cpp"
    "Scenes/Scene.cpp"
    "Scenes/SceneManager.cpp"
    "Input/InputManager.cpp"
    "ResourceManagement/MemoryPool.cpp"
    "ResourceManagement/ResourceManager.cpp"
    "Physics/PhysX/Physics_PhysX.cpp"
    "Physics/Physics.cpp"
    "Physics/PhysicsImpl.cpp"
    "Physics/PhysX/CollisionCallback.cpp"
    "Rendering/HUD/ImGui/ImGuiSetup.cpp"
 "Rendering/HUD/UI.cpp" "Rendering/HUD/UI.h") #No need to add header files

target_include_directories(ReVengine-Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR}/GameSettings) #adds current dir of this file to include path of engine dir, Public so everyone that links with this can access there files lol

#"${D3D11_LIBRARY}" "D3DX11_LIBRARY"
target_link_libraries(ReVengine-Engine PUBLIC 
    soloud 
    dxgi d3d11 d3dcompiler
    SDL2::SDL2 SDL2::SDL2main
    glm::glm
    unofficial::omniverse-physx-sdk::sdk
    imgui::imgui
) #link libs to engine, name "soload" is the "target_name"

target_include_directories(ReVengine-Engine PUBLIC 
    ${soloud_SOURCE_DIR}/include
    ${Stb_INCLUDE_DIR}
)

if(MSVC)
  target_compile_options(ReVengine-Engine PRIVATE /W4 /WX)
else()
  target_compile_options(ReVengine-Engine PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()


set(RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Resources")
set(SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Rendering/HLSLshaders")

add_custom_target(copy_engine_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${RESOURCES_DIR}"
        "${CMAKE_BINARY_DIR}/engine_resources/Resources"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SHADER_DIR}"
        "${CMAKE_BINARY_DIR}/engine_resources/shaders"
    COMMENT "Copying resources and shaders to build directory"
)

# add this as a dependency to the engine
add_dependencies(ReVengine-Engine copy_engine_resources)



set(DIRECTX11_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Rendering")

set(SHADERS_DIR "${DIRECTX11_DIR}/HLSLshaders")
set(SHADERS_OUT_DIR "${CMAKE_BINARY_DIR}/Rendering/HLSLshaders")

# make shader output dir
file(MAKE_DIRECTORY "${SHADERS_OUT_DIR}")

function(compile_shader IN_FILE OUT_FILE PROFILE ENTRY FILES_LIST)
        set(DEBUG_FLAGS "/Zi /Od")
    add_custom_command(
        OUTPUT ${OUT_FILE}
        COMMAND fxc.exe /T ${PROFILE} /E ${ENTRY} /Fo ${OUT_FILE} /Zi /Od ${IN_FILE}
        DEPENDS ${IN_FILE}
        VERBATIM
    )
     
    list(APPEND ${FILES_LIST} ${OUT_FILE})

    set(${FILES_LIST} "${${FILES_LIST}}" PARENT_SCOPE)
endfunction()

#Compiling vertex shaders
foreach(FILE ${SHADERS_DIR}/VertexShader.hlsl)
    get_filename_component(SHADER_NAME ${FILE} NAME_WE) # get shader name without extension
    
    compile_shader(${FILE} ${SHADERS_OUT_DIR}/${SHADER_NAME}.cso vs_5_0 vs_main COMPILED_SHADER_FILES)
endforeach()

foreach(FILE ${SHADERS_DIR}/PixelShader.hlsl)
    get_filename_component(SHADER_NAME ${FILE} NAME_WE) # get shader name without extension
    
    compile_shader(${FILE} ${SHADERS_OUT_DIR}/${SHADER_NAME}.cso ps_5_0 ps_main COMPILED_SHADER_FILES)
endforeach()

message("Adding CUSTOM TARGET on ${COMPILED_SHADER_FILES}")

add_custom_target(compile_shaders DEPENDS ${COMPILED_SHADER_FILES})
add_dependencies(ReVengine-Engine compile_shaders)