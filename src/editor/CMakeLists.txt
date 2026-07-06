target_sources(isophine
  PRIVATE
    isophine_editor.cpp
    isophine_editor.h
    isophine_editor.ui
    pageviewer.cpp
    pageviewer.h
    project_settings.h
)

target_include_directories(isophine
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
