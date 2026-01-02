add_executable(sablona_scitacky main.cpp zlomek.h)

# Zajistit C++23 i pro tuto labku
set_property(TARGET sablona_scitacky PROPERTY CXX_STANDARD 23)
set_property(TARGET sablona_scitacky PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET sablona_scitacky PROPERTY CXX_EXTENSIONS OFF)
