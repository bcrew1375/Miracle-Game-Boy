# CMakeList.txt : CMake project for Miracle-Game-Boy-CMake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.25)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Miracle-Game-Boy")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(UI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ui)

set(PROJECT_SOURCES
	"${INCLUDE_DIR}/platform.h"
	"${INCLUDE_DIR}/platformsmap.h"
	"${INCLUDE_DIR}/mainwindow.h"

	"${INCLUDE_DIR}/gbemu/cpu.h"
	"${INCLUDE_DIR}/gbemu/cycletables.h"
	"${INCLUDE_DIR}/gbemu/display.h"
	"${INCLUDE_DIR}/gbemu/ioports.h"
	"include/gbemu/MemoryMap.h"
	"${INCLUDE_DIR}/gbemu/memorybankcontroller.h"
	"${INCLUDE_DIR}/gbemu/system.h"

	"${INCLUDE_DIR}/opengl/opengl.h"

	"${SOURCE_DIR}/main.cpp"
	"${SOURCE_DIR}/mainwindow.cpp"
	"${SOURCE_DIR}/platform.cpp"

	"${SOURCE_DIR}/gbemu/cpu.cpp"
	"${SOURCE_DIR}/gbemu/display.cpp"
	"${SOURCE_DIR}/gbemu/instructionset.cpp"
	"${SOURCE_DIR}/gbemu/instructionsetcb.cpp"
	"${SOURCE_DIR}/gbemu/ioports.cpp"
	"src/gbemu/MemoryMap.cpp"
	"${SOURCE_DIR}/gbemu/memorybankcontroller.cpp"
	"${SOURCE_DIR}/gbemu/system.cpp"

	"${SOURCE_DIR}/opengl/opengl.cpp"

	"${UI_DIR}/mainwindow.ui"
)

# Add source to this project's executable.
add_executable(Miracle-Game-Boy ${PROJECT_SOURCES})

set_target_properties(Miracle-Game-Boy PROPERTIES AUTOUIC_SEARCH_PATHS ${UI_DIR})
target_include_directories(Miracle-Game-Boy PRIVATE ${INCLUDE_DIR} ${INCLUDE_DIR}/gbemu ${INCLUDE_DIR}/opengl ${UI_DIR})
target_link_libraries(Miracle-Game-Boy Qt6::Core Qt6::Gui Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets)

if (CMAKE_VERSION VERSION_GREATER 3.25)
  set_property(TARGET Miracle-Game-Boy PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
