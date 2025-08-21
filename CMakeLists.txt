cmake_minimum_required(VERSION 4.0)
project(SqlProject C)

set(CMAKE_C_STANDARD 11)

add_executable(SqlProject main.c sqlite/sqlite3.c
        logica/logica.h
        logica/Constantes.h)