

cmake_minimum_required(VERSION 3.16)
project(main)
project(server)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Use vendored libs
set(SDLTTF_VENDORED OFF)

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)
add_subdirectory(vendored/libuv EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_executable(main WIN32 
	main.c 
	app.h
	view/view.c
	view/view.h
	view/dashboard.c
	view/dashboard.h
	view/text.c
	view/text.h
	net/net.c
	net/net.h
	net/tcp.c
	net/tcp.h
	net/udp.c
	net/udp.h
	net/shared.c
	net/shared.h
	shared/parse.h
	shared/parse.c
	loop/loop.c
	loop/loop.h
	view/circle.c
	view/circle.h
	view/physics.c
	view/physics.h
	vendored/SDL_ttf/include/SDL3_ttf/SDL_ttf.h
	vendored/cc/array.c
	vendored/cc/array.h
	vendored/cc/cc_common.h
	vendored/cc/cc_common.c
)

add_executable(server WIN32
	server/server.c
	server/server.h
	shared/parse.c
	shared/parse.h
	server/tcp.c
	server/tcp.h
	server/udp.c
	server/udp.h
	server/handlers/handlers.c
	server/handlers/handlers.h
	server/shared.c
	server/shared.h
)

# Link to the actual SDL3 library.
target_link_libraries(main PRIVATE SDL3_ttf::SDL3_ttf SDL3::SDL3)
target_link_libraries(main PRIVATE uv_a)
target_link_libraries(server PRIVATE uv_a)
# target_link_libraries(hello PRIVATE SDL3_ttf::SDL3_ttf SDL3::SDL3)
# target_include_direc

# target_link_libraries(view PRIVATE uv_a)
