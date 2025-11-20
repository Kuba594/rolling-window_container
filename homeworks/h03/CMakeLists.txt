add_executable(h03 main.cpp makro.cpp makro.h)

# Zajistit C++23 i pro tuto labku
set_property(TARGET h03 PROPERTY CXX_STANDARD 23)
set_property(TARGET h03 PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET h03 PROPERTY CXX_EXTENSIONS OFF)
