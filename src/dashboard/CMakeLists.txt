target_sources(isophine
  PRIVATE
    dashboard.cpp
    dashboard.h
    dashboard.ui
)

target_include_directories(isophine
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
