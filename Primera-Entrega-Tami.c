#include "utils.h"
#define MAX_PELICULAS 100
#define MAX_NOMBRE 60
#define MAX_GENERO 30

// Prototipos de funciones
void AgregarPelicula(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int *total_peliculas);
void listarPeliculas(char titulos[][MAX_NOMBRE], char generos[][MAX_GENERO], int duraciones[], int horarios[], bool activos[], int total_peliculas);
void BuscarPelicula(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int total_peliculas);
void ModificarPelicula(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int total_peliculas);
void MostrarEncabezado(cadena mensaje);
int MostrarPeliculasActivas(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int total_peliculas);
void MostrarMenuModificar(char *titulo, char *genero, int duracion, int horario);
int BuscarIndicePelicula(char titulos[][MAX_NOMBRE], bool activos[], int total_peliculas, char *titulo_buscar);

// Para probar las funciones
int main()
{
    char titulos[MAX_PELICULAS][MAX_NOMBRE];
    int duraciones[MAX_PELICULAS];
    char generos[MAX_PELICULAS][MAX_GENERO];
    int horarios[MAX_PELICULAS];
    bool activos[MAX_PELICULAS];

    int total_peliculas = 0;
    int opcion;

    do
    {
        printf("\n======= MENU CATALOGO =======\n");
        printf("1. Agregar Pelicula (Create)\n");
        printf("2. Ver lista de peliculas disponibles (Delete)\n");
        printf("3. Buscar Pelicula (Read)\n");
        printf("4. Modificar Pelicula (Update)\n");
        printf("5. Salir\n");
        opcion = leerEnteroEntre(1, 5, "Selecione una opcion: ");
        switch (opcion)
        {
        case 1:
            AgregarPelicula(titulos, duraciones, generos, horarios, activos, &total_peliculas);
            break;

        case 2:
            listarPeliculas(titulos, generos, duraciones, horarios, activos, total_peliculas);
            break;
        case 3:
            BuscarPelicula(titulos, duraciones, generos, horarios, activos, total_peliculas);
            break;
        case 4:
            ModificarPelicula(titulos, duraciones, generos, horarios, activos, total_peliculas);
            break;
        case 5:
            printf("Saliendo del programa...\n");
            break;
        default:
            printf("Opcion invalida.\n");
        }
    } while (opcion != 5);

    return 0;
}

// Del codigo de Farfan
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
        PantallaDeEspera("Cargando...");
        printf(">> Pelicula '%s' agregada con exito!\n", titulos[indice]);
        (*total_peliculas)++;
    }
}

void listarPeliculas(char titulos[][MAX_NOMBRE], char generos[][MAX_GENERO], int duraciones[], int horarios[], bool activos[], int total_peliculas)
{
    MostrarEncabezado("LISTADO DE PELICULAS");
    MostrarPeliculasActivas(titulos, duraciones, generos, horarios, activos, total_peliculas);
}


void BuscarPelicula(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int total_peliculas)
{
    char titulo_buscar[MAX_NOMBRE];
    int indice = -1;

    printf("\n--- BUSCAR PELICULA ---\n");
    if (total_peliculas == 0)
    {
        printf("El catalogo esta vacio.\n");
    }
    else
    {
        printf("Ingrese el titulo a buscar: ");
        LimpiarBuffer();
        fgets(titulo_buscar, MAX_NOMBRE, stdin);
        titulo_buscar[strcspn(titulo_buscar, "\n")] = '\0';

        indice = BuscarIndicePelicula(titulos, activos, total_peliculas, titulo_buscar);

        if (indice != -1)
        {
            printf("\n>> PELICULA ENCONTRADA <<\n");
            printf("Titulo:   %s\n", titulos[indice]);
            printf("Genero:   %s\n", generos[indice]);
            printf("Duracion: %dh %02dm\n", duraciones[indice]/60, duraciones[indice]%60);
            printf("Horario:  %02d:%02d hs\n", horarios[indice] / 100, horarios[indice] % 100);
        }
        else
        {
            printf("No se encontro ninguna pelicula activa con ese titulo.\n");
        }
    }
}


void ModificarPelicula(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int total_peliculas)
{
    int peliculas_activas = 0;
    int opcion_campo;
    int indice ;
    char titulo_buscar[MAX_NOMBRE];

    if (total_peliculas == 0)
    {
        printf("\nEl catalogo esta vacio. No hay peliculas para modificar.\n");
    }
    else
    {
        MostrarEncabezado("MODIFICAR PELICULA");
        peliculas_activas = MostrarPeliculasActivas(titulos, duraciones, generos, horarios, activos, total_peliculas);

        if (peliculas_activas == 0)
        {
            printf("No hay peliculas activas para modificar.\n");
        }
        else
        {
            printf("\nIngrese el Nombre de la pelicula a modificar: ");
            LimpiarBuffer();
            fgets(titulo_buscar, MAX_NOMBRE, stdin);
            titulo_buscar[strcspn(titulo_buscar, "\n")] = '\0';

            indice = BuscarIndicePelicula(titulos, activos, total_peliculas, titulo_buscar);

            if (indice == -1)
            {
                printf("Error: No se encontro ninguna pelicula activa con el nombre '%s'.\n", titulo_buscar);
            }
            else
            {
                do
                {
                    MostrarMenuModificar(titulos[indice], generos[indice], duraciones[indice], horarios[indice]);
                    opcion_campo = leerEnteroEntre(1, 6, "Seleccione una opcion: ");

                    switch (opcion_campo)
                    {
                    case 1:
                        printf("Nuevo Titulo: ");
                        LimpiarBuffer();
                        fgets(titulos[indice], MAX_NOMBRE, stdin);
                        titulos[indice][strcspn(titulos[indice], "\n")] = '\0';
                        printf(">> Titulo actualizado correctamente.\n");
                        break;

                    case 2:
                        printf("Nuevo Genero: ");
                        scanf(" %s", generos[indice]);
                        printf(">> Genero actualizado correctamente.\n");
                        break;

                    case 3:
                        duraciones[indice] = leerEntero("Nueva Duracion (en minutos): ");
                        printf(">> Duracion actualizada correctamente.\n");
                        break;

                    case 4:
                        horarios[indice] = leerEnteroEntre(1000, 9999, "Nuevo Horario (Ej. 2030 para las 20:30): ");
                        printf(">> Horario actualizado correctamente.\n");
                        break;

                    case 5:
                        printf("\n--- Ingrese los nuevos datos completos ---\n");
                        printf("Nuevo Titulo: ");
                        LimpiarBuffer();
                        fgets(titulos[indice], MAX_NOMBRE, stdin);
                        titulos[indice][strcspn(titulos[indice], "\n")] = '\0';

                        duraciones[indice] = leerEntero("Nueva Duracion (en minutos): ");
                        printf("Nuevo Genero: ");
                        scanf(" %s", generos[indice]);
                        horarios[indice] = leerEnteroEntre(1000, 9999, "Nuevo Horario: ");

                        printf(">> Todos los datos fueron actualizados con exito.\n");
                        break;

                    case 6:
                        printf("Regresando al menu principal...\n");
                        break;
                    }
                } while (opcion_campo != 6);
            }
        }
    }
}

// Funciones para modularizar
void MostrarEncabezado(cadena mensaje)
{
    printf("\n=======================================================\n");
    printf("                  %-35s\n", mensaje);
    printf("=======================================================\n");

    printf("%-20s %-15s %-10s %-10s\n", "Titulo", "Genero", "Duracion", "Horario");
    printf("-------------------------------------------------------\n");
}

int MostrarPeliculasActivas(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int total_peliculas)
{
    int peliculas_activas = 0;
    int i, horas, minutos,horas_d, minutos_d;

    for (i = 0; i < total_peliculas; i++)
    {

        if (activos[i])
        {
            horas = horarios[i] / 100;
            minutos = horarios[i] % 100;

            horas_d = duraciones[i] / 60;
            minutos_d = duraciones[i] % 60;
            printf("%-20s %-15s %dh %02dm     %02d:%02d hs\n",
                   titulos[i],
                   generos[i],
                   horas_d,
                   minutos_d,
                   horas,
                   minutos);
            peliculas_activas++;
        }
    }

    return peliculas_activas;
}

void MostrarMenuModificar(char *titulo, char *genero, int duracion, int horario)
{
    printf("\n--- Modificando: '%s' ---\n", titulo);
    printf("1. Modificar Titulo (Actual: %s)\n", titulo);
    printf("2. Modificar Genero (Actual: %s)\n", genero);
    printf("3. Modificar Duracion (Actual: %dh %02dm)\n", duracion/60, duracion%60);
    printf("4. Modificar Horario (Actual: %d)\n", horario);
    printf("5. Modificar Todos los datos\n");
    printf("6. Volver al menu principal\n");
}

int BuscarIndicePelicula(char titulos[][MAX_NOMBRE], bool activos[], int total_peliculas, char *titulo_buscar)
{
    int posicion = -1;
    int i = 0;
    while (i < total_peliculas && posicion == -1)
    {
        if (activos[i] && strcasecmp(titulos[i], titulo_buscar) == 0)
        {
            posicion = i;
        }
        i++;
    }

    return posicion;
}