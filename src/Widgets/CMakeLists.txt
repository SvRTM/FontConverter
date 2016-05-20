add_subdirectory(Tab)
add_subdirectory(TableModel)
add_subdirectory(TableView)

set(SOURCES
    ${SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/characterInfoTableItem.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/characterPropertyWidget.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/listCharactersScene.cpp"
    PARENT_SCOPE
)

set(HEADERS
    ${HEADERS}
    "${CMAKE_CURRENT_SOURCE_DIR}/characterInfoTableItem.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/characterPropertyWidget.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/listCharactersScene.h"
    PARENT_SCOPE
)
