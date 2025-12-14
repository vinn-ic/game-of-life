cmake_minimum_required(VERSION 3.10)
project(GameOfLife C)

# Encontrar o GTK (substitua gtk-3.0 por gtk-4 se usar GTK4)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

# Adicionar executável
add_executable(game-of-life src/main.c)

# Incluir os headers do GTK
target_include_directories(game-of-life PRIVATE ${GTK_INCLUDE_DIRS})

# Linkar as bibliotecas do GTK
target_link_libraries(game-of-life ${GTK_LIBRARIES})

# Adicionar flags extras do GTK
add_definitions(${GTK_CFLAGS_OTHER})

