cmake_minimum_required(VERSION 3.15)
project(${PROJECT_NAME})

find_package(LATEX)
if(LATEX_FOUND)
  include(${CMAKE_SOURCE_DIR}/cmake/UseLATEX.cmake)
  add_latex_document(CCGS.tex INPUTS
    actor.png
    CD1.png
    CD2.png
    CD3.png

    header.tex

    AD_Simulation.tex
    StateD.tex
    UC.tex
    AD_Turn.tex
    AD_CardGen.tex
    OD.tex
    CRC.tex
    CD.tex
    CCG_description.tex
    SD2.tex
    SD.tex
  )
endif()
