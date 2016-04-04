add_subdirectory(Dialogs)
add_subdirectory(font)
add_subdirectory(UI)
add_subdirectory(Widgets)

set(SOURCES
    ${SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp"
    PARENT_SCOPE
)

set(HEADERS
    ${HEADERS}
    "${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h"
    PARENT_SCOPE
)

set (UIs ${UIs} PARENT_SCOPE)
