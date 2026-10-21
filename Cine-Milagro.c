
#include "utils.h"

#define UNO 1
#define OCULTO 88
#define CLAVE_ADMIN 1234
#define SALIDA 4
#define MAX_PELICULAS 100
#define MAX_NOMBRE 60
#define MAX_GENERO 50
#define MINUTOS 60
#define HORAS_DIA 24
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
    int id;
    char titulo[MAX_NOMBRE];
    Horario duracion;
    char genero[MAX_GENERO];
    Horario horario;
    bool activo;
    Color asientos[TOTAL_ASIENTOS];
    int asientos_libres;
} Pelicula;

// Definicio de colores
static const Color COLOR_RESERVADO = {220, 20, 60}; // Rojo carmesí
static const Color COLOR_PREMIUM = {46, 204, 113};  // Verde esmeralda (Fila 1)
static const Color COLOR_MEDIA = {241, 196, 15};    // Amarillo dorado (Filas 2-3)
static const Color COLOR_GENERAL = {52, 152, 219};  // Azul claro (Filas 4-5)

// Funciones de menu
void bienvenidos();
void mostrarDespedida();
void primermenu();
int solicitarOpcionPrincipal(Pelicula catalogo[], int total_peliculas);

// Estructura principal
void iniciarSistema(Pelicula catalogo[], int total_peliculas);
void procesarOpcionPrincipal(int op, Pelicula catalogo[], int *total_peliculas);
void gestionarAccesoAdmin(Pelicula catalogo[], int *total_peliculas);
void ejecutarAdmin(Pelicula catalogo[], int *total_peliculas);
void menuadministrador();
int verifiadmi();

// Funcionamiento
Pelicula AgregarPelicula(int total_peliculas);
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

// Compra y funciones para matris de color
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
void leerHorarioFormato(cadena mensaje, int *hora, int *minutos);
float calcular_precio_asiento(int numero_asiento);

// Funciones de pruebas
void cargarDatosPrueba(Pelicula catalogo[], int *total_peliculas);
int BuscarIndicePeliculaPorID(Pelicula catalogo[], int total_peliculas, int id_buscar);

int main()
{
    // pelis
    int total_peliculas = 0;
    Pelicula catalogo[MAX_PELICULAS];

    bienvenidos();
    // matriz
    habilitarModoANSI();
    // CARGAMOS LOS DATOS DE PRUEBA AQUÍ
    cargarDatosPrueba(catalogo, &total_peliculas); // sacar y poner un archivo txt

    iniciarSistema(catalogo, total_peliculas);
    mostrarDespedida();
    return 0;
}

void leerHorarioFormato(cadena mensaje, int *hora, int *minutos)
{
    int leidos;

    do
    {
        printf("%s", mensaje);
        leidos = scanf("%d:%d", hora, minutos);

        if (leidos != 2)
        {
            printf("Error: Formato incorrecto. Asegurate de usar los dos puntos (ej: 14:30).\n");
            LimpiarBuffer();
        }
        else if (*hora < 0 || *hora > 23 || *minutos < 0 || *minutos > 59)
        {
            printf("Error: Tiempo fuera de rango. Vuelve a intentarlo.\n");
            leidos = 0; // Forzamos el valor a 0 para que el bucle while se repita
            LimpiarBuffer();
        }
    } while (leidos != 2);
}
void iniciarSistema(Pelicula catalogo[], int total_peliculas)
{
    int op = 0;
    while (op != SALIDA)
    {
        op = solicitarOpcionPrincipal(catalogo, total_peliculas);

        procesarOpcionPrincipal(op, catalogo, &total_peliculas);
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
    case 1:
        limpiarPantalla();
        BuscarPelicula(catalogo, *total_peliculas);
        break;
    case 2:
        limpiarPantalla();
        comprarEntradas(catalogo, *total_peliculas);
        break;
    case 3:
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

    while (opadmin != SALIDA)
    {
        menuadministrador();
        opadmin = leerEntero("Seleccione una opcion: ");

        switch (opadmin)
        {
        case 1:
            limpiarPantalla();
            if (*total_peliculas >= MAX_PELICULAS)
            {
                printf("Error: El catalogo esta lleno.\n");
            }
            else
            {
                catalogo[*total_peliculas] = AgregarPelicula(*total_peliculas);
                (*total_peliculas)++;
            }
            break;
        case 2:
            limpiarPantalla();
            EliminarPelicula(catalogo, *total_peliculas);
            break;
        case 3:
            limpiarPantalla();
            ModificarPelicula(catalogo, total_peliculas);
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

Pelicula AgregarPelicula(int total_pelicula)
{
    Pelicula catalogo;
    printf("\n--- AGREGAR NUEVA PELICULA ---\n");
    leerCadena("Titulo de la pelicula: ", catalogo.titulo, MAX_NOMBRE);
    leerHorarioFormato(" Duracion (HH/MM): ", &catalogo.duracion.hora, &catalogo.duracion.minutos);
    leerCadena("Genero: ", catalogo.genero, MAX_GENERO);
    leerHorarioFormato("Hora de la funcion (HH/MM): ", &catalogo.horario.hora, &catalogo.horario.minutos);
    catalogo.activo = true;
    catalogo.id = total_pelicula + 1;

    inicializar_sala(catalogo.asientos, TOTAL_ASIENTOS);
    catalogo.asientos_libres = TOTAL_ASIENTOS;
    PantallaDeEspera("Cargando pelicula...");
    printf(" > Pelicula '%s' agregada con exito!\n", catalogo.titulo);
    return catalogo;
}

void EliminarPelicula(Pelicula catalogo[], int total_peliculas)
{
    char titulo_buscar[MAX_NOMBRE];
    bool encontrada = false;

    printf("\n ELIMINAR PELICULA \n");
    if (total_peliculas == 0)
    {
        printf("El catalogo esta vacio.\n");
    }
    else
    {
        leerCadena("Ingrese el titulo de la pelicula que desea eliminar: ", titulo_buscar, MAX_NOMBRE);
        for (int i = 0; i < total_peliculas; i++)
        {
            if (strcmp(catalogo[i].titulo, titulo_buscar) == 0 && catalogo[i].activo)
            {
                catalogo[i].activo = false;
                encontrada = true;
                printf(" > La pelicula '%s' cambio su estado a 'No disponible'.\n", catalogo[i].titulo);
            }
        }
        if (!encontrada)
            printf("Error: No se encontro la pelicula o ya estaba 'No disponible'.\n");
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
            MostrarPeliculaEncontrada(catalogo, indice);
        else
            printf("No se encontro ninguna pelicula activa con ese titulo.\n");
    }
}
void ModificarPelicula(Pelicula catalogo[], int *total_peliculas)
{
    int opcion_campo;
    int indice;
    int id;

    if (*total_peliculas == 0)
    {
        printf("\nEl catalogo esta vacio. No hay peliculas para modificar.\n");
    }
    else
    {
        MostrarEncabezado("MODIFICAR PELICULA");
        MostrarPeliculasActivas(catalogo, *total_peliculas);

        id = leerEntero("Ingrese el ID de la pelicula que desea modificar: ");
        indice = BuscarIndicePeliculaPorID(catalogo, *total_peliculas, id);

        if (indice == -1)
        {
            printf("Error: No se encontro ninguna pelicula activa con el ID ingresado .\n");
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

void MostrarEncabezado(cadena mensaje)
{
    printf("\n==============================================================================================================\n");
    printf("%-85s\n", mensaje);
    printf("==============================================================================================================\n");
    printf("\t%-40s %-20s %-10s %-10s\n", "Titulo", "Genero", "Duracion", "Horario");
    printf("-------------------------------------------------------\n");
}

// VEr que onda con el int
int MostrarPeliculasActivas(Pelicula catalogo[], int total_peliculas)
{
    int peliculas_activas = 0;
    MostrarEncabezado("\n\tLista de Peliculas\n");
    for (int i = 0; i < total_peliculas; i++)
    {
        if (catalogo[i].activo)
        {
            printf("[ %d ] %-40s %-25s %dh %02dm     %02d:%02d hs\n",
                   catalogo[i].id,
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
    case 1:
        ModificarTitulo((*pelicula).titulo);
        break;
    case 2:
        ModificarGenero((*pelicula).genero);
        break;
    case 3:
        ModificarDuracion(&((*pelicula).duracion));
        break;
    case 4:
        ModificarHorario(&((*pelicula).horario));
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
    (*duracion).hora = leerEntero("Nueva Duracion (Horas): ");
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
    int indice = -1;

    while (izquierda <= derecha && indice == -1)
    {
        int medio = izquierda + (derecha - izquierda) / 2;
        int asiento_actual = medio + 1; // Los asientos se enumeran de 1 a 50

        if (asiento_actual == numero_asiento)
        {
            indice = medio; // Retorna el índice (0 a 49)
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
            int indice = -1;
            bool seleccion_exitosa = false;
            printf("\n\033[1;34m[Reserva %d de %d]\033[0m\n", i + 1, cantidad);
            int numero_deseado = 0;

            do
            {
                numero_deseado = leerEnteroEntre(BOLETO_MIN, BOLETO_MAX, " > Seleccione el numero de asiento a reservar: ");
                indice = buscar_indice_asiento(numero_deseado);

                if (indice != -1 && esta_asiento_ocupado(asientos, indice))
                {
                    printf("  \033[31m[✘] El asiento |%02d| ya se encuentra ocupado. Elija otro.\033[0m\n\n", numero_deseado);
                }
                else if (indice != -1)
                {
                    seleccion_exitosa = true;
                    // Guardamos el número de asiento para el ticket
                    asientos_sesion[contador_sesion++] = numero_deseado;
                    total_pagar += calcular_precio_asiento(numero_deseado);
                }
            } while (!seleccion_exitosa);

            // Asignación en memoria
            asientos[indice] = COLOR_RESERVADO;
            (*asientos_libres)--;

            actualizar_interfaz(asientos, *asientos_libres);
            printf("  \033[32m[✔] Asiento asignado. Subtotal acumulado: $%.2f\033[0m\n\n", total_pagar);
        }

        // Preguntar si desea finalizar la compra
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
    int indices_activos[MAX_PELICULAS];
    int peliculas_activas = 0;
    if (total_peliculas == 0)
    {
        printf("El catalogo esta vacio. No hay peliculas para comprar.\n");
    }
    else
    {
        MostrarEncabezado("SELECCIONE UNA PELICULA PARA COMPRAR");
        for (int i = 0; i < total_peliculas; i++)
        {
            if (catalogo[i].activo)
            {
                indices_activos[peliculas_activas] = i;
                peliculas_activas++;
            }
        }
        MostrarPeliculasActivas(catalogo, total_peliculas);

        printf("0) Volver al menu\n");
        int opcion = leerEnteroEntre(0, peliculas_activas, "\n > Seleccione el numero de la pelicula: ");
        if (opcion != 0)
        {
            int indice = indices_activos[opcion - 1];
            actualizar_interfaz(catalogo[indice].asientos, catalogo[indice].asientos_libres);
            procesar_compra(catalogo[indice].asientos, &catalogo[indice].asientos_libres, &catalogo[indice]);
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

// Preacrgado de datos void cargarDatosPrueba(Pelicula catalogo[], int *total_peliculas)
void cargarDatosPrueba(Pelicula catalogo[], int *total_peliculas)
{
    // Verificamos que haya espacio al menos para 13 películas (3 orig. + 4 nuevas + 6 Terminators)
    if (*total_peliculas + 13 > MAX_PELICULAS)
    {
        printf("\n [ERROR] No hay suficiente espacio en el catalogo para cargar los datos de prueba.\n");
        return;
    }

    // ==========================================
    // PELÍCULAS ORIGINALES
    // ==========================================

    // --- PELICULA 1 ---
    catalogo[*total_peliculas].id = 1;
    strcpy(catalogo[*total_peliculas].titulo, "Batman: El Caballero de la Noche");
    catalogo[*total_peliculas].duracion.hora = 2;
    catalogo[*total_peliculas].duracion.minutos = 32;
    strcpy(catalogo[*total_peliculas].genero, "Accion");
    catalogo[*total_peliculas].horario.hora = 20;
    catalogo[*total_peliculas].horario.minutos = 15;
    catalogo[*total_peliculas].activo = true;
    inicializar_sala(catalogo[*total_peliculas].asientos, TOTAL_ASIENTOS);
    catalogo[*total_peliculas].asientos_libres = TOTAL_ASIENTOS;
    (*total_peliculas)++;

    // --- PELICULA 2 ---
    catalogo[*total_peliculas].id = 2;
    strcpy(catalogo[*total_peliculas].titulo, "Interstellar");
    catalogo[*total_peliculas].duracion.hora = 2;
    catalogo[*total_peliculas].duracion.minutos = 49;
    strcpy(catalogo[*total_peliculas].genero, "Ciencia Ficcion");
    catalogo[*total_peliculas].horario.hora = 18;
    catalogo[*total_peliculas].horario.minutos = 30;
    catalogo[*total_peliculas].activo = true;
    inicializar_sala(catalogo[*total_peliculas].asientos, TOTAL_ASIENTOS);
    catalogo[*total_peliculas].asientos_libres = TOTAL_ASIENTOS;
    (*total_peliculas)++;

    // --- PELICULA 3 ---
    catalogo[*total_peliculas].id = 3;
    strcpy(catalogo[*total_peliculas].titulo, "Shrek 2");
    catalogo[*total_peliculas].duracion.hora = 1;
    catalogo[*total_peliculas].duracion.minutos = 33;
    strcpy(catalogo[*total_peliculas].genero, "Animacion");
    catalogo[*total_peliculas].horario.hora = 16;
    catalogo[*total_peliculas].horario.minutos = 0;
    catalogo[*total_peliculas].activo = true;
    inicializar_sala(catalogo[*total_peliculas].asientos, TOTAL_ASIENTOS);
    catalogo[*total_peliculas].asientos_libres = TOTAL_ASIENTOS;
    (*total_peliculas)++;

    // ==========================================
    // 4 PELÍCULAS NUEVAS
    // ==========================================

    // --- PELICULA 4 ---
    catalogo[*total_peliculas].id = 4;
    strcpy(catalogo[*total_peliculas].titulo, "Boku no Hero Academia: Two Heroes");
    catalogo[*total_peliculas].duracion.hora = 1;
    catalogo[*total_peliculas].duracion.minutos = 36;
    strcpy(catalogo[*total_peliculas].genero, "Anime / Accion");
    catalogo[*total_peliculas].horario.hora = 14;
    catalogo[*total_peliculas].horario.minutos = 0;
    catalogo[*total_peliculas].activo = true;
    inicializar_sala(catalogo[*total_peliculas].asientos, TOTAL_ASIENTOS);
    catalogo[*total_peliculas].asientos_libres = TOTAL_ASIENTOS;
    (*total_peliculas)++;

    // --- PELICULA 5 ---
    catalogo[*total_peliculas].id = 5;
    strcpy(catalogo[*total_peliculas].titulo, "The Matrix");
    catalogo[*total_peliculas].duracion.hora = 2;
    catalogo[*total_peliculas].duracion.minutos = 16;
    strcpy(catalogo[*total_peliculas].genero, "Ciencia Ficcion");
    catalogo[*total_peliculas].horario.hora = 22;
    catalogo[*total_peliculas].horario.minutos = 45;
    catalogo[*total_peliculas].activo = true;
    inicializar_sala(catalogo[*total_peliculas].asientos, TOTAL_ASIENTOS);
    catalogo[*total_peliculas].asientos_libres = TOTAL_ASIENTOS;
    (*total_peliculas)++;

    // --- PELICULA 6 ---
    catalogo[*total_peliculas].id = 6;
    strcpy(catalogo[*total_peliculas].titulo, "El Señor de los Anillos: El Retorno del Rey");
    catalogo[*total_peliculas].duracion.hora = 3;
    catalogo[*total_peliculas].duracion.minutos = 21;
    strcpy(catalogo[*total_peliculas].genero, "Fantasia");
    catalogo[*total_peliculas].horario.hora = 19;
    catalogo[*total_peliculas].horario.minutos = 0;
    catalogo[*total_peliculas].activo = true;
    inicializar_sala(catalogo[*total_peliculas].asientos, TOTAL_ASIENTOS);
    catalogo[*total_peliculas].asientos_libres = TOTAL_ASIENTOS;
    (*total_peliculas)++;

    // --- PELICULA 7 ---
    catalogo[*total_peliculas].id = 7;
    strcpy(catalogo[*total_peliculas].titulo, "Gladiador");
    catalogo[*total_peliculas].duracion.hora = 2;
    catalogo[*total_peliculas].duracion.minutos = 35;
    strcpy(catalogo[*total_peliculas].genero, "Accion / Drama");
    catalogo[*total_peliculas].horario.hora = 21;
    catalogo[*total_peliculas].horario.minutos = 15;
    catalogo[*total_peliculas].activo = true;
    inicializar_sala(catalogo[*total_peliculas].asientos, TOTAL_ASIENTOS);
    catalogo[*total_peliculas].asientos_libres = TOTAL_ASIENTOS;
    (*total_peliculas)++;

    // ==========================================
    // SAGA TERMINATOR (6 Entregas)
    // ==========================================

    // --- PELICULA 8 ---
    catalogo[*total_peliculas].id = 8;
    strcpy(catalogo[*total_peliculas].titulo, "Terminator");
    catalogo[*total_peliculas].duracion.hora = 1;
    catalogo[*total_peliculas].duracion.minutos = 47;
    strcpy(catalogo[*total_peliculas].genero, "Ciencia Ficcion");
    catalogo[*total_peliculas].horario.hora = 15;
    catalogo[*total_peliculas].horario.minutos = 30;
    catalogo[*total_peliculas].activo = true;
    inicializar_sala(catalogo[*total_peliculas].asientos, TOTAL_ASIENTOS);
    catalogo[*total_peliculas].asientos_libres = TOTAL_ASIENTOS;
    (*total_peliculas)++;

    // --- PELICULA 9 ---
    catalogo[*total_peliculas].id = 9;
    strcpy(catalogo[*total_peliculas].titulo, "Terminator 2: El Juicio Final");
    catalogo[*total_peliculas].duracion.hora = 2;
    catalogo[*total_peliculas].duracion.minutos = 17;
    strcpy(catalogo[*total_peliculas].genero, "Ciencia Ficcion");
    catalogo[*total_peliculas].horario.hora = 17;
    catalogo[*total_peliculas].horario.minutos = 45;
    catalogo[*total_peliculas].activo = true;
    inicializar_sala(catalogo[*total_peliculas].asientos, TOTAL_ASIENTOS);
    catalogo[*total_peliculas].asientos_libres = TOTAL_ASIENTOS;
    (*total_peliculas)++;

    // --- PELICULA 10 ---
    catalogo[*total_peliculas].id = 10;
    strcpy(catalogo[*total_peliculas].titulo, "Terminator 3: La Rebelion de las Maquinas");
    catalogo[*total_peliculas].duracion.hora = 1;
    catalogo[*total_peliculas].duracion.minutos = 49;
    strcpy(catalogo[*total_peliculas].genero, "Ciencia Ficcion");
    catalogo[*total_peliculas].horario.hora = 20;
    catalogo[*total_peliculas].horario.minutos = 30;
    catalogo[*total_peliculas].activo = true;
    inicializar_sala(catalogo[*total_peliculas].asientos, TOTAL_ASIENTOS);
    catalogo[*total_peliculas].asientos_libres = TOTAL_ASIENTOS;
    (*total_peliculas)++;

    // --- PELICULA 11 ---
    catalogo[*total_peliculas].id = 11;
    strcpy(catalogo[*total_peliculas].titulo, "Terminator: La Salvacion");
    catalogo[*total_peliculas].duracion.hora = 1;
    catalogo[*total_peliculas].duracion.minutos = 55;
    strcpy(catalogo[*total_peliculas].genero, "Ciencia Ficcion");
    catalogo[*total_peliculas].horario.hora = 22;
    catalogo[*total_peliculas].horario.minutos = 45;
    catalogo[*total_peliculas].activo = true;
    inicializar_sala(catalogo[*total_peliculas].asientos, TOTAL_ASIENTOS);
    catalogo[*total_peliculas].asientos_libres = TOTAL_ASIENTOS;
    (*total_peliculas)++;

    // --- PELICULA 12 ---
    catalogo[*total_peliculas].id = 12;
    strcpy(catalogo[*total_peliculas].titulo, "Terminator: Genesis");
    catalogo[*total_peliculas].duracion.hora = 2;
    catalogo[*total_peliculas].duracion.minutos = 6;
    strcpy(catalogo[*total_peliculas].genero, "Ciencia Ficcion");
    catalogo[*total_peliculas].horario.hora = 13;
    catalogo[*total_peliculas].horario.minutos = 15;
    catalogo[*total_peliculas].activo = true;
    inicializar_sala(catalogo[*total_peliculas].asientos, TOTAL_ASIENTOS);
    catalogo[*total_peliculas].asientos_libres = TOTAL_ASIENTOS;
    (*total_peliculas)++;

    // --- PELICULA 13 ---
    catalogo[*total_peliculas].id = 13;
    strcpy(catalogo[*total_peliculas].titulo, "Terminator: Destino Oculto");
    catalogo[*total_peliculas].duracion.hora = 2;
    catalogo[*total_peliculas].duracion.minutos = 8;
    strcpy(catalogo[*total_peliculas].genero, "Ciencia Ficcion");
    catalogo[*total_peliculas].horario.hora = 16;
    catalogo[*total_peliculas].horario.minutos = 20;
    catalogo[*total_peliculas].activo = true;
    inicializar_sala(catalogo[*total_peliculas].asientos, TOTAL_ASIENTOS);
    catalogo[*total_peliculas].asientos_libres = TOTAL_ASIENTOS;
    (*total_peliculas)++;
}

// Nuevas funcion
int BuscarIndicePeliculaPorID(Pelicula catalogo[], int total_peliculas, int id_buscar)
{
    int posicion = -1;
    int i = 0;

    while (i < total_peliculas && posicion == -1)
    {
        if (catalogo[i].activo && catalogo[i].id == id_buscar)
            posicion = i;
        i++;
    }

    return posicion;
}

void listarPeliculasPrincipio(Pelicula catalogo[], int total_peliculas)
{
    int peliculas_activas = 0;

    printf("\n=======================================================\n");
    printf("                  PELICULAS ACTIVAS\n");
    printf("=======================================================\n");
    printf("%-40s %-25s \n", "Titulo", "Genero");
    printf("-------------------------------------------------------\n");

    for (int i = 0; i < total_peliculas; i++)
    {
        if (catalogo[i].activo)
        {
            printf("%-40s %-25s \n", catalogo[i].titulo, catalogo[i].genero);
            peliculas_activas++;
        }
    }
}

float calcular_precio_asiento(int numero_asiento)
{
    float precio_calculado;

    // Obtenemos la fila matemática (0 a 4) igual que en el resto de tu programa
    int fila = (numero_asiento - 1) / COLUMNAS;

    // Asignamos el precio según la fila manteniendo un solo 'return'
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