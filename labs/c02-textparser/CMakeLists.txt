add_executable(c02-textparser main.cpp Counting.cpp Counting.h)

# Zajistit C++23 i pro tuto labku
set_property(TARGET c02-textparser PROPERTY CXX_STANDARD 23)
set_property(TARGET c02-textparser PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET c02-textparser PROPERTY CXX_EXTENSIONS OFF)
