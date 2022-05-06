cmake_minimum_required (VERSION 3.11)

project(scm LANGUAGES CXX VERSION 1.0.0)

include(FetchContent)
 
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  # Important: Specify a GIT_TAG XXXXX here.
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(YAML_CPP REQUIRED yaml-cpp)
include_directories(${YAML_CPP_INCLUDEDIR})


FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
 

add_executable(scm src/main.cpp)

target_link_libraries(scm
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component # Not needed for this example.
  ${YAML_CPP_LIBRARIES}
)