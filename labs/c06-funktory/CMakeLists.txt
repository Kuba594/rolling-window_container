add_executable(c06-funktory main.cpp)

# Zajistit C++23 i pro tuto labku
set_property(TARGET c06-funktory PROPERTY CXX_STANDARD 23)
set_property(TARGET c06-funktory PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET c06-funktory PROPERTY CXX_EXTENSIONS OFF)
