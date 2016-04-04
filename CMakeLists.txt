cmake_minimum_required (VERSION 3.2)

project(FontConverter)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_VERBOSE_MAKEFILE OFF)

if (MSVC)
    add_definitions(/W4)
elseif (CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-Wall -pedantic)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -mtune=native")

    if (CMAKE_BUILD_TYPE MATCHES Release)
        set(CMAKE_EXE_LINKER_FLAGS "-s")
    endif ()
else ()
    message("Unknown compiler")
endif ()



set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5 REQUIRED COMPONENTS Widgets Core)


include_directories(${Qt5Widgets_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})



set(HEADERS)
set(SOURCES)
set(UIs)


include_directories("src")
add_subdirectory(src)

source_group("Header Files" FILES ${HEADERS})
source_group("Source Files" FILES ${SOURCES})
source_group("UI Files"     FILES ${UIs})

QT5_WRAP_UI(UI_HEADERS ${UIs})


add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${UI_HEADERS})
target_link_libraries(${PROJECT_NAME} ${Qt5Widgets_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_d")
