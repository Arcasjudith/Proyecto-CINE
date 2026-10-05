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


typedef struct
{
    char titulo[MAX_NOMBRE];
    char genero[MAX_GENERO];
    Horario duracion;
    Horario horario;
    bool activo;
} Pelicula;

// Prototipos de funciones
void AgregarPelicula(Pelicula catalogo[], int *total_peliculas);
void listarPeliculas(Pelicula catalogo[], int total_peliculas);
void BuscarPelicula(Pelicula catalogo[], int total_peliculas);
void ModificarPelicula(Pelicula catalogo[], int *total_peliculas);
void MostrarEncabezado(cadena mensaje);
int MostrarPeliculasActivas(Pelicula catalogo[], int total_peliculas);
void MostrarMenuModificar(Pelicula catalogo[], int indice);
int BuscarIndicePelicula(Pelicula catalogo[], int total_peliculas, char *titulo_buscar);
void MostrarPeliculaEncontrada(Pelicula catalogo[], int indice);
void EjecutarMenuModificar(Pelicula *pelicula, int opcion_campo);
void ModificarTitulo(char *titulo);
void ModificarGenero(char *genero);
void ModificarDuracion(Horario *duracion);
void ModificarHorario(Horario *horario);

// Para probar las funciones
int main()
{
    Pelicula catalogo[MAX_PELICULAS];

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
            AgregarPelicula(catalogo, &total_peliculas);
            break;

        case 2:
            listarPeliculas(catalogo, total_peliculas);
            break;
        case 3:
            BuscarPelicula(catalogo, total_peliculas);
            break;
        case 4:
            ModificarPelicula(catalogo, &total_peliculas);
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
void AgregarPelicula(Pelicula catalogo[], int *total_peliculas)
{
    if (*total_peliculas >= MAX_PELICULAS)
    {
        printf("Error: El catalogo esta lleno.\n");
    }
    else
    {
        printf("\n--- AGREGAR NUEVA PELICULA ---\n");
        leerCadena("Titulo de la pelicula: ", catalogo[*total_peliculas].titulo, MAX_NOMBRE);
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
void listarPeliculas(Pelicula catalogo[], int total_peliculas)
{
    MostrarEncabezado("LISTADO DE PELICULAS");
    MostrarPeliculasActivas(catalogo, total_peliculas);
}

void BuscarPelicula(Pelicula catalogo[], int total_peliculas)
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
        leerCadena("Ingrese el titulo a buscar: ", titulo_buscar, MAX_NOMBRE);
        indice = BuscarIndicePelicula(catalogo, total_peliculas, titulo_buscar);

        if (indice != -1)
        {
            MostrarPeliculaEncontrada(catalogo, indice);
        }
        else
        {
            printf("No se encontro ninguna pelicula activa con ese titulo.\n");
        }
    }
}

void ModificarPelicula(Pelicula catalogo[], int *total_peliculas)
{
    int peliculas_activas = 0;
    int opcion_campo;
    int indice;
    char titulo_buscar[MAX_NOMBRE];
    int rango_min = 1, rango_max = 5;

    if (*total_peliculas == 0)
    {
        printf("\nEl catalogo esta vacio. No hay peliculas para modificar.\n");
    }
    else
    {
        MostrarEncabezado("MODIFICAR PELICULA");
        peliculas_activas = MostrarPeliculasActivas(catalogo, *total_peliculas);

        if (peliculas_activas == 0)
        {
            printf("No hay peliculas activas para modificar.\n");
        }
        else
        {
            leerCadena("Ingrese el Nombre de la pelicula a modificar: ", titulo_buscar, MAX_NOMBRE);
            indice = BuscarIndicePelicula(catalogo, *total_peliculas, titulo_buscar);

            if (indice == -1)
            {
                printf("Error: No se encontro ninguna pelicula activa con el nombre '%s'.\n", titulo_buscar);
            }
            else
            {
                do
                {
                    MostrarMenuModificar(catalogo, indice);
                    opcion_campo = leerEnteroEntre(rango_min, rango_max, "Seleccione una opcion: ");

                    EjecutarMenuModificar(&catalogo[indice], opcion_campo);

                } while (opcion_campo != rango_max);
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

int MostrarPeliculasActivas(Pelicula catalogo[], int total_peliculas)
{
    int peliculas_activas = 0;
    int i;

    for (i = 0; i < total_peliculas; i++)
    {

        if (catalogo[i].activo)
        {

            printf("%-20s %-15s %dh %02dm     %02d:%02d hs\n",
                   catalogo[i].titulo,
                   catalogo[i].genero,
                   catalogo[i].duracion.hora,
                   catalogo[i].duracion.minutos,
                   catalogo[i].horario.hora,
                   catalogo[i].horario.minutos);
            peliculas_activas++;
        }
    }

    return peliculas_activas;
}

void MostrarMenuModificar(Pelicula catalogo[], int indice)
{
    printf("\n--- Modificando: '%s' ---\n", catalogo[indice].titulo);
    printf("1. Modificar Titulo (Actual: %s)\n", catalogo[indice].titulo);
    printf("2. Modificar Genero (Actual: %s)\n", catalogo[indice].genero);
    printf("3. Modificar Duracion (Actual: %dh %02dm)\n", catalogo[indice].duracion.hora, catalogo[indice].duracion.minutos);
    printf("4. Modificar Horario (Actual: %02d:%02d)\n", catalogo[indice].horario.hora, catalogo[indice].horario.minutos);
    printf("5. Volver al menu principal\n");
}

int BuscarIndicePelicula(Pelicula catalogo[], int total_peliculas, char *titulo_buscar)
{
    int posicion = -1;
    int i = 0;
    while (i < total_peliculas && posicion == -1)
    {
        if (catalogo[i].activo && strcasecmp(catalogo[i].titulo, titulo_buscar) == 0)
        {
            posicion = i;
        }
        i++;
    }

    return posicion;
}
void MostrarPeliculaEncontrada(Pelicula catalogo[], int indice)
{
    printf("\n>> PELICULA ENCONTRADA <<\n");
    printf("Titulo:   %s\n", catalogo[indice].titulo);
    printf("Genero:   %s\n", catalogo[indice].genero);
    printf("Duracion: %dh %02dm\n", catalogo[indice].duracion.hora, catalogo[indice].duracion.minutos);
    printf("Horario:  %02d:%02d hs\n", catalogo[indice].horario.hora, catalogo[indice].horario.minutos);
}

void EjecutarMenuModificar(Pelicula *pelicula, int opcion_campo)
{
    switch (opcion_campo)
    {
    case 1:
        ModificarTitulo(pelicula->titulo);
        break;
    case 2:
        ModificarGenero(pelicula->genero);
        break;
    case 3:
        ModificarDuracion(&pelicula->duracion);
        break;
    case 4:
        ModificarHorario(&pelicula->horario);
        break;
    case 5:
        printf("Regresando al menu principal...\n");
        break;
    default:
        printf("Opcion invalida. Intente nuevamente.\n");
        break;
    }
}

void ModificarTitulo(char *titulo)
{
    leerCadena("Nuevo Titulo: ", titulo, MAX_NOMBRE);
    printf(">> Titulo actualizado correctamente.\n");
}
void ModificarGenero(char *genero)
{
    leerCadena("Nuevo Genero: ", genero, MAX_GENERO);
    printf(">> Genero actualizado correctamente.\n");
}
void ModificarDuracion(Horario *duracion)
{
    duracion->hora = leerEntero("Nueva Duracion (Horas): ");
    duracion->minutos = leerEnteroEntre(0, MINUTOS, "Nueva Duracion (Minutos): ");
    printf(">> Duracion actualizada correctamente.\n");
}
void ModificarHorario(Horario *horario)
{
    horario->hora = leerEnteroEntre(0, HORAS_DIA, "Nuevo Horario (Hora 0-23): ");
    horario->minutos = leerEnteroEntre(0, MINUTOS, "Nuevo Horario (Minutos 0-59): ");
    printf(">> Horario actualizado correctamente.\n");
}