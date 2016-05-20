set(HEADERS ${HEADERS} PARENT_SCOPE)
set(SOURCES ${SOURCES} PARENT_SCOPE)

set(UIs 
    ${UIs}
    "${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.ui"
    "${CMAKE_CURRENT_SOURCE_DIR}/characterPropertyWidget.ui"
    "${CMAKE_CURRENT_SOURCE_DIR}/Dialogs/exportDialog.ui"
    "${CMAKE_CURRENT_SOURCE_DIR}/Dialogs/listCharactersDialog.ui"
    "${CMAKE_CURRENT_SOURCE_DIR}/Tab/fixedCharactersPage.ui"
    "${CMAKE_CURRENT_SOURCE_DIR}/Tab/selectedCharactersPage.ui"
    PARENT_SCOPE 
)
