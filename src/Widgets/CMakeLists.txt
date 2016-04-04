add_subdirectory(TableModel)
add_subdirectory(TableView)

set(SOURCES
    ${SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/symboltableitem.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/symbolwidget.cpp"
    PARENT_SCOPE
)

set(HEADERS
    ${HEADERS}
    "${CMAKE_CURRENT_SOURCE_DIR}/symboltableitem.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/symbolwidget.h"
    PARENT_SCOPE
)
