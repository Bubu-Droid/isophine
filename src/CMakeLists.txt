qt_add_executable(isophine
  main.cpp
)

target_link_libraries(isophine
  PRIVATE
    Qt::Core
    Qt::Widgets
    Qt::Pdf
)

set_target_properties(isophine PROPERTIES
  WIN32_EXECUTABLE ON
  MACOSX_BUNDLE ON
)

target_include_directories(isophine
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

# add_subdirectory(editor)
add_subdirectory(widgets)
add_subdirectory(dashboard)
