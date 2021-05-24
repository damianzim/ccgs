#!/bin/bash

find -E . -regex '.\/(src|test)\/.*\.(cc|hpp)' | xargs clang-format -verbose -style=file -i
