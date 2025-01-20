cmake_minimum_required(VERSION 3.14)
project(main)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(SOURCE_FILES src/main.cpp
        src/Player.cpp
        src/Player.h
        src/WorldGenerator.cpp
        src/WorldGenerator.h
        src/Block.h
        src/Camera.h
        src/Camera.cpp
        src/Math.h
        src/Circle.h
        src/Physics.h
        src/Button.h
        src/Interface.h
        src/CollisionShape.h
        src/Signal.h
        src/Signals.h
        src/GameManager.h
        src/Signals.cpp
        src/GuideArrow.h
        src/Background.h
)
add_executable(main ${SOURCE_FILES})
include_directories(/usr/local/include)

file(COPY data DESTINATION ${CMAKE_BINARY_DIR})

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(main sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
