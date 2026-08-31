#include "utils.h"
#define MAX_PELICULAS 100 
#define MAX_NOMBRE 60
#define MAX_GENERO 30

// Prototipos 
void AgregarPelicula(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int* total_peliculas);
void EliminarPelicula(char titulos[][MAX_NOMBRE], bool activos[], int total_peliculas);


void AgregarPelicula(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int* total_peliculas) {
    if (*total_peliculas >= MAX_PELICULAS) {
        printf("Error: El catalogo esta lleno.\n");
    } else {
        int indice = *total_peliculas;
        printf("\n--- AGREGAR NUEVA PELICULA ---\n");
        printf("Titulo de la pelicula: ");
        LimpiarBuffer(); 
        fgets(titulos[indice], MAX_NOMBRE, stdin);
        // Eliminar el salto de línea '\n' que fgets guarda al presionar Enter
        titulos[indice][strcspn(titulos[indice], "\n")] = '\0';
        duraciones[indice] = leerEntero("Duracion (en minutos)");
        printf("Genero: ");
        scanf(" %s", generos[indice]);
        horarios[indice] = leerEnteroEntre(1000,9999,"Horario de funcion (Ej. 2030 para las 20:30): ");
        activos[indice] = 1; 
        PantallaDeEspera();
        printf(">> Pelicula '%s' agregada con exito!\n", titulos[indice]);
        (*total_peliculas)++; 
    }
}
void EliminarPelicula(char titulos[][MAX_NOMBRE], bool activos[], int total_peliculas) {
    char titulo_buscar[MAX_NOMBRE];
    bool encontrada = 0;

    printf("\n--- ELIMINAR PELICULA ---\n");
    if (total_peliculas == 0) {
        printf("El catalogo esta vacio.\n");
    } else {
        printf("Ingrese el titulo de la pelicula que desea eliminar: ");
        LimpiarBuffer();
        fgets(titulo_buscar, MAX_NOMBRE, stdin);
        titulo_buscar[strcspn(titulo_buscar, "\n")] = '\0'; // Limpiar newline
        
        for (int i = 0; i < total_peliculas; i++) {
            if (strcmp(titulos[i], titulo_buscar) == 0 && activos[i] == 1) {
                activos[i] = 0; 
                encontrada = 1;
                printf(">> La pelicula '%s' cambio su estado a 'No disponible'.\n", titulos[i]);
            }
        }
        if (!encontrada) {
            printf("Error: No se encontro la pelicula o ya estaba 'No disponible'.\n");
        }
    }
}





// referencia de uso en el main

int main() {
    char titulos[MAX_PELICULAS][MAX_NOMBRE];
    int duraciones[MAX_PELICULAS];
    char generos[MAX_PELICULAS][MAX_GENERO];
    int horarios[MAX_PELICULAS];
    bool activos[MAX_PELICULAS];

    int total_peliculas = 0;
    int opcion;

    do {
        printf("\n======= MENU CATALOGO =======\n");
        printf("1. Agregar Pelicula (Create)\n");
        printf("2. Eliminar Pelicula (Delete)\n");
        printf("3. Salir\n");
        opcion = leerEnteroEntre(1,3,"Selecione una opcion: ");
        switch (opcion) {
            case 1:
                AgregarPelicula(titulos, duraciones, generos, horarios, activos, &total_peliculas);
                break;
            case 2:
                EliminarPelicula(titulos, activos, total_peliculas);
                break;
            case 3:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while (opcion != 3);

    return 0;
}