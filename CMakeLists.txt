cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

if("${CMAKE_CURRENT_BINARY_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
	message(FATAL_ERROR "In-source builds are blocked. Please build from a separate directory.")
endif()

# Set up CMAKE path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include(CMakeDependentOption)

project(kkdedit LANGUAGES CXX)

##### PACKAGE CONFIGURATION #####

# bitness check
set(SRB2_SYSTEM_BITS 0)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	message(STATUS "Target is 64-bit")
	set(SRB2_SYSTEM_BITS 64)
endif()
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
	message(STATUS "Target is 32-bit")
	set(SRB2_SYSTEM_BITS 32)
endif()
if(${SRB2_SYSTEM_BITS} EQUAL 0)
	message(STATUS "Target bitness is unknown")
endif()

add_subdirectory(nlohmann-json)
#add_subdirectory(fmt)

add_executable(kkdedit)
target_link_libraries(kkdedit PRIVATE nlohmann_json::nlohmann_json)
#target_link_libraries(kkdedit PRIVATE fmt::fmt-header-only)
target_compile_features(kkdedit PRIVATE cxx_std_17)
set_target_properties(kkdedit PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)
target_sources(kkdedit PRIVATE
	"kkdedit.cpp"
	"g_gamedata.h"
)
