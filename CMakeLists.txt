cmake_minimum_required(VERSION 3.5)


    project(SDLAsteroids VERSION 0.1 LANGUAGES CXX)

    set(PROJECT_SOURCES
				SDLAsteroids.cpp
                GameObject.cpp GameObject.h
                Ship.cpp Ship.h
                Rock.cpp Rock.h
                Bullet.cpp Bullet.h
                RVector2D.cpp RVector2D.h
                RTexture.cpp RTexture.h
                RockFactory.cpp RockFactory.h
	)

    add_executable(SDLAsteroids ${PROJECT_SOURCES})

    set(SDL2_STATIC_LIBRARIES FALSE)

    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

    if (MSYS)
        message("--> MSYS " $ENV{USER})
        add_compile_definitions(_MSYS )
    endif()

    if (LINUX)
        message("--> LINUX " $ENV{USER})
        #add_compile_definitions(_MSYS )
    endif()

    target_compile_features(SDLAsteroids PUBLIC cxx_std_20)

    target_compile_definitions(SDLAsteroids PRIVATE SDL2_SHARED)

    target_link_libraries(SDLAsteroids SDL2 SDL2main SDL2_ttf SDL2_mixer SDL2_image fmt)
