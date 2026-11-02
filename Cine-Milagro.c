#include "utils.h"

#define OPCION_BUSCAR 1
#define OPCION_COMPRAR 2
#define OPADMI_ELMINAR 2
#define OPADMI_MODIFICAR 3
#define OPADMI_REPORTE 4
#define OPADMI_AGREGAR 1
#define OPMODIFICARTITULO 1
#define OPMODIFICARGENERO 2
#define OPMODIFICARDURACION 3
#define OPMODIFICARHORARIO 4
#define INICIALIZACION_INDICE -1
#define ADMINISTRACION 88
#define CLAVE_ADMIN 1234
#define OPCION_SALIDA 3
#define MAX_PELICULAS 100
#define MAX_NOMBRE 60
#define SALIDAADMI 5
#define MAX_GENERO 50
#define MINUTOS 59
#define HORAS_DIA 23
#define MAX_HORA_PELICULA 3
#define MIN_HORA_PELICULA 1
#define FILAS 5
#define COLUMNAS 10
#define TOTAL_ASIENTOS (FILAS * COLUMNAS)
#define BOLETO_MIN 1
#define BOLETO_MAX TOTAL_ASIENTOS
#define PRECIO_PREMIUM 5000.0
#define PRECIO_MEDIA 3500.0
#define PRECIO_GENERAL 2000.0

// Definiciones de color unificadas para interfaz
#define COLOR_RESERVADO (Color){220, 20, 60}
#define COLOR_PREMIUM   (Color){46, 204, 113}
#define COLOR_MEDIA     (Color){241, 196, 15}
#define COLOR_GENERAL   (Color){52, 152, 219}

#ifdef _WIN32
void habilitarModoANSI()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE)
    {
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= 0x0004;
        SetConsoleMode(hOut, dwMode);
    }
}
#else
void habilitarModoANSI()
{
}
#endif

typedef struct
{
    int hora;
    int minutos;
} Horario;

typedef struct
{
    int r;
    int g;
    int b;
} Color;

typedef struct
{
    bool estado[FILAS][COLUMNAS];
    char titulo[MAX_NOMBRE];
    Horario duracion;
    char genero[MAX_GENERO];
    Horario horario;
    bool activo;
} Pelicula;

// Declaraciones de interfaz 
void bienvenidos();
void mostrarDespedida();
void primermenu();
void menuadministrador();
void MostrarEncabezado(cadena mensaje);
void opcioninvalida();

// Consultas y reportes
int MostrarPeliculasActivas(Pelicula catalogo[], int total_peliculas);
void MostrarPeliculaEncontrada(Pelicula catalogo[], int indice);
void MostrarMenuModificar(Pelicula catalogo[], int indice);
void listarPeliculas(Pelicula catalogo[], int total_peliculas);
void listarPeliculasPrincipio(Pelicula catalogo[], int total_peliculas);
void mostrar_ticket(Pelicula *pelicula, int asientos_sesion[], int total_comprados, float total_pagado);

// Flujo principal
int solicitarOpcionPrincipal(Pelicula catalogo[], int total_peliculas);
void iniciarSistema(Pelicula catalogo[], int *total_peliculas);
void procesarOpcionPrincipal(int op, Pelicula catalogo[], int *total_peliculas);
void gestionarAccesoAdmin(Pelicula catalogo[], int *total_peliculas);
void ejecutarAdmin(Pelicula catalogo[], int *total_peliculas);
int verifiadmi();

// Funciones de catalogo
Pelicula leerpelicula();
void EliminarPelicula(Pelicula catalogo[], int total_peliculas);
void BuscarPelicula(Pelicula catalogo[], int total_peliculas);
void ModificarPelicula(Pelicula catalogo[], int *total_peliculas);
int BuscarIndicePelicula(Pelicula catalogo[], int total_peliculas, char *titulo_buscar);
void EjecutarMenuModificar(Pelicula *pelicula, int opcion_campo);
void ModificarTitulo(char *titulo);
void ModificarGenero(char *genero);
void ModificarDuracion(Horario *duracion);
void ModificarHorario(Horario *horario);
void agregarpelicula(Pelicula catalogo[], int *total_peliculas, int indice);
int ObtenerIndiceReal(Pelicula catalogo[], int total_peliculas, int opcion_visual);
void Ordenamiento_Horario(Pelicula catalogo[], int total_peliculas);

// Gestion de asientos
void comprarEntradas(Pelicula catalogo[], int total_peliculas);
void procesar_compra(Pelicula *pelicula);
void actualizar_interfaz(bool estado[FILAS][COLUMNAS]);
void mostrar_sala(bool estado[FILAS][COLUMNAS]);
void dibujar_leyenda(void);
void dibujar_encabezado(int libres);
void inicializar_sala(bool estado[FILAS][COLUMNAS]);
int contar_asientos_libres(bool estado[FILAS][COLUMNAS]);
Color determinar_color_por_fila(int fila);
float calcular_precio_asiento(int numero_asiento);

// Persistencia
void CargarSesionBinaria(Pelicula catalogo[], int *total_peliculas);
void GuardarSesionBinaria(Pelicula catalogo[], int total_peliculas);
void ExportarReporteTexto(Pelicula catalogo[], int total_peliculas);

int main()
{
    int total_peliculas = 0;
    Pelicula catalogo[MAX_PELICULAS];

    bienvenidos();
    CargarSesionBinaria(catalogo, &total_peliculas);
    habilitarModoANSI();
    iniciarSistema(catalogo, &total_peliculas);
    mostrarDespedida();

    return 0;
}

void iniciarSistema(Pelicula catalogo[], int *total_peliculas)
{
    int op = 0;
    while (op != OPCION_SALIDA)
    {
        op = solicitarOpcionPrincipal(catalogo, *total_peliculas);
        procesarOpcionPrincipal(op, catalogo, total_peliculas);
    }
}

int solicitarOpcionPrincipal(Pelicula catalogo[], int total_peliculas)
{
    listarPeliculasPrincipio(catalogo, total_peliculas);
    primermenu();
    return leerEntero("Seleccione una opcion");
}

void procesarOpcionPrincipal(int op, Pelicula catalogo[], int *total_peliculas)
{
    switch (op)
    {
    case OPCION_BUSCAR:
        limpiarPantalla();
        BuscarPelicula(catalogo, *total_peliculas);
        break;
    case OPCION_COMPRAR:
        limpiarPantalla();
        comprarEntradas(catalogo, *total_peliculas);
        break;
    case OPCION_SALIDA:
        GuardarSesionBinaria(catalogo, *total_peliculas);
        PantallaDeEspera("Saliendo..");
        break;
    case ADMINISTRACION:
        gestionarAccesoAdmin(catalogo, total_peliculas);
        break;
    default:
        opcioninvalida();
        break;
    }
}

void gestionarAccesoAdmin(Pelicula catalogo[], int *total_peliculas)
{
    limpiarPantalla();
    if (verifiadmi() == 1)
    {
        printf("\n--- ACCESO CONCEDIDO ---\n");
        ejecutarAdmin(catalogo, total_peliculas);
    }
    else
    {
        printf("\nClave incorrecta. Acceso denegado.\n");
    }
}

void bienvenidos()
{
    limpiarPantalla();
    printf("\n====================================================\n");
    printf("\n            BIENVENIDO AL CINE MILAGRO              \n");
    printf("\n====================================================\n");
    PantallaDeEspera("Ingresando");
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
    printf("===================================\n");
    printf("1. Buscar pelicula\n");
    printf("2. Comprar / Reservar asientos\n");
    printf("3. Salir\n");
    printf("-----------------------------------\n");
}

void opcioninvalida()
{
    printf(" OPCION INVALIDA, VUELVA A INGRESAR \n");
}

void ejecutarAdmin(Pelicula catalogo[], int *total_peliculas)
{
    int opadmin = 0;

    while (opadmin != SALIDAADMI)
    {
        menuadministrador();
        opadmin = leerEntero("Seleccione una opcion");

        switch (opadmin)
        {
        case OPADMI_AGREGAR:
            limpiarPantalla();
            agregarpelicula(catalogo, total_peliculas, INICIALIZACION_INDICE);
            break;
        case OPADMI_ELMINAR:
            limpiarPantalla();
            EliminarPelicula(catalogo, *total_peliculas);
            break;
        case OPADMI_MODIFICAR:
            limpiarPantalla();
            ModificarPelicula(catalogo, total_peliculas);
            break;
        case OPADMI_REPORTE:
            limpiarPantalla();
            ExportarReporteTexto(catalogo, *total_peliculas);
            break;
        case SALIDAADMI:
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
    printf("4. Generar reporte de Sistema \n");
    printf("5. Volver al menu principal\n");
    printf("-----------------------------------\n");
}

int verifiadmi()
{
    int clave = 0;
    printf("\nIngrese clave de acceso administrativo: ");
    scanf("%d", &clave);
    LimpiarBuffer();
    return (clave == CLAVE_ADMIN);
}

void inicializar_sala(bool estado[FILAS][COLUMNAS])
{
    int i = 0;
    while (i < TOTAL_ASIENTOS)
    {
        int fila = i / COLUMNAS;
        int columna = i % COLUMNAS;
        estado[fila][columna] = false;
        i++;
    }
}

int contar_asientos_libres(bool estado[FILAS][COLUMNAS])
{
    int i = 0;
    int libres = 0;
    while (i < TOTAL_ASIENTOS)
    {
        int fila = i / COLUMNAS;
        int columna = i % COLUMNAS;
        if (!estado[fila][columna])
        {
            libres++;
        }
        i++;
    }
    return libres;
}

Pelicula leerpelicula()
{
    Pelicula catalogo;
    printf("\n--- AGREGAR NUEVA PELICULA ---\n");
    leerCadena("Titulo de la pelicula: ", catalogo.titulo, MAX_NOMBRE);
    leerCadena("Genero: ", catalogo.genero, MAX_GENERO);
    printf("\n--- Duracion de la pelicula ---\n");
    catalogo.duracion.hora = leerEnteroEntre(MIN_HORA_PELICULA, MAX_HORA_PELICULA, " > Horas (1 a 3)");
    catalogo.duracion.minutos = leerEnteroEntre(0, MINUTOS, " > Minutos (0 a 59)");
    printf("\n--- Horario de la funcion ---\n");
    catalogo.horario.hora = leerEnteroEntre(0, HORAS_DIA, " Hora (0 a 23)");
    catalogo.horario.minutos = leerEnteroEntre(0, MINUTOS, " Minutos (0 a 59)");
    catalogo.activo = true;

    inicializar_sala(catalogo.estado);

    PantallaDeEspera("Cargando pelicula...");
    printf(" > Pelicula '%s' agregada con exito!\n", catalogo.titulo);
    return catalogo;
}

void EliminarPelicula(Pelicula catalogo[], int total_peliculas)
{
    printf("\n ELIMINAR PELICULA \n");
    if (total_peliculas == 0)
    {
        printf("El catalogo esta vacio.\n");
    }
    else
    {
        int activas = MostrarPeliculasActivas(catalogo, total_peliculas);
        if (activas > 0)
        {
            int buscar = leerEnteroEntre(0, activas, "Ingrese el numero de la pelicula a eliminar (0 para cancelar): ");
            if (buscar != 0)
            {
                int indice = ObtenerIndiceReal(catalogo, total_peliculas, buscar);
                if (indice != INICIALIZACION_INDICE)
                {
                    catalogo[indice].activo = false;
                    printf(" > La pelicula '%s' cambio su estado a 'No disponible'.\n", catalogo[indice].titulo);
                    FILE *historial = fopen("Historial_Bajas.txt", "a");
                    if (historial != NULL)
                    {
                        fprintf(historial, "BAJA REGISTRADA: %s (Genero: %s)\n", catalogo[indice].titulo, catalogo[indice].genero);
                        fclose(historial);
                    }
                    else
                    {
                        printf("No se logro registrar la baja.\n");
                    }
                }
            }
        }
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
    int indice = INICIALIZACION_INDICE;

    printf("\n BUSCAR PELICULA \n");
    if (total_peliculas == 0)
    {
        printf("El catalogo esta vacio.\n");
    }
    else
    {
        leerCadena("Ingrese el titulo a buscar: ", titulo_buscar, MAX_NOMBRE);
        indice = BuscarIndicePelicula(catalogo, total_peliculas, titulo_buscar);

        if (indice != INICIALIZACION_INDICE)
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
    int opcion_campo = 0;
    int indice = INICIALIZACION_INDICE;
    int opcion_visual = 0;

    if (*total_peliculas == 0)
    {
        printf("\nEl catalogo esta vacio. No hay peliculas para modificar.\n");
    }
    else
    {
        MostrarEncabezado("MODIFICAR PELICULA");
        int activas = MostrarPeliculasActivas(catalogo, *total_peliculas);

        if (activas > 0)
        {
            opcion_visual = leerEnteroEntre(1, activas, "Ingrese el numero de la pelicula que desea modificar: ");
            indice = ObtenerIndiceReal(catalogo, *total_peliculas, opcion_visual);

            if (indice == INICIALIZACION_INDICE)
            {
                printf("Error: No se encontro ninguna pelicula.\n");
            }
            else
            {
                do
                {
                    MostrarMenuModificar(catalogo, indice);
                    opcion_campo = leerEnteroEntre(1, FILAS, "Seleccione una opcion: ");
                    EjecutarMenuModificar(&catalogo[indice], opcion_campo);
                } while (opcion_campo != FILAS);
            }
        }
    }
}

void MostrarEncabezado(cadena mensaje)
{
    printf("\n==============================================================================================================\n");
    printf("%-85s\n", mensaje);
    printf("==============================================================================================================\n");
    printf("\t%-40s %-20s %-10s %-10s\n", "Titulo", "Genero", "Duracion", "Horario");
    printf("--------------------------------------------------------------------------------------------------------------\n");
}

int MostrarPeliculasActivas(Pelicula catalogo[], int total_peliculas)
{
    int peliculas_activas = 0;
    MostrarEncabezado("\n\tLista de Peliculas\n");
    for (int i = 0; i < total_peliculas; i++)
    {
        if (catalogo[i].activo)
        {
            peliculas_activas++;
            printf("[ %d ] %-40s %-25s %dh %02dm     %02d:%02d hs\n",
                   peliculas_activas,
                   catalogo[i].titulo,
                   catalogo[i].genero,
                   catalogo[i].duracion.hora,
                   catalogo[i].duracion.minutos,
                   catalogo[i].horario.hora,
                   catalogo[i].horario.minutos);
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
    int posicion = INICIALIZACION_INDICE;
    int i = 0;
    while (i < total_peliculas && posicion == INICIALIZACION_INDICE)
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
    case OPMODIFICARTITULO:
        ModificarTitulo((*pelicula).titulo);
        break;
    case OPMODIFICARGENERO:
        ModificarGenero((*pelicula).genero);
        break;
    case OPMODIFICARDURACION:
        ModificarDuracion(&((*pelicula).duracion));
        break;
    case OPMODIFICARHORARIO:
        ModificarHorario(&((*pelicula).horario));
        break;
    case SALIDAADMI:
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
    (*duracion).hora = leerEnteroEntre(MIN_HORA_PELICULA, MAX_HORA_PELICULA, "Nueva Duracion (Horas): ");
    (*duracion).minutos = leerEnteroEntre(0, MINUTOS, "Nueva Duracion (Minutos): ");
    printf(">> Duracion actualizada correctamente.\n");
}

void ModificarHorario(Horario *horario)
{
    (*horario).hora = leerEnteroEntre(0, HORAS_DIA, "Nuevo Horario (Hora 0-23): ");
    (*horario).minutos = leerEnteroEntre(0, MINUTOS, "Nuevo Horario (Minutos 0-59): ");
    printf(">> Horario actualizado correctamente.\n");
}

Color determinar_color_por_fila(int fila)
{
    Color color_asignado;
    if (fila == 0)
    {
        color_asignado = COLOR_PREMIUM;
    }
    else if (fila < 3)
    {
        color_asignado = COLOR_MEDIA;
    }
    else
    {
        color_asignado = COLOR_GENERAL;
    }
    return color_asignado;
}

void dibujar_encabezado(int libres)
{
    printf("\033[1;36m========================================================================\033[0m\n");
    printf("\033[1;37m                       SISTEMA DE RESERVA DE CINE                       \033[0m\n");
    printf("\033[1;36m========================================================================\033[0m\n");
    printf(" Asientos Totales: \033[1m%d\033[0m | Disponibles: \033[1;32m%d\033[0m | Ocupados: \033[1;31m%d\033[0m\n", TOTAL_ASIENTOS, libres, TOTAL_ASIENTOS - libres);
    printf("------------------------------------------------------------------------\n\n");
    printf("              \033[1;30;47m            [ P A N T A L L A ]            \033[0m\n\n");
}

void dibujar_leyenda(void)
{
    printf("\n\033[1mLEYENDA:\033[0m\n");
    printf(" \033[48;2;%d;%d;%dm \033[30m 01 \033[0m Premium  Precio[ %.2f]   ", COLOR_PREMIUM.r, COLOR_PREMIUM.g, COLOR_PREMIUM.b, PRECIO_PREMIUM);
    printf(" \033[48;2;%d;%d;%dm \033[30m 01 \033[0m Media    Precio[ %.2f]\n", COLOR_MEDIA.r, COLOR_MEDIA.g, COLOR_MEDIA.b, PRECIO_MEDIA);
    printf(" \033[48;2;%d;%d;%dm \033[30m 01 \033[0m General  Precio[ %.2f]   ", COLOR_GENERAL.r, COLOR_GENERAL.g, COLOR_GENERAL.b, PRECIO_GENERAL);
    printf(" \033[48;2;%d;%d;%dm \033[37m XX \033[0m Reservado\n", COLOR_RESERVADO.r, COLOR_RESERVADO.g, COLOR_RESERVADO.b);
    printf("------------------------------------------------------------------------\n");
}

void mostrar_sala(bool estado[FILAS][COLUMNAS])
{
    int i = 0;
    while (i < TOTAL_ASIENTOS)
    {
        int fila = i / COLUMNAS;
        int columna = i % COLUMNAS;

        if (columna == 0)
        {
            printf("  \033[1;33mFila %c\033[0m  ", 'A' + fila);
        }

        if (estado[fila][columna])
        {
            printf("\033[48;2;%d;%d;%dm\033[1;37m|XX|\033[0m ",
                   COLOR_RESERVADO.r, COLOR_RESERVADO.g, COLOR_RESERVADO.b);
        }
        else
        {
            Color color_libre = determinar_color_por_fila(fila);
            printf("\033[48;2;%d;%d;%dm\033[30m|%02d|\033[0m ",
                   color_libre.r, color_libre.g, color_libre.b, i + 1);
        }

        if ((i + 1) % COLUMNAS == 0)
        {
            printf("\n\n");
        }

        i++;
    }
    dibujar_leyenda();
}

void actualizar_interfaz(bool estado[FILAS][COLUMNAS])
{
    limpiarPantalla();
    int libres = contar_asientos_libres(estado);
    dibujar_encabezado(libres);
    mostrar_sala(estado);
}

void procesar_compra(Pelicula *pelicula)
{
    int asientos_disponibles = contar_asientos_libres((*pelicula).estado);
    int cantidad = leerEnteroEntre(0, asientos_disponibles, " > ¿Cuantas entradas desea comprar? (0 para salir): ");

    if (cantidad != 0)
    {
        int asientos_sesion[TOTAL_ASIENTOS];
        int contador_sesion = 0;
        float total_pagar = 0.0f;

        for (int i = 0; i < cantidad; i++)
        {
            bool seleccion_exitosa = false;
            printf("\n\033[1;34m[Reserva %d de %d]\033[0m\n", i + 1, cantidad);

            while (!seleccion_exitosa)
            {
                int numero_deseado = leerEnteroEntre(BOLETO_MIN, BOLETO_MAX, " > Seleccione el numero de asiento a reservar: ");
                int fila = (numero_deseado - 1) / COLUMNAS;
                int columna = (numero_deseado - 1) % COLUMNAS;

                if ((*pelicula).estado[fila][columna])
                {
                    printf("  \033[31m[✘] El asiento |%02d| ya se encuentra ocupado. Elija otro.\033[0m\n\n", numero_deseado);
                }
                else
                {
                    (*pelicula).estado[fila][columna] = true;
                    seleccion_exitosa = true;
                    asientos_sesion[contador_sesion++] = numero_deseado;
                    total_pagar += calcular_precio_asiento(numero_deseado);
                }
            }

            actualizar_interfaz((*pelicula).estado);
            printf("  \033[32m[✔] Asiento asignado. Subtotal acumulado: $%.2f\033[0m\n\n", total_pagar);
        }

        if (confirmaUsuario("¿Desea finalizar la compra y emitir el ticket?"))
        {
            mostrar_ticket(pelicula, asientos_sesion, contador_sesion, total_pagar);
        }
        else
        {
            printf("\nCompra finalizada sin imprimir ticket. Volviendo al menu...\n");
            PantallaDeEspera("");
        }
    }
}

void comprarEntradas(Pelicula catalogo[], int total_peliculas)
{
    if (total_peliculas == 0)
    {
        printf("El catalogo esta vacio. No hay peliculas para comprar.\n");
    }
    else
    {
        int activas = MostrarPeliculasActivas(catalogo, total_peliculas);

        if (activas > 0)
        {
            printf("0) Volver al menu\n");
            int opcion = leerEnteroEntre(0, activas, "\n > Seleccione el numero de la pelicula: ");

            if (opcion != 0)
            {
                int indice = ObtenerIndiceReal(catalogo, total_peliculas, opcion);

                if (indice != INICIALIZACION_INDICE)
                {
                    printf("\n--- COMPRANDO ENTRADAS PARA: '%s' ---\n", catalogo[indice].titulo);
                    actualizar_interfaz(catalogo[indice].estado);
                    procesar_compra(&catalogo[indice]);
                }
            }
        }
    }
}

void mostrar_ticket(Pelicula *pelicula, int asientos_sesion[], int total_comprados, float total_pagado)
{
    limpiarPantalla();
    printf("\n==================================================\n");
    printf("          TICKET DE COMPRA - CINE                 \n");
    printf("==================================================\n");
    printf(" Pelicula : %s\n", pelicula->titulo);
    printf(" Genero   : %s\n", pelicula->genero);
    printf(" Horario  : %02d:%02d hs\n", pelicula->horario.hora, pelicula->horario.minutos);
    printf(" Duracion : %dh %02dm\n", pelicula->duracion.hora, pelicula->duracion.minutos);
    printf("--------------------------------------------------\n");
    printf(" Asientos adquiridos (%d):\n", total_comprados);

    for (int i = 0; i < total_comprados; i++)
    {
        int num = asientos_sesion[i];
        int fila = (num - 1) / COLUMNAS;
        int col = (num - 1) % COLUMNAS;
        float precio_individual = calcular_precio_asiento(num);
        printf("   -> Asiento #%02d (Fila %c, Col %d) $%.2f\n", num, 'A' + fila, col + 1, precio_individual);
    }

    printf("--------------------------------------------------\n");
    printf(" TOTAL ABONADO:                         $%.2f\n", total_pagado);
    printf("==================================================\n");
    printf(" ¡Gracias por elegir El Milagro del Cine!\n");
    printf("==================================================\n");
    printf("\nPresione Enter para volver al menu principal...");
    LimpiarBuffer();
    getchar();
}

void listarPeliculasPrincipio(Pelicula catalogo[], int total_peliculas)
{
    int peliculas_activas = 0;

    printf("\n=======================================================\n");
    printf("                  PELICULAS ACTIVAS                    \n");
    printf("=======================================================\n");
    printf("%-48s %-28s \n", "Titulo", "Horario");
    printf("-------------------------------------------------------\n");

    Ordenamiento_Horario(catalogo, total_peliculas);
    for (int i = 0; i < total_peliculas; i++)
    {
        if (catalogo[i].activo)
        {
            peliculas_activas++;
            printf("[ %d ] %-45s %02d:%02d \n", peliculas_activas, catalogo[i].titulo, catalogo[i].horario.hora, catalogo[i].horario.minutos);
        }
    }
}

float calcular_precio_asiento(int numero_asiento)
{
    float precio_calculado;
    int fila = (numero_asiento - 1) / COLUMNAS;

    if (fila == 0)
    {
        precio_calculado = PRECIO_PREMIUM;
    }
    else if (fila < 3)
    {
        precio_calculado = PRECIO_MEDIA;
    }
    else
    {
        precio_calculado = PRECIO_GENERAL;
    }
    return precio_calculado;
}

int ObtenerIndiceReal(Pelicula catalogo[], int total_peliculas, int opcion_visual)
{
    int posicion = INICIALIZACION_INDICE;
    int contador = 0;
    int i = 0;

    while (i < total_peliculas && posicion == INICIALIZACION_INDICE)
    {
        if (catalogo[i].activo)
        {
            contador++;
            if (contador == opcion_visual)
            {
                posicion = i;
            }
        }
        i++;
    }
    return posicion;
}

void agregarpelicula(Pelicula catalogo[], int *total_peliculas, int indice)
{
    int i = 0;
    if (*total_peliculas >= MAX_PELICULAS)
    {
        if (confirmaUsuario("El catalogo esta lleno. ¿Desea reemplazar una pelicula inactiva?"))
        {
            while (indice == INICIALIZACION_INDICE && i < MAX_PELICULAS)
            {
                if (!catalogo[i].activo)
                {
                    indice = i;
                }
                else
                {
                    i++;
                }
            }
            catalogo[indice] = leerpelicula();
        }
        else
        {
            printf("No se agrego pelicula, catalogo lleno.....\n");
        }
    }
    else
    {
        catalogo[*total_peliculas] = leerpelicula();
        (*total_peliculas)++;
    }
}

void Ordenamiento_Horario(Pelicula catalogo[], int total_peliculas)
{
    int i = 0;
    Pelicula aux;
    while (i < (total_peliculas - 1))
    {
        if ((catalogo[i].horario.hora) > (catalogo[i + 1].horario.hora) || ((catalogo[i].horario.hora == catalogo[i + 1].horario.hora) && (catalogo[i].horario.minutos > catalogo[i + 1].horario.minutos)))
        {
            aux = catalogo[i];
            catalogo[i] = catalogo[i + 1];
            catalogo[i + 1] = aux;
            i = INICIALIZACION_INDICE;
        }

        i++;
    }
}

void CargarSesionBinaria(Pelicula catalogo[], int *total_peliculas)
{
    FILE *archivo = fopen("cine_milagro.dat", "rb");
    if (archivo != NULL)
    {
        fread(total_peliculas, sizeof(int), 1, archivo);
        fread(catalogo, sizeof(Pelicula), *total_peliculas, archivo);
        fclose(archivo);
        printf("\nSesion restaurada con exito. (%d peliculas cargadas)\n", *total_peliculas);
    }
    else
    {
        *total_peliculas = 0;
        printf("\nNo se encontro sesion previa. Iniciando sistema en limpio.\n");
    }
    PantallaDeEspera("cargando");
}

void GuardarSesionBinaria(Pelicula catalogo[], int total_peliculas)
{
    FILE *archivo = fopen("cine_milagro.dat", "wb");
    if (archivo != NULL)
    {
        fwrite(&total_peliculas, sizeof(int), 1, archivo);
        fwrite(catalogo, sizeof(Pelicula), total_peliculas, archivo);
        fclose(archivo);
        printf("\nEstado del sistema guardado correctamente en 'cine_milagro.dat'.\n");
    }
    else
    {
        printf("\nNo se pudo acceder al disco para guardar los datos.\n");
    }
}

void ExportarReporteTexto(Pelicula catalogo[], int total_peliculas)
{
    FILE *archivo = fopen("Auditoria_Cartelera.txt", "w");
    if (archivo != NULL)
    {
        Ordenamiento_Horario(catalogo, total_peliculas);

        fprintf(archivo, "=======================================================\n");
        fprintf(archivo, "           REPORTE DE AUDITORIA - CINE MILAGRO         \n");
        fprintf(archivo, "=======================================================\n\n");

        fprintf(archivo, "> PELICULAS ACTIVAS EN CARTELERA <\n");
        for (int i = 0; i < total_peliculas; i++)
        {
            if (catalogo[i].activo)
            {
                int libres = contar_asientos_libres(catalogo[i].estado);
                fprintf(archivo, "%d. %s (Horario: %02d:%02d hs) - Asientos libres: %d\n",
                        i + 1, catalogo[i].titulo, catalogo[i].horario.hora, catalogo[i].horario.minutos, libres);
            }
        }

        fprintf(archivo, "\n-------------------------------------------------------\n\n");
        fprintf(archivo, "> HISTORIAL DE PELICULAS INACTIVAS <\n");
        for (int i = 0; i < total_peliculas; i++)
        {
            if (!catalogo[i].activo)
            {
                fprintf(archivo, "- %s (Desactivada)\n", catalogo[i].titulo);
            }
        }

        fprintf(archivo, "\n=======================================================\n");
        fprintf(archivo, "Total en memoria: %d\n", total_peliculas);

        fclose(archivo);
        printf("\nEl reporte completo fue exportado en 'Auditoria_Cartelera.txt'.\n");
    }
    else
    {
        printf("\nNo se tienen permisos para generar el reporte de texto.\n");
    }
}