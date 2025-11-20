add_executable(h01 main.cpp Counting.cpp Counting.h)

# Zajistit C++23 i pro tuto labku
set_property(TARGET h01 PROPERTY CXX_STANDARD 23)
set_property(TARGET h01 PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET h01 PROPERTY CXX_EXTENSIONS OFF)
