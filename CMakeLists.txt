cmake_minimum_required(VERSION 3.16)

# C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\Launch-VsDevShell.ps1
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake") # temp
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(cpwd CXX)

find_package(nowide CONFIG REQUIRED)

add_executable(cpwd
	src/cpwd.cpp
)
target_compile_definitions(cpwd PRIVATE
	UNICODE
	_UNICODE
	)
target_link_libraries(cpwd PRIVATE nowide::nowide)
target_compile_features(cpwd PUBLIC cxx_std_17)
