#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sqlite/sqlite3.h"
#include  "logica/logica.h"


char *err_msg = NULL;
char sql[1024];
sqlite3 *db = NULL;
int opcion;

int main(void) {

    if (abrirConexion(&db) != 0) {
        return 1;
    }


    do {
        printf("\n--- MENU ---\n");
        printf("1 - Cargar .CSV\n");
        printf("2 - Compactar base de datos\n");
        printf("3 - Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                cargarCSV("C:\\Users\\Jurguen Monge\\CLionProjects\\SqlProject\\archivos\\carga.bat");
                break;
            case 2:
                ejecutarSQL(db,"VACUUM;");
                break;
            case 3:
                cerrarConexion(&db);
                break;
            default:
                printf("Opción invalida.\n");
        }
    }while (opcion != 3);

    return 0;
}