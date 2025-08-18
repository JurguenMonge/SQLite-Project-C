//
// Created by Jurguen Monge on 10/8/2025.
//

#ifndef SQLPROJECT_LOGICA_H
#define SQLPROJECT_LOGICA_H
#include <stdio.h>
#include <stdlib.h>
#include "../sqlite/sqlite3.h"

static int mostrar_devolucion(void *NotUsed, int argc, char **argv, char **azColName) {
    for(int i=0; i < argc ; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int ejecutarSQL(sqlite3 *db, const char *sql) {
    char *errMsg = NULL;
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error SQL: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    }

    printf("SQL ejecutado correctamente: %s\n", sql);

    return SQLITE_OK;
}

int abrirConexion(sqlite3 *db) {
    int rc = sqlite3_open("C:/Users/Jurguen Monge/CLionProjects/SqlProject/archivos/DBCongestion.db", db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "No se pudo abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    return 0;
}

void cerrarConexion(sqlite3 *db) {
    if (sqlite3_close(db) == SQLITE_OK) {
        printf("Conexion cerrada exitosamente.\n");
    } else {
        printf("Error al cerrar la conexion.\n");
    }
    db = NULL;
}

void cargarCSV(char *ruta)
{
    char comando[1024];

    snprintf(comando, sizeof(comando), "cmd /c \"%s\"", ruta);

    int resultado = system(comando);

    if (resultado == 0) {
        printf("La carga fue de datos fue exitosa.\n");
    } else {
        printf("Ocurrio un error al ejecutar el archivo .bat (codigo %d).\n", resultado);
    }
}

#endif //SQLPROJECT_LOGICA_H