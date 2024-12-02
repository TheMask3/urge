# UI Library
cmake_minimum_required(VERSION 3.20.1)

#--------------------------------------------------------------------------------
# ui widget
#--------------------------------------------------------------------------------

set(UI_SRC
  widget/widget.cc
  widget/widget.h
)

add_library(engine_ui ${UI_SRC})
urge_setup_target_compile_feature(engine_ui)
target_link_libraries(engine_ui PUBLIC engine_base)
