add_executable(c08-sablony main.cpp zlomek.h)

# Zajistit C++23 i pro tuto labku
set_property(TARGET c08-sablony PROPERTY CXX_STANDARD 23)
set_property(TARGET c08-sablony PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET c08-sablony PROPERTY CXX_EXTENSIONS OFF)
