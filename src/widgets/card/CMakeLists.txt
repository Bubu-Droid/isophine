target_sources(isophine
  PRIVATE
    projectcard.cpp
    projectcard.h
    projectcard.ui
)

target_include_directories(isophine
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
