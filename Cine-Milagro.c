
#include "utils.h"

#define UNO 1
#define OPBUSCAR 1
#define OPCOMPRAR 2
#define OPADMI_ELMINAR 2
#define OPADMI_MODIFICAR 3
#define OPADMI_REPORTE 4
#define OPADMI_AGREGAR 1
#define OPMODIFICARTITULO 1
#define OPMODIFICARGENERO 2
#define OPMODIFICARDURACION 3
#define OPMODIFICARHORARIO 4
#define INICIALIZACION_INDICE -1
#define OCULTO 88
#define CLAVE_ADMIN 1234
#define OPSALIDA 3
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
#define DIVIDIR_HORA 100
#define DURACION_HORA 60
#define MIN_HORARIO 1000
#define MAX_HORARIO 9999
#define PRECIO_PREMIUM 5000.0
#define PRECIO_MEDIA 3500.0
#define PRECIO_GENERAL 2000.0
#define COLOR_RESERVADO (Color){220, 20, 60}
#define COLOR_PREMIUM (Color){46, 204, 113}
#define COLOR_MEDIA (Color){241, 196, 15}
#define COLOR_GENERAL (Color){52, 152, 219}
#ifdef _WIN32
#include <windows.h>
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
// sacaf solo esta para que a Jose le compile el codigo
#else
void habilitarModoANSI()
{
    // En Linux los colores ANSI funcionan por defecto de forma nativa.
    // Dejamos la función vacía para que el programa compile sin arrojar error.
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
    Color asientos[TOTAL_ASIENTOS];
    int asientos_libres;
} Pelicula;

void bienvenidos();
void mostrarDespedida();
void primermenu();
int solicitarOpcionPrincipal(Pelicula catalogo[], int total_peliculas);

void iniciarSistema(Pelicula catalogo[], int *total_peliculas);
void procesarOpcionPrincipal(int op, Pelicula catalogo[], int *total_peliculas);
void gestionarAccesoAdmin(Pelicula catalogo[], int *total_peliculas);
void ejecutarAdmin(Pelicula catalogo[], int *total_peliculas);
void menuadministrador();
int verifiadmi();

Pelicula leerpelicula();
void EliminarPelicula(Pelicula catalogo[], int total_peliculas);
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
void opcioninvalida();
void listarPeliculasPrincipio(Pelicula catalogo[], int total_peliculas);

void comprarEntradas(Pelicula catalogo[], int total_peliculas);
void procesar_compra(Color *asientos, int *asientos_libres, Pelicula *pelicula);
void reservar_asiento_interactivo(Color *asientos, int *asientos_libres);
void actualizar_interfaz(const Color *asientos, int asientos_libres);
void mostrar_sala(const Color *asientos, int total);
void dibujar_leyenda(void);
void dibujar_encabezado(int libres);
int buscar_indice_asiento(int numero_asiento);
bool son_colores_iguales(Color c1, Color c2);
bool esta_asiento_ocupado(const Color *asientos, int indice);
void inicializar_sala(Color *asientos, int total);
Color determinar_color_por_fila(int fila);
void mostrar_ticket(Pelicula *pelicula, int asientos_sesion[], int total_comprados, float total_pagado);
void leerHorarioFormato(cadena mensaje, int *hora, int *minutos, int m);
float calcular_precio_asiento(int numero_asiento);
void agregarpelicula(Pelicula catalogo[], int *total_peliculas, int indice);
int ObtenerIndiceReal(Pelicula catalogo[], int total_peliculas, int opcion_visual);
void Ordenamiento_Horario(Pelicula catalogo[], int total_peliculas);

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

void leerHorarioFormato(cadena mensaje, int *hora, int *minutos, int m)
{
    printf("%s", mensaje);
    *hora = leerEnteroEntre(0, m, "Ingrese la hora (0-23): ");
    *minutos = leerEnteroEntre(0, MINUTOS, "Ingrese los minutos (0-59): ");
    LimpiarBuffer();
}

void iniciarSistema(Pelicula catalogo[], int *total_peliculas)
{
    int op = 0;
    while (op != OPSALIDA)
    {
        op = solicitarOpcionPrincipal(catalogo, *total_peliculas);

        procesarOpcionPrincipal(op, catalogo, total_peliculas);
    }
}

int solicitarOpcionPrincipal(Pelicula catalogo[], int total_peliculas)
{
    listarPeliculasPrincipio(catalogo, total_peliculas);
    primermenu();
    return leerEntero("Seleccione una opcion: ");
}

void procesarOpcionPrincipal(int op, Pelicula catalogo[], int *total_peliculas)
{

    switch (op)
    {
    case OPBUSCAR:
        limpiarPantalla();
        BuscarPelicula(catalogo, *total_peliculas);
        break;
    case OPCOMPRAR:
        limpiarPantalla();
        comprarEntradas(catalogo, *total_peliculas);
        break;
    case OPSALIDA:
        GuardarSesionBinaria(catalogo, *total_peliculas);
        PantallaDeEspera("Saliendo..");
        break;
    case OCULTO:
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
    printf("\n            BIENVENIDO AL CINE MILAGRO           \n");
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
        opadmin = leerEntero("Seleccione una opcion: ");

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
    return (clave == CLAVE_ADMIN);
}

Pelicula leerpelicula()
{
    Pelicula catalogo;
    printf("\n--- AGREGAR NUEVA PELICULA ---\n");
    leerCadena("Titulo de la pelicula: ", catalogo.titulo, MAX_NOMBRE);
    leerHorarioFormato(" Duracion (HH:MM): ", &catalogo.duracion.hora, &catalogo.duracion.minutos, MAX_HORA_PELICULA);
    leerCadena("Genero: ", catalogo.genero, MAX_GENERO);
    leerHorarioFormato("Hora de la funcion (HH:MM): ", &catalogo.horario.hora, &catalogo.horario.minutos, HORAS_DIA);
    catalogo.activo = true;

    inicializar_sala(catalogo.asientos, TOTAL_ASIENTOS);
    catalogo.asientos_libres = TOTAL_ASIENTOS;
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
                        printf("No se logro registrar la baja.");
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

    printf("\n--- BUSCAR PELICULA ---\n");
    if (total_peliculas == 0)
    {
        printf("El catalogo esta vacio.\n");
    }
    else
    {
        leerCadena("Ingrese el titulo a buscar: ", titulo_buscar, MAX_NOMBRE);
        indice = BuscarIndicePelicula(catalogo, total_peliculas, titulo_buscar);

        if (indice != INICIALIZACION_INDICE)
            MostrarPeliculaEncontrada(catalogo, indice);
        else
            printf("No se encontro ninguna pelicula activa con ese titulo.\n");
    }
}
void ModificarPelicula(Pelicula catalogo[], int *total_peliculas)
{
    int opcion_campo;
    int indice;
    int opcion_visual;

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
                    opcion_campo = leerEnteroEntre(UNO, FILAS, "Seleccione una opcion: ");
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
    printf("-------------------------------------------------------\n");
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
        if (catalogo[i].activo && _stricmp(catalogo[i].titulo, titulo_buscar) == 0)
            posicion = i;
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

void inicializar_sala(Color *asientos, int total)
{
    for (int i = 0; i < total; i++)
    {
        int fila = i / COLUMNAS;
        asientos[i] = determinar_color_por_fila(fila);
    }
}

bool son_colores_iguales(Color c1, Color c2)
{
    return (c1.r == c2.r && c1.g == c2.g && c1.b == c2.b);
}

bool esta_asiento_ocupado(const Color *asientos, int indice)
{
    return son_colores_iguales(asientos[indice], COLOR_RESERVADO);
}

int buscar_indice_asiento(int numero_asiento)
{
    int izquierda = 0;
    int derecha = TOTAL_ASIENTOS - 1;
    int indice = INICIALIZACION_INDICE;

    while (izquierda <= derecha && indice == INICIALIZACION_INDICE)
    {
        int medio = izquierda + (derecha - izquierda) / 2;
        int asiento_actual = medio + 1;

        if (asiento_actual == numero_asiento)
        {
            indice = medio;
        }
        else if (numero_asiento < asiento_actual)
        {
            derecha = medio - 1;
        }
        else
        {
            izquierda = medio + 1;
        }
    }
    return indice;
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
    printf(" \033[48;2;%d;%d;%dm \033[30m 01 \033[0m Media  Precio[ %.2f]", COLOR_MEDIA.r, COLOR_MEDIA.g, COLOR_MEDIA.b, PRECIO_MEDIA);
    printf(" \033[48;2;%d;%d;%dm \033[30m 01 \033[0m General  Precio[ %.2f]  ", COLOR_GENERAL.r, COLOR_GENERAL.g, COLOR_GENERAL.b, PRECIO_GENERAL);
    printf(" \033[48;2;%d;%d;%dm \033[37m XX \033[0m Reservado\n", COLOR_RESERVADO.r, COLOR_RESERVADO.g, COLOR_RESERVADO.b);
    printf("------------------------------------------------------------------------\n");
}

void mostrar_sala(const Color *asientos, int total)
{
    for (int i = 0; i < total; i++)
    {
        int fila = i / COLUMNAS;
        int columna = i % COLUMNAS;

        if (columna == 0)
            printf("  \033[1;33mFila %c\033[0m  ", 'A' + fila);

        if (esta_asiento_ocupado(asientos, i))
            printf("\033[48;2;%d;%d;%dm\033[1;37m|XX|\033[0m ", asientos[i].r, asientos[i].g, asientos[i].b);
        else
            printf("\033[48;2;%d;%d;%dm\033[30m|%02d|\033[0m ", asientos[i].r, asientos[i].g, asientos[i].b, i + 1);

        if ((i + 1) % COLUMNAS == 0)
            printf("\n\n");
    }
    dibujar_leyenda();
}

void actualizar_interfaz(const Color *asientos, int asientos_libres)
{
    limpiarPantalla();
    dibujar_encabezado(asientos_libres);
    mostrar_sala(asientos, TOTAL_ASIENTOS);
}

void reservar_asiento_interactivo(Color *asientos, int *asientos_libres)
{
    int indice = -1;
    bool seleccion_exitosa = false;

    do
    {
        int numero_deseado = leerEnteroEntre(BOLETO_MIN, BOLETO_MAX, " > Seleccione el numero de asiento a reservar: ");

        indice = buscar_indice_asiento(numero_deseado);

        if (indice != -1 && esta_asiento_ocupado(asientos, indice))
        {
            printf("  \033[31m[✘] El asiento |%02d| ya se encuentra ocupado. Elija otro.\033[0m\n\n", numero_deseado);
        }
        else if (indice != -1)
        {
            seleccion_exitosa = true;
        }
    } while (!seleccion_exitosa);

    asientos[indice] = COLOR_RESERVADO;
    (*asientos_libres)--;
}

void procesar_compra(Color *asientos, int *asientos_libres, Pelicula *pelicula)
{
    int cantidad = leerEnteroEntre(0, *asientos_libres, " > ¿Cuantas entradas desea comprar? (0 para salir): ");
    if (cantidad != 0)
    {
        int asientos_sesion[TOTAL_ASIENTOS];
        int contador_sesion = 0;
        float total_pagar = 0;

        for (int i = 0; i < cantidad; i++)
        {
            int indice = INICIALIZACION_INDICE;
            bool seleccion_exitosa = false;
            printf("\n\033[1;34m[Reserva %d de %d]\033[0m\n", i + 1, cantidad);
            int numero_deseado = 0;
            int x = 0, y = 0;
            do
            {
                numero_deseado = leerEnteroEntre(BOLETO_MIN, BOLETO_MAX, " > Seleccione el numero de asiento a reservar: ");
                x = (numero_deseado - 1) / COLUMNAS;
                y = (numero_deseado - 1) % COLUMNAS;
                if ((*pelicula).estado[x][y] == true)
                {
                    printf("  \033[31m[✘] El asiento |%02d| ya se encuentra ocupado. Elija otro.\033[0m\n\n", numero_deseado);
                }
                else
                {
                    (*pelicula).estado[x][y] = true;
                    seleccion_exitosa = true;
                    asientos_sesion[contador_sesion++] = numero_deseado;
                    total_pagar += calcular_precio_asiento(numero_deseado);
                }
            } while (!seleccion_exitosa);

            asientos[indice] = COLOR_RESERVADO;
            (*asientos_libres)--;

            actualizar_interfaz(asientos, *asientos_libres);
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
        MostrarEncabezado("SELECCIONE UNA PELICULA PARA COMPRAR");
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
                    actualizar_interfaz(catalogo[indice].asientos, catalogo[indice].asientos_libres);
                    procesar_compra(catalogo[indice].asientos, &catalogo[indice].asientos_libres, &catalogo[indice]);
                }
            }
        }
    }
}
void mostrar_ticket(Pelicula *pelicula, int asientos_sesion[], int total_comprados, float total_pagado)
{
    limpiarPantalla();
    printf("\n==================================================\n");
    printf("          TICKET DE COMPRA - CINE           \n");
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
    printf("                  PELICULAS ACTIVAS\n");
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
                if (catalogo[i].activo == false)
                    indice = i;
                else
                    i++;
            }
            catalogo[indice] = leerpelicula();
        }
        else
        {
            printf("No se agrego pelicula,catalogo lleno.....\n");
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
                fprintf(archivo, "%d. %s (Horario: %02d:%02d hs) - Asientos libres: %d\n", i + 1, catalogo[i].titulo, catalogo[i].horario.hora, catalogo[i].horario.minutos, catalogo[i].asientos_libres);
            }
        }

        fprintf(archivo, "\n-------------------------------------------------------\n\n");

        fprintf(archivo, "> HISTORIAL DE PELICULAS INACTIVAS <\n");
        for (int i = 0; i < total_peliculas; i++)
        {
            if (!catalogo[i].activo)
                fprintf(archivo, "- %s (Desactivada)\n", catalogo[i].titulo);
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