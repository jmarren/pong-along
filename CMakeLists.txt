

cmake_minimum_required(VERSION 3.16)
project(client)
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
add_executable(client WIN32 
	client/client.c 
	client/client.h 
	client/view/view.c
	client/view/view.h
	client/view/dashboard.c
	client/view/dashboard.h
	client/view/text.c
	client/view/text.h
	client/net/net.c
	client/net/net.h
	client/net/tcp.c
	client/net/tcp.h
	client/net/udp.c
	client/net/udp.h
	client/net/shared.c
	client/net/shared.h
	shared/parse.h
	shared/parse.c
	client/loop.c
	client/loop.h
	client/view/circle.c
	client/view/circle.h
	client/view/physics.c
	client/view/physics.h
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
target_link_libraries(client PRIVATE SDL3_ttf::SDL3_ttf SDL3::SDL3)
target_link_libraries(client PRIVATE uv_a)
target_link_libraries(server PRIVATE uv_a)
# target_link_libraries(hello PRIVATE SDL3_ttf::SDL3_ttf SDL3::SDL3)
# target_include_direc

# target_link_libraries(view PRIVATE uv_a)
