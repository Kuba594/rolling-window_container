add_executable(c04-slovnik main.cpp slovnik.cpp slovnik.h aplikace.cpp aplikace.h)

# Zajistit C++23 i pro tuto labku
set_property(TARGET c04-slovnik PROPERTY CXX_STANDARD 23)
set_property(TARGET c04-slovnik PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET c04-slovnik PROPERTY CXX_EXTENSIONS OFF)
