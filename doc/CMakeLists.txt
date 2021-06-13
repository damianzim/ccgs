cmake_minimum_required(VERSION 3.15)
project(${PROJECT_NAME})

find_package(LATEX)
if(LATEX_FOUND)
  include(${CMAKE_SOURCE_DIR}/cmake/UseLATEX.cmake)
  add_latex_document(CCGS.tex INPUTS header.tex)
endif()
