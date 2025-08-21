#include <stdio.h>
#include "sqlite/sqlite3.h"
#include "logica/logica.h"
#include "logica/Constantes.h"

char *err_msg = NULL;
char sql[2048];
sqlite3 *db = NULL;
int opcion;

int main(void) {

    if (abrirConexion(&db) != 0) {
        return 1;
    }


    do {
        printf("\n--- MENU ---\n");
        printf("1 - Cargar .CSV\n");
        printf("2 - Crear vista\n");
        printf("3 - Crear indices\n");
        printf("4 - Ejecutar consulta (sin indice)\n");
        printf("5 - Ejecutar consulta (con indice)\n");
        printf("6 - Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        getchar();


        switch (opcion) {
            case 1:
                cargarCSV(CARGA);
                break;

            case 2:
                int opcionVista;
                printf("1 - Vista de cantidad de eventos\n");
                printf("2 - Vista de cantidad de ubicaciones\n");
                printf("Seleccione una vista: ");
                scanf("%d", &opcionVista);
                getchar();

                if (opcionVista == 1) {
                   abrirArchivo(db,VISTA_CANTIDAD_EVENTOS,sql, sizeof(sql));
                }else if (opcionVista == 2) {
                    abrirArchivo(db,VISTA_CANTIDAD,sql, sizeof(sql));
                }

                break;

            case 3:
                int opcionIndice;
                printf("1 - Crear indice de cantidad de eventos\n");
                printf("2 - Crear indice de cantidad de ubicaciones\n");
                printf("Seleccione un indice: ");
                scanf("%d", &opcionIndice);
                getchar();

                if (opcionIndice == 1) {
                    abrirArchivo(db,INDICE_CANTIDAD_EVENTOS,sql, sizeof(sql));
                }else if (opcionIndice == 2) {
                    abrirArchivo(db,INDICE_CANTIDAD_EVENTOS,sql, sizeof(sql));
                }
                break;

            case 4:
                printf("Ingrese la consulta SQL que desea ejecutar:\n> ");
                fgets(sql, sizeof(sql), stdin);
                sql[strcspn(sql, "\n")] = 0;
                ejecutarVista(db, sql, "no", RESULTADO_VISTA_CANTIDAD_EVENTOS_SIN_INDICE);
                break;

            case 5:
                printf("Ingrese la consulta SQL que desea ejecutar:\n> ");
                fgets(sql, sizeof(sql), stdin);
                sql[strcspn(sql, "\n")] = 0;
                ejecutarVista(db, sql, "si", RESULTADO_VISTA_CANTIDAD_EVENTOS_CON_INDICE);
                break;

            case 6:
                cerrarConexion(&db);
                break;

            default:
                printf("Opción invalida.\n");
        }
    }while (opcion != 6);

    return 0;
}