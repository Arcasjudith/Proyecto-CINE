#include "utils.h"
#define FILAS 5
#define COLUMNAS 10
#define Boleto_MIn 0
#define Boleto_Max 50
/*Aver q es esto */
#ifdef _WIN32
#include <windows.h>
void habilitarModoANSI()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE)
    {
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= 0x0004; // ENABLE\_VIRTUAL\_TERMINAL\_PROCESSING
        SetConsoleMode(hOut, dwMode);
    }
}
#endif

// Dado q ahora tengo reacer el codigo con estructuras tenemos q adaptar un poco el codigo anteriror 
/* La idea ahroa es dividir la matris de asientos en 3 colores q representen el nivel de costo de cado uno los colores son
 rosa pastel , Amarillo patito y al verde tenis, y */
// Primero defino la estrucutura Colores  q usare para representar los asientos

typedef struct
{
    int a;
    int b;
    int c;
    
} Color;
/* ahora tengo q crear la mtriz de  colores */
void Tablero_Asientos_RGB(Color *lugares, int fila, int columna);
// esta fucnion umestra la matriz con los colores
void Mostrar_Matriz_RGB(Color *a, int fila, int columnas);
// FUncion Busqueda ASientos :
// 
void Busqueda_Asientos(Color *lugares, int *contardor); // Capas y los parametros de fila y coiumna no son necesarios
// FUncion Contar asientos
//[  ] Cabiar el pasaje por referencia en la funcion de contador de lugares
int Validacion_asientos(int min, int max, cadena mensaje);
// NUeva fucion para cortar el codigo de la funcion antes llamada Asignacion de asientos 


int main()
{
#ifdef _WIN32
    habilitarModoANSI();
#endif
    Color Butacas[FILAS][COLUMNAS];
    Tablero_Asientos_RGB(&Butacas[0][0], FILAS, COLUMNAS);
    Mostrar_Matriz_RGB(&Butacas[0][0], FILAS, COLUMNAS);
    Color Asiento_Reservado = {255,0,0};

    int Can_Asientos = 1, contador_Asientos = Boleto_Max;

    while (Can_Asientos != 0)
    {
        printf("\t================================\n");
        printf("\t         ASIENTOS\n");
        printf("\t================================\n");
        printf("Asientos: %d\n", Boleto_Max);
        printf("| 'Numero' => asiento libre || \t \033[48;2;%d;%d;%dm \033[0m <= Asiento Ocupado|\n\n",Asiento_Reservado.a,Asiento_Reservado.b,Asiento_Reservado.c);
        printf(" INgrese |0 | para salir \n");
        printf("Cantidad de asientos Libres => |%d |\n", contador_Asientos); // aqui el contador de asientos se modifica utilizando punteros

        Mostrar_Matriz_RGB(&Butacas[0][0], FILAS, COLUMNAS);                                                    // esta linea despue se borra
        Can_Asientos = Validacion_asientos(Boleto_MIn, Boleto_Max, "Ingrese Cuantos Asientos va a comparar\n"); // Verificar q pasa si el usuario ingresa un 0 o numeors negativos ?
        limpiarPantalla();
        Mostrar_Matriz_RGB(&Butacas[0][0], FILAS, COLUMNAS);
        for (int i = 0; i < Can_Asientos; i++)
        {
            Busqueda_Asientos((Color *)Butacas, &contador_Asientos);

            limpiarPantalla();
            Mostrar_Matriz_RGB(&Butacas[0][0], FILAS, COLUMNAS);
        }

        limpiarPantalla();
    }

    return 0;
}
void Tablero_Asientos_RGB(Color *lugares, int fila, int columna)
{
    // Necesito variables auxiliares para asignarles un color
    Color roza_pastel = {251, 124, 155};
    Color amarillo_patito = {247, 231, 84};
    Color verde_tenis = {121, 240, 24};

    for (int i = 0; i < fila; i++)
    {
        for (int j = 0; j < columna; j++)
        {
            // Variable auxiliar
            Color *aux = lugares + (i * columna) + j;
            if (i < 1)
            {
                *aux = verde_tenis;
            }
            else if (i < 3)
            {
                *aux = amarillo_patito;
            }
            else
            {
                *aux = roza_pastel;
            }
        }
    }
}
void Mostrar_Matriz_RGB(Color *a, int filas, int columnas)
{

    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            // Variable auxiliar
            Color *aux = a + (i * columnas) + j;
            printf("\t\033[48;2;%d;%d;%dm \033[30m|%.2d|\033[0m", aux->a, aux->b, aux->c, ((i * columnas) + j + 1));
        }
        printf("|");
        printf("\n\n");
    }
    printf("\n");
}
void Busqueda_Asientos(Color *lugares, int *contador)
{
    // Implementaremos la busqueda binaria / dicotomica
    int op = 0;                                             //  posicion q buscamos
    int inzquierada = Boleto_MIn, Derecha = Boleto_Max - 1; // SOn ls extremos de la matriz
    int posicion_Media;                                     // seria el centro de la "matriz"
    int vandera = -1;                                       // esta variable cumple una doble funcion es una bandera q indica cuando cortar el bucle while y tambien almacena la poscion que se va a alterar;
    Color Asiento_Reservado = {255,0,0};


    op = Validacion_asientos(Boleto_MIn, Boleto_Max, "Eliga su asiento \n");
    while (inzquierada <= Derecha && vandera == -1) //  Se repite miestras el rango de  izquiera(valor minimo "0") a derecha (Valor maximo (50)) sea validao ,y no se halla encontrado la posicion
    {
        posicion_Media = (inzquierada + Derecha) / 2;     //  calcula la posicion media del vector ejm; (0+50)/2 = 25=> posicion media
        int numero_asiento_original = posicion_Media + 1; /*Esta linea impide q se rompa lafucnion en caso de q el usuario eliga el asiento 25*/
        if (op == numero_asiento_original)                // verifica si la posicion media osea el lugar / asineto 25 es justo la eleccion del usuario
        {
            vandera = posicion_Media; // en caso de q si sea la opcion del usuario cambia el valor de la vandera y sale del ciclo
        }
        else
        {
            if (op < numero_asiento_original) // en caso de q no compara si la opcion del usuario es de un valor menor para vusar hacia la mitad "inferior" de la matriz
            {
                Derecha = posicion_Media - 1;
            }
            else
            {
                inzquierada = posicion_Media + 1; // en caso de q sea mayor busca en la mitad "superiror";
            }
        }
    }

    if (vandera != -1)
    {
        Color *aux = lugares + vandera ;
        if ( aux->a == Asiento_Reservado.a && aux->b == Asiento_Reservado.b && aux->c == Asiento_Reservado.c)
        { 
           printf("Error , el asieto seleccionado esta ocupado , Eliga otro ");
           Busqueda_Asientos(lugares,contador );
        }
        else
        {
            *aux = Asiento_Reservado; // una ves encontrado el valor se altera utilizando punteros ;
            (*contador)--;            // se le resta 1 al numero total de asientos disponibles;
        }
    }
    /**else
    {
        printf("El lugar seleccionado => |%d| no existe \n",vandera); // en caso de no ser encontrado sale este mensaje ;
    }*/
}
int Validacion_asientos(int min, int max, cadena mensaje)
{

    int ingreso_Valido = 0;
    int op = 0;

    do
    {
        printf("%s", mensaje);

        // Si sop 1, significa que leyó un número correctamente
        if (scanf("%d", &op) == 1)
        {
            // Verificamos si eesta dentro del rango
            if (op >= min && op <= max)
            {
                ingreso_Valido = 1; //  Es un número y está en rango. Rompe el bucle.
            }
            else
            {
                printf("Error: Numero invalido . Intente de nuevo dentro del rango de [%d-%d].\n\n", min, max);
            }
        }
        else
        {

            printf("Error: Entrada invalida. Ingrese un Numero entero.\n\n");
            LimpiarBuffer(); // Limpiamos la letra del buffer para que no se trabe
        }

        /* code */
    } while (ingreso_Valido == 0);
    return op;
}