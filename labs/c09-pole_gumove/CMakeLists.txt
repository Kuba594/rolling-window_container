add_executable(c09-pole_gumove main.cpp)

# Zajistit C++23 i pro tuto labku
set_property(TARGET c09-pole_gumove PROPERTY CXX_STANDARD 23)
set_property(TARGET c09-pole_gumove PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET c09-pole_gumove PROPERTY CXX_EXTENSIONS OFF)
