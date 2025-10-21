add_executable(c03-frequencies main.cpp frequencies.cpp frequencies.h)

# Zajistit C++23 i pro tuto labku
set_property(TARGET c03-frequencies PROPERTY CXX_STANDARD 23)
set_property(TARGET c03-frequencies PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET c03-frequencies PROPERTY CXX_EXTENSIONS OFF)
