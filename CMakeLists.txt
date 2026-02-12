

cmake_minimum_required(VERSION 3.16)
project(main)
project(server)
project(client)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/libuv EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_executable(main WIN32 
	main.c 
	view/view.c
	view/view.h
	net/net.c
	net/net.h
	loop/loop.c
	loop/loop.h
	view/circle.c
	view/circle.h
	view/physics.c
	view/physics.h
)
add_executable(server WIN32 server.c)
add_executable(client WIN32 client.c)

# Link to the actual SDL3 library.
target_link_libraries(main PRIVATE SDL3::SDL3)
target_link_libraries(main PRIVATE uv_a)
target_link_libraries(server PRIVATE uv_a)
target_link_libraries(client PRIVATE uv_a)
# target_include_direc

# target_link_libraries(view PRIVATE uv_a)
