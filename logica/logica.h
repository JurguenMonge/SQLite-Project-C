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

#define MAX_LINE 16384
#define MAX_COLS 100
#define BATCH_SIZE 1000000

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
    return SQLITE_OK;
}

int abrirConexion(sqlite3 **db) {
    int rc = sqlite3_open("C:/Users/Jurguen Monge/Documents/Quiz/Congestion.db", db);
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
    *db = NULL;
}

int cargarCSV(sqlite3 *db, const char *nombreTabla) {
    FILE *archivo = fopen("C:/Users/Jurguen Monge/CLionProjects/SqlProject/archivos/data.csv", "r");
    if (!archivo) {
        perror("Error abriendo el archivo CSV");
        return 1;
    }

    char linea[MAX_LINE];
    if (!fgets(linea, sizeof(linea), archivo)) {
        fprintf(stderr, "Archivo CSV vacío o error leyendo encabezado\n");
        fclose(archivo);
        return 1;
    }

    linea[strcspn(linea, "\r\n")] = 0;

    char *cols[MAX_COLS];
    int nCols = 0;
    char *token = strtok(linea, ",");
    while (token && nCols < MAX_COLS) {
        cols[nCols++] = strdup(token);
        token = strtok(NULL, ",");
    }

    char sqlCreate[8192];
    snprintf(sqlCreate, sizeof(sqlCreate), "CREATE TABLE IF NOT EXISTS %s (", nombreTabla);
    for (int i = 0; i < nCols; i++) {
        char colDef[256];
        snprintf(colDef, sizeof(colDef), "\"%s\" TEXT%s", cols[i], (i < nCols - 1) ? ", " : "");
        strncat(sqlCreate, colDef, sizeof(sqlCreate) - strlen(sqlCreate) - 1);
    }
    strncat(sqlCreate, ");", sizeof(sqlCreate) - strlen(sqlCreate) - 1);

    if (ejecutarSQL(db, sqlCreate) != SQLITE_OK) {
        fclose(archivo);
        for (int i = 0; i < nCols; i++) free(cols[i]);
        return 1;
    }

    char sqlInsert[8192];
    snprintf(sqlInsert, sizeof(sqlInsert), "INSERT INTO %s VALUES (", nombreTabla);
    for (int i = 0; i < nCols; i++) {
        strncat(sqlInsert, "?", sizeof(sqlInsert) - strlen(sqlInsert) - 1);
        if (i < nCols - 1) strncat(sqlInsert, ",", sizeof(sqlInsert) - strlen(sqlInsert) - 1);
    }
    strncat(sqlInsert, ");", sizeof(sqlInsert) - strlen(sqlInsert) - 1);

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparando statement: %s\n", sqlite3_errmsg(db));
        fclose(archivo);
        for (int i = 0; i < nCols; i++) free(cols[i]);
        return 1;
    }

    ejecutarSQL(db, "PRAGMA synchronous = OFF;");
    clock_t inicio = clock();
    int contador = 0;

    ejecutarSQL(db, "BEGIN TRANSACTION;");

    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\r\n")] = 0;

        int i = 0;
        char *campo = strtok(linea, ",");
        while (campo && i < nCols) {
            sqlite3_bind_text(stmt, i + 1, campo, -1, SQLITE_TRANSIENT);
            campo = strtok(NULL, ",");
            i++;
        }

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            fprintf(stderr, "Error insertando fila: %s\n", sqlite3_errmsg(db));
        }
        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
        contador++;

        if (contador % BATCH_SIZE == 0) {
            ejecutarSQL(db, "COMMIT;");
            printf("Insertado (%d filas)\n", contador);
            ejecutarSQL(db, "BEGIN TRANSACTION;");
        }
    }

    ejecutarSQL(db, "COMMIT;");
    clock_t fin = clock();
    double segundos = (double)(fin - inicio) / CLOCKS_PER_SEC;
    printf("Carga completada en %.3f segundos\n", segundos);

    sqlite3_finalize(stmt);
    fclose(archivo);

    // Liberar memoria columnas
    for (int i = 0; i < nCols; i++) {
        free(cols[i]);
    }

    // Guardar tiempo en CSV
    FILE *logFile = fopen("tiempo_carga.csv", "w");
    if (logFile) {
        fprintf(logFile, "TiempoCargaSegundos\n%.6f\n", segundos);
        fclose(logFile);
    }

    return 0;

}

#endif //SQLPROJECT_LOGICA_H