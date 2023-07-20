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
	"include/Platform.h"
	"${INCLUDE_DIR}/platformsmap.h"
	"include/MainWindow.h"

	"include/gbemu/Cpu.h"
	"${INCLUDE_DIR}/gbemu/cycletables.h"
	"include/gbemu/Display.h"
	"include/gbemu/IoPorts.h"
	"include/gbemu/MemoryMap.h"
	"include/gbemu/MemoryBankController.h"
	"include/gbemu/System.h"

	"${INCLUDE_DIR}/opengl/opengl.h"

	"${SOURCE_DIR}/main.cpp"
	"src/MainWindow.cpp"
	"src/Platform.cpp"

	"src/gbemu/Cpu.cpp"
	"src/gbemu/Display.cpp"
	"${SOURCE_DIR}/gbemu/instructionset.cpp"
	"${SOURCE_DIR}/gbemu/instructionsetcb.cpp"
	"src/gbemu/IoPorts.cpp"
	"src/gbemu/MemoryMap.cpp"
	"src/gbemu/MemoryBankController.cpp"
	"src/gbemu/System.cpp"

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
