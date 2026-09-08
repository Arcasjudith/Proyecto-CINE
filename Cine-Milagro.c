#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

#define OCULTO 88
#define CLAVE_ADMIN 1234
#define SALIDA 4
#define MAX_PELICULAS 100
#define MAX_NOMBRE 60
#define MAX_GENERO 30
//Agus
#define FILAS 5
#define COLUMNAS 10
#define BOLETO_MIN 0
#define BOLETO_MAX 50

void bienvenidos();
void mostrarDespedida();
void primermenu();
int solicitarOpcionPrincipal();

void iniciarSistema(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int *total_peliculas);
void procesarOpcionPrincipal(int op, char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int *total_peliculas);
void gestionarAccesoAdmin(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int *total_peliculas);

void vercartelera();
void buscarpelicula();
void comprabutacas();

void ejecutarAdmin(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int *total_peliculas);
void menuadministrador();
int verifiadmi();
void cargarpeli(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int *total_peliculas);
void darbajapeli(char titulos[][MAX_NOMBRE], bool activos[], int total_peliculas);
void modificardatos();

void AgregarPelicula(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int *total_peliculas);
void EliminarPelicula(char titulos[][MAX_NOMBRE], bool activos[], int total_peliculas);
void listarPeliculas(char titulos[][MAX_NOMBRE], char generos[][MAX_GENERO], int duraciones[], int horarios[], bool activos[], int total_peliculas);
void BuscarPelicula(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int total_peliculas);
void ModificarPelicula(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int total_peliculas);

void MostrarEncabezado(cadena mensaje);
int MostrarPeliculasActivas(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int total_peliculas);
void MostrarMenuModificar(char *titulo, char *genero, int duracion, int horario);
int BuscarIndicePelicula(char titulos[][MAX_NOMBRE], bool activos[], int total_peliculas, char *titulo_buscar);
void opcioninvalida();

// Nuevos prototipos de asientos (Agus)
void Tablero_Asientos_Agus(int lugares[][COLUMNAS]);
void Mostrar_Tablero_ASientos(int *lugares, int Indice1, int indice2);
void ASignacion_Asientos(int *lugares);
int contarAsientosDisponibles(int *lugares);

// Variables Globales Originales
char titulos_global[MAX_PELICULAS][MAX_NOMBRE];
int duraciones_global[MAX_PELICULAS];
char generos_global[MAX_PELICULAS][MAX_GENERO];
int horarios_global[MAX_PELICULAS];
bool activos_global[MAX_PELICULAS];
int total_peliculas_global = 0;

// Variables Globales Nuevas (Asientos por película)
int asientos_global[MAX_PELICULAS][FILAS][COLUMNAS];
bool asientos_inicializados = false;

int main()
{
    bienvenidos();
    iniciarSistema(titulos_global, duraciones_global, generos_global, horarios_global, activos_global, &total_peliculas_global);
    mostrarDespedida();

    return 0;
}

void iniciarSistema(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int *total_peliculas)
{
    int op = 0;
    while (op != SALIDA)
    {
        op = solicitarOpcionPrincipal();
        procesarOpcionPrincipal(op, titulos, duraciones, generos, horarios, activos, total_peliculas);
    }
}

int solicitarOpcionPrincipal()
{
    primermenu();
    return leerEntero("Seleccione una opcion: ");
}

void procesarOpcionPrincipal(int op, char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int *total_peliculas)
{
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
        gestionarAccesoAdmin(titulos, duraciones, generos, horarios, activos, total_peliculas);
        break;
    default:
        opcioninvalida();
        break;
    }
}

void gestionarAccesoAdmin(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int *total_peliculas)
{
    limpiarPantalla();
    if (verifiadmi() == 1)
    {
        printf("\n--- ACCESO CONCEDIDO ---\n");
        ejecutarAdmin(titulos, duraciones, generos, horarios, activos, total_peliculas);
    }
    else
    {
        printf("\nClave incorrecta. Acceso denegado.\n");
    }
}

void bienvenidos()
{
    limpiarPantalla();
    printf("\n===================================\n");
    printf("\n            BIENVENIDO            \n");
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

void opcioninvalida()
{
    printf("\n===================================\n");
    printf(" OPCION INVALIDA, VUELVA A INGRESAR \n");
    printf("===================================\n");
}

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

void cargarpeli(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int *total_peliculas)
{
    AgregarPelicula(titulos, duraciones, generos, horarios, activos, total_peliculas);
}

void darbajapeli(char titulos[][MAX_NOMBRE], bool activos[], int total_peliculas)
{
    EliminarPelicula(titulos, activos, total_peliculas);
}

void vercartelera()
{
    listarPeliculas(titulos_global, generos_global, duraciones_global, horarios_global, activos_global, total_peliculas_global);
}

void buscarpelicula()
{
    BuscarPelicula(titulos_global, duraciones_global, generos_global, horarios_global, activos_global, total_peliculas_global);
}

void modificardatos()
{
    ModificarPelicula(titulos_global, duraciones_global, generos_global, horarios_global, activos_global, total_peliculas_global);
}

// =========================================================================
// NUEVA FUNCION COMPRA BUTACAS (Integracion)
// =========================================================================
void comprabutacas()
{
    // 1. Inicializar todas las matrices de asientos en el primer uso
    if (!asientos_inicializados) {
        for (int i = 0; i < MAX_PELICULAS; i++) {
            Tablero_Asientos_Agus(asientos_global[i]);
        }
        asientos_inicializados = true;
    }

    int id_seleccionado = -1;
    bool sistema_activo = true;

    while (sistema_activo) {
        limpiarPantalla();
        printf("\n=================================================================\n");
        printf("                CARTELERA ACTUAL - COMPRA DE BUTACAS\n");
        printf("=================================================================\n");
        printf("%-5s %-20s %-15s %-10s %-10s\n", "ID", "Titulo", "Genero", "Duracion", "Horario");
        printf("-----------------------------------------------------------------\n");
        
        int peliculas_activas = 0;
        for (int i = 0; i < total_peliculas_global; i++)
        {
            if (activos_global[i])
            {
                int horas = horarios_global[i] / 100;
                int minutos = horarios_global[i] % 100;
                int horas_d = duraciones_global[i] / 60;
                int minutos_d = duraciones_global[i] % 60;
                // Listado con ID para seleccion
                printf("[%d]   %-20s %-15s %dh %02dm     %02d:%02d hs\n",
                       i + 1, titulos_global[i], generos_global[i], horas_d, minutos_d, horas, minutos);
                peliculas_activas++;
            }
        }

        if (peliculas_activas == 0) {
            printf("\nNo hay peliculas disponibles en este momento.\n");
            printf("\nPresione una tecla para continuar...\n");
            LimpiarBuffer();
            sistema_activo = false; 
        } 
        else {
            id_seleccionado = leerEnteroEntre(0, total_peliculas_global, "\nIngrese el ID de la pelicula (0 para salir): ");

            if (id_seleccionado == 0) {
                sistema_activo = false;
            } 
            else {
                int index_peli = id_seleccionado - 1;

                if (!activos_global[index_peli]) {
                    printf("\nError: Pelicula no activa o ID invalido.\n");
                    LimpiarBuffer();
                } 
                else {
                    int Can_Asientos = 1;
                    while (Can_Asientos != 0)
                    {
                        limpiarPantalla();
                        printf("\t================================================\n");
                        printf("\t         ASIENTOS - %s\n", titulos_global[index_peli]);
                        printf("\t================================================\n");
                        printf("| Ingrese |0| para salir || '0' => Asiento ocupado |\n\n");
                        
                        printf("Cantidad de asientos Libres => |%d|\n", contarAsientosDisponibles((int *)asientos_global[index_peli]));
                        Mostrar_Tablero_ASientos((int *)asientos_global[index_peli], FILAS, COLUMNAS);
                        
                        Can_Asientos = leerEnteroEntre(BOLETO_MIN, BOLETO_MAX, "\nIngrese la cantidad de asientos a comprar (0 para volver a la cartelera): ");
                        
                        for (int i = 0; i < Can_Asientos; i++)
                        {
                            limpiarPantalla();
                            printf("\nComprando entrada %d de %d...\n\n", i + 1, Can_Asientos);
                            Mostrar_Tablero_ASientos((int *)asientos_global[index_peli], FILAS, COLUMNAS);
                            ASignacion_Asientos((int *)asientos_global[index_peli]);
                        }
                    }
                }
            }
        }
    }
}

// =========================================================================
// NUEVAS FUNCIONES DE ASIENTOS Y TABLERO
// =========================================================================

void Tablero_Asientos_Agus(int lugares[][COLUMNAS])
{
    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            lugares[i][j] = (i * COLUMNAS) + j + 1;
        }
    }
}

void Mostrar_Tablero_ASientos(int *lugares, int Indice1, int indice2)
{
    for (int i = 0; i < Indice1; i++)
    {
        for (int j = 0; j < indice2; j++)
        {
            printf("[%2d] ", *(lugares + (i * indice2) + j));
        }
        printf("\n");
    }
}

void ASignacion_Asientos(int *lugares)
{
    int op;    
    op = leerEnteroEntre(BOLETO_MIN, BOLETO_MAX, "Ingrese el numero de asiento: ");
    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            if (op == *(lugares + (i * COLUMNAS) + j))
            {
                *(lugares + (i * COLUMNAS) + j) = 0; 
            }
        }
    }
}

int contarAsientosDisponibles(int *lugares) {
    int contador = 0;
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (*(lugares + (i * COLUMNAS) + j) != 0 ) {
                contador++;
            }
        }
    }
    return contador;
}


// =========================================================================
// FUNCIONES RESTANTES INTACTAS DEL SISTEMA ORIGINAL
// =========================================================================

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
            printf("Duracion: %dh %02dm\n", duraciones[indice] / 60, duraciones[indice] % 60);
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
    int indice;
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

void MostrarEncabezado(cadena mensaje)
{
    printf("\n=======================================================\n");
    printf("                %-35s\n", mensaje);
    printf("=======================================================\n");
    printf("%-20s %-15s %-10s %-10s\n", "Titulo", "Genero", "Duracion", "Horario");
    printf("-------------------------------------------------------\n");
}

int MostrarPeliculasActivas(char titulos[][MAX_NOMBRE], int duraciones[], char generos[][MAX_GENERO], int horarios[], bool activos[], int total_peliculas)
{
    int peliculas_activas = 0;
    int i, horas, minutos, horas_d, minutos_d;

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
    printf("3. Modificar Duracion (Actual: %dh %02dm)\n", duracion / 60, duracion % 60);
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