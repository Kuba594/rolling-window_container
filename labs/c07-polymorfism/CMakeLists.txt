add_executable(c07-polymorfism main.cpp Seznam.cpp Seznam.h AbstValue.cpp AbstValue.h)

# Zajistit C++23 i pro tuto labku
set_property(TARGET c07-polymorfism PROPERTY CXX_STANDARD 23)
set_property(TARGET c07-polymorfism PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET c07-polymorfism PROPERTY CXX_EXTENSIONS OFF)
