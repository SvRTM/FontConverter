set(HEADERS ${HEADERS} PARENT_SCOPE)
set(SOURCES ${SOURCES} PARENT_SCOPE)

set(UIs 
    ${UIs}
    "${CMAKE_CURRENT_SOURCE_DIR}/export.ui"
    "${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.ui"
    "${CMAKE_CURRENT_SOURCE_DIR}/symbolwidget.ui"
    PARENT_SCOPE 
)
