//
// Created by Jurguen Monge on 10/8/2025.
//

#ifndef SQLPROJECT_LOGICA_H
#define SQLPROJECT_LOGICA_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../sqlite/sqlite3.h"

#define RUTA_CSV "C:/Users/Jurguen Monge/CLionProjects/SqlProject/archivos/resultados.csv"


int callbackGuardarEnArchivo(void *archivo, int argc, char **argv, char **azColName) {
    FILE *f = (FILE *)archivo;

    for (int i = 0; i < argc; i++) {
        fprintf(f, "%s=%s", azColName[i], argv[i] ? argv[i] : "NULL");
        if (i < argc - 1) fprintf(f, ", ");
    }
    fprintf(f, "\n");
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

int abrirConexion(sqlite3 **db) {
    int rc = sqlite3_open("C:/Users/Jurguen Monge/CLionProjects/SqlProject/archivos/DBCongestion.db", db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "No se pudo abrir la base de datos: %s\n", sqlite3_errmsg(*db));
        return 1;
    }
    return 0;
}

void cerrarConexion(sqlite3 **db) {
    if (sqlite3_close(*db) == SQLITE_OK) {
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

void guardarTiempos(const char *consulta, double tiempo, const char *indice) {
    FILE *f = fopen(RUTA_CSV, "r");
    int existe = (f != NULL);
    if (f) fclose(f);

    f = fopen(RUTA_CSV, "a");
    if (!existe) {
        fprintf(f, "consulta,tiempo,indice\n");
    }
    fprintf(f, "\"%s\",%.6f,%s\n", consulta, tiempo, indice);
    fclose(f);
}

void ejecutarVista(sqlite3 *db, const char *consulta, const char *usoIndice, const char *rutaArchivo) {
    clock_t inicio = clock();

    FILE *f = fopen(rutaArchivo, "w");
    if (!f) {
        fprintf(stderr, "No se pudo abrir el archivo para guardar resultados.\n");
        return;
    }

    char *errMsg = NULL;
    int rc = sqlite3_exec(db, consulta, callbackGuardarEnArchivo, f, &errMsg);

    clock_t fin = clock();
    double tiempo = ((double)(fin - inicio)) / CLOCKS_PER_SEC;

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("Consulta ejecutada correctamente.\n");
        printf("Tiempo de ejecucion: %.6f segundos\n", tiempo);
        guardarTiempos(consulta, tiempo, usoIndice);
    }

    fclose(f);
}

void abrirArchivo(sqlite3 *db, char *ruta, char *sql, size_t tam_sql) {
    FILE *archivo = fopen(ruta, "r");
    if (archivo) {
        size_t leidos = fread(sql, 1, tam_sql - 1, archivo);
        sql[leidos] = '\0';
        fclose(archivo);
        ejecutarSQL(db, sql);
    } else {
        fprintf(stderr,"\nError al abrir el archivo\n");
    }
}

#endif //SQLPROJECT_LOGICA_H