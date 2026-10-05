#include "utils.h"

#define MAX_PELICULAS 100 
#define MAX_NOMBRE 60
#define MAX_GENERO 30
#define MINUTOS 60
#define HORAS_DIA 24

typedef struct
{
    int hora;
    int minutos;
} Horario;

typedef struct {
    char titulo[MAX_NOMBRE];
    Horario duracion;     
    char genero[MAX_GENERO];
    Horario horario;      
    bool activo;      
} Pelicula;

void AgregarPelicula(Pelicula catalogo[], int *total_peliculas);
void EliminarPelicula(Pelicula catalogo[], int total_peliculas);

void AgregarPelicula(Pelicula catalogo[], int* total_peliculas) {
    if (*total_peliculas >= MAX_PELICULAS) {
        printf("Error: El catalogo esta lleno.\n");
    } else {
        printf("\n--- AGREGAR NUEVA PELICULA ---\n");
        leerCadena("Titulo de la pelicula: ", catalogo[*total_peliculas].titulo,MAX_NOMBRE);
        catalogo[*total_peliculas].duracion.hora = leerEntero("Duracion (Las hora): ");
        catalogo[*total_peliculas].duracion.minutos = leerEntero("Duracion (los minutos): ");
        leerCadena("Genero: ", catalogo[*total_peliculas].genero, MAX_GENERO);
        catalogo[*total_peliculas].horario.hora = leerEnteroEntre(0, HORAS_DIA, "La hora de la funcion: ");
        catalogo[*total_peliculas].horario.minutos = leerEnteroEntre(0, MINUTOS, "Ingrese los minutos(en caso no tener 00): ");
        catalogo[*total_peliculas].activo = true; 
        PantallaDeEspera("Cargando pelicula...");
        printf(">> Pelicula '%s' agregada con exito!\n", catalogo[*total_peliculas].titulo);
        (*total_peliculas)++; 
    }
}

void EliminarPelicula(Pelicula catalogo[], int total_peliculas) {
    char titulo_buscar[60];
    bool encontrada = false;

    printf("\n--- ELIMINAR PELICULA ---\n");
    if (total_peliculas == 0) {
        printf("El catalogo esta vacio.\n");
    } else {
        leerCadena("Ingrese el titulo de la pelicula que desea eliminar: ", titulo_buscar, MAX_NOMBRE);
        for (int i = 0; i < total_peliculas; i++) {
            if (strcmp(catalogo[i].titulo, titulo_buscar) == 0 && catalogo[i].activo) {
                catalogo[i].activo = false; 
                encontrada = true;
                printf(">> La pelicula '%s' cambio su estado a 'No disponible'.\n", catalogo[i].titulo);
            }
        }
        if (!encontrada) printf("Error: No se encontro la pelicula o ya estaba 'No disponible'.\n");
    }
}

// USO EN EL MAIN 
int main() {
    Pelicula catalogo[MAX_PELICULAS]; 
    int total_peliculas = 0;
    int opcion;

    do {
        printf("\n======= MENU CATALOGO =======\n");
        printf("1. Agregar Pelicula (Create)\n");
        printf("2. Eliminar Pelicula (Delete)\n");
        printf("3. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                AgregarPelicula(catalogo, &total_peliculas);
                break;
            case 2:
                EliminarPelicula(catalogo, total_peliculas);
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