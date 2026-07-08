target_sources(isophine
  PRIVATE
    buttondialogs.cpp
    buttondialogs.h
    messageboxes.cpp
    messageboxes.h
    newprojectdialog.cpp
    newprojectdialog.h
    newprojectdialog.ui
    openprojectdialog.cpp
    openprojectdialog.h
    openprojectdialog.ui
)

target_include_directories(isophine
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
