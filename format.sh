#!/bin/bash

find -E . -regex '.\/(src|tests)\/.*\.(cc|hpp)' | xargs clang-format -verbose -style=file -i
