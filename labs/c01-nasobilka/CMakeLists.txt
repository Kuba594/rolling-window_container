add_executable(c01-nasobilka main.cpp)

# Zajistit C++23 i pro tuto labku
set_property(TARGET c01-nasobilka PROPERTY CXX_STANDARD 23)
set_property(TARGET c01-nasobilka PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET c01-nasobilka PROPERTY CXX_EXTENSIONS OFF)
