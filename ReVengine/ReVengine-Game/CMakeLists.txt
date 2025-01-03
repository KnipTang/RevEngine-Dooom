add_executable(ReVengine-Game 
"Game.cpp" 
"Objects/Weapons/BulletComp.cpp"
"Objects/Weapons/GunComp.cpp"
"Objects/Enemies/LookAtPlayerComp.cpp"
"Objects/Enemies/EnemyComp.cpp"
"Objects/Enemies/EnemyComp.h" 
"Objects/Player/PlayerComp.cpp"
 "Objects/HealthComp.cpp" 
 "Objects/ObjectSpawnerComp.cpp") #No need to add header files

target_link_libraries(ReVengine-Game ReVengine-Engine) #links Game to Engine lib(dll)

set(RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Resources")

add_custom_target(copy_game_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${RESOURCES_DIR}"
        "${CMAKE_BINARY_DIR}/game_resources"
    COMMENT "Copying resources to build directory"
)

set(GAME_SETTINGS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/GameSettings.h")

add_custom_command(TARGET ReVengine-Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${GAME_SETTINGS_FILE}"
        "${CMAKE_BINARY_DIR}/GameSettings/GameSettings.h"
    COMMENT "Copying MyHeader.h to the build directory"
)
set_property(GLOBAL PROPERTY EXCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/GameSettings")

add_dependencies(ReVengine-Game copy_game_resources) #Adds this as a dependency to the game