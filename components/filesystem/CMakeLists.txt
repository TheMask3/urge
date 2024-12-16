# content components
cmake_minimum_required(VERSION 3.20.1)

#--------------------------------------------------------------------------------
# components source
#--------------------------------------------------------------------------------

set(COMPONENTS_FILESYSTEM_SRC
  io.cc
  io.h
)

add_library(components_filesystem STATIC ${COMPONENTS_FILESYSTEM_SRC})
urge_setup_target_compile_feature(components_filesystem)
target_link_libraries(components_filesystem PUBLIC
  engine_base
)
