#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"

#define OCULTO 88
#define CLAVE_ADMIN 1234
#define SALIDA 4
// FARFAN
#define MAX_PELICULAS 100
#define MAX_NOMBRE 60
#define MAX_GENERO 30

void bienvenidos();
void mostrarDespedida();

void primermenu();
// USUARIO
void vercartelera();   // tami
void buscarpelicula(); // tami
void comprabutacas();  // Agus

// ADMIN
// FARFAN
void ejecutarAdmin(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int *total_peliculas);
void menuadministrador();
int verifiadmi();
// FARFAN
void cargarpeli(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int *total_peliculas);
void darbajapeli(char titulos[][MAX_NOMBRE], bool activos[], int total_peliculas);
void modificardatos(); // tami

// FARFAN
void AgregarPelicula(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int *total_peliculas);
void EliminarPelicula(char titulos[][MAX_NOMBRE], bool activos[], int total_peliculas);

void opcioninvalida();

int main()
{

    char titulos[MAX_PELICULAS][MAX_NOMBRE];
    int duraciones[MAX_PELICULAS];
    char generos[MAX_PELICULAS][MAX_GENERO];
    int horarios[MAX_PELICULAS];
    bool activos[MAX_PELICULAS];
    int total_peliculas = 0;

    int op = 0;

    bienvenidos();

    while (op != SALIDA)
    {
        primermenu();
        op = leerEntero("Seleccione una opcion: ");

        switch (op)
        {
        case 1:
            limpiarPantalla();
            vercartelera();
            break;
        case 2:
            limpiarPantalla();
            buscarpelicula();
            break;
        case 3:
            limpiarPantalla();
            comprabutacas();
            break;
        case 4:

            break;
        case OCULTO:
            limpiarPantalla();
            if (verifiadmi() == 1)
            {
                printf("\n--- ACCESO CONCEDIDO ---\n");
                ejecutarAdmin(titulos, duraciones, generos, horarios, activos, &total_peliculas);
            }
            else
            {
                printf("\nClave incorrecta. Acceso denegado.\n");
            }
            break;
        default:
            opcioninvalida();
            break;
        }
    }

    mostrarDespedida();

    return 0;
}

void bienvenidos()
{
    limpiarPantalla();
    printf("\n===================================\n");
    printf("\n             BIENVENIDO            \n");
}

void mostrarDespedida()
{
    limpiarPantalla();
    printf("\n===================================\n");
    printf("\n ¡Gracias por visitar El Milagro del Cine!\n");
    printf("===================================\n");
}

void primermenu()
{
    printf("\n===================================\n");
    printf("        EL MILAGRO DEL CINE        \n");
    printf("===================================\n");
    printf("1. Ver cartelera\n");
    printf("2. Buscar pelicula\n");
    printf("3. Comprar / Reservar asientos\n");
    printf("4. Salir\n");
    printf("-----------------------------------\n");
}

// --- PARTE ADMIN ---

void ejecutarAdmin(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int *total_peliculas)
{
    int opadmin = 0;

    while (opadmin != SALIDA)
    {
        menuadministrador();
        opadmin = leerEntero("Seleccione una opcion: ");

        switch (opadmin)
        {
        case 1:
            limpiarPantalla();
            cargarpeli(titulos, duraciones, generos, horarios, activos, total_peliculas);
            break;
        case 2:
            limpiarPantalla();
            darbajapeli(titulos, activos, *total_peliculas);
            break;
        case 3:
            limpiarPantalla();
            modificardatos();
            break;
        case 4:
            limpiarPantalla();
            printf("\nVolviendo al menu principal...\n");
            break;
        default:
            limpiarPantalla();
            opcioninvalida();
            break;
        }
    }
}

void menuadministrador()
{
    printf("\n===================================\n");
    printf("      PANEL DE ADMINISTRACION      \n");
    printf("===================================\n");
    printf("1. Cargar nueva pelicula \n");
    printf("2. Dar de baja una pelicula \n");
    printf("3. Modificar datos o horarios \n");
    printf("4. Volver al menu principal\n");
    printf("-----------------------------------\n");
}

int verifiadmi()
{
    int clave = 0;
    printf("\nIngrese clave de acceso administrativo: ");
    scanf("%d", &clave);

    return (clave == CLAVE_ADMIN);
}

void opcioninvalida()
{
    printf("\n===================================\n");
    printf(" OPCION INVALIDA, VUELVA A INGRESAR \n");
    printf("===================================\n");
}

void cargarpeli(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int *total_peliculas)
{
    AgregarPelicula(titulos, duraciones, generos, horarios, activos, total_peliculas);
}

void darbajapeli(char titulos[][MAX_NOMBRE], bool activos[], int total_peliculas)
{
    EliminarPelicula(titulos, activos, total_peliculas);
}

void AgregarPelicula(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int *total_peliculas)
{
    if (*total_peliculas >= MAX_PELICULAS)
    {
        printf("Error: El catalogo esta lleno.\n");
    }
    else
    {
        int indice = *total_peliculas;
        printf("\n--- AGREGAR NUEVA PELICULA ---\n");
        printf("Titulo de la pelicula: ");
        LimpiarBuffer();
        fgets(titulos[indice], MAX_NOMBRE, stdin);
        titulos[indice][strcspn(titulos[indice], "\n")] = '\0';
        duraciones[indice] = leerEntero("Duracion (en minutos): ");
        printf("Genero: ");
        scanf(" %s", generos[indice]);
        horarios[indice] = leerEnteroEntre(1000, 9999, "Horario de funcion (Ej. 2030 para las 20:30): ");
        activos[indice] = 1;
        PantallaDeEspera();
        printf(">> Pelicula '%s' agregada con exito!\n", titulos[indice]);
        (*total_peliculas)++;
    }
}

void EliminarPelicula(char titulos[][MAX_NOMBRE], bool activos[], int total_peliculas)
{
    char titulo_buscar[MAX_NOMBRE];
    bool encontrada = 0;

    printf("\n--- ELIMINAR PELICULA ---\n");
    if (total_peliculas == 0)
    {
        printf("El catalogo esta vacio.\n");
    }
    else
    {
        printf("Ingrese el titulo de la pelicula que desea eliminar: ");
        LimpiarBuffer();
        fgets(titulo_buscar, MAX_NOMBRE, stdin);
        titulo_buscar[strcspn(titulo_buscar, "\n")] = '\0';

        for (int i = 0; i < total_peliculas; i++)
        {
            if (strcmp(titulos[i], titulo_buscar) == 0 && activos[i] == 1)
            {
                activos[i] = 0;
                encontrada = 1;
                printf(">> La pelicula '%s' cambio su estado a 'No disponible'.\n", titulos[i]);
            }
        }
        if (!encontrada)
        {
            printf("Error: No se encontro la pelicula o ya estaba 'No disponible'.\n");
        }
    }
}

void vercartelera()
{
}
void buscarpelicula()
{
}
void comprabutacas()
{
}
void modificardatos()
{
}