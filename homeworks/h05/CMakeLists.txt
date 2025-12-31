add_executable(h05 main.cpp stable_vector.hpp)

# Zajistit C++23 i pro tuto labku
set_property(TARGET h05 PROPERTY CXX_STANDARD 23)
set_property(TARGET h05 PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET h05 PROPERTY CXX_EXTENSIONS OFF)
