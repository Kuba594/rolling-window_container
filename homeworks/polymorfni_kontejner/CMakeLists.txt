add_executable(polymorfni_kontejner main.cpp AbstValue.h Seznam.h Seznam.cpp)

# Zajistit C++23 i pro tuto labku
set_property(TARGET polymorfni_kontejner PROPERTY CXX_STANDARD 23)
set_property(TARGET polymorfni_kontejner PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET polymorfni_kontejner PROPERTY CXX_EXTENSIONS OFF)
