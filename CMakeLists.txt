cmake_minimum_required(VERSION 3.10.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
cmake_policy(SET CMP0057 NEW)

# option(BUILD_SHARED_LIBS "" ON)


macro(setup_mod project_name file_name)
  project(${project_name})
  add_library(${project_name} SHARED ${file_name})
  set(CMAKE_SIZEOF_VOID_P 4)

  if (";${ARGN};" MATCHES ";minhook;")
    target_include_directories(${project_name} PRIVATE libraries/minhook/include)
    target_link_libraries(${project_name} minhook)
  endif()
  if (";${ARGN};" MATCHES ";cocos;")
    target_link_libraries(${project_name} cocos2d)
  endif()
  if (";${ARGN};" MATCHES ";gd.h;")
    target_include_directories(${project_name} PRIVATE libraries/gd.h libraries/gd.h/include)
  endif()
  if (";${ARGN};" MATCHES ";nomerge;")
    target_link_options(${project_name} PRIVATE "/OPT:REF,NOICF")
  endif()
  if (";${ARGN};" MATCHES ";fmod;")
    target_link_libraries(${project_name} ${CMAKE_SOURCE_DIR}/libraries/fmod/fmod.lib)
    target_include_directories(${project_name} PRIVATE libraries/fmod)
  endif()
  if (";${ARGN};" MATCHES ";opengl;")
    target_link_libraries(${project_name} opengl32 ${CMAKE_SOURCE_DIR}/libraries/glew32.lib)
  endif()
  set_target_properties(${project_name} PROPERTIES PREFIX "")
endmacro()

setup_mod(more-button src/more-button.cpp
  minhook cocos gd.h nomerge)

setup_mod(level-info src/level-info.cpp
  minhook cocos gd.h nomerge)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
  # ensure 32 bit on clang
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -target i386-pc-windows-msvc")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -target i386-pc-windows-msvc")
  add_definitions("--target=i386-pc-windows-msvc")
endif()

add_subdirectory(libraries/minhook)
add_subdirectory(libraries/cocos-headers)