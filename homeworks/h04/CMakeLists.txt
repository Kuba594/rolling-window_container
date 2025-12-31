add_executable(h04 main.cpp api.cpp api.h Table.cpp Table.h)

# Zajistit C++23 i pro tuto labku
set_property(TARGET h04 PROPERTY CXX_STANDARD 23)
set_property(TARGET h04 PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET h04 PROPERTY CXX_EXTENSIONS OFF)
