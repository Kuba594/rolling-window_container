add_executable(h02 main.cpp slovnik.cpp slovnik.h aplikace.cpp aplikace.h)

# Zajistit C++23 i pro tuto labku
set_property(TARGET h02 PROPERTY CXX_STANDARD 23)
set_property(TARGET h02 PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET h02 PROPERTY CXX_EXTENSIONS OFF)
