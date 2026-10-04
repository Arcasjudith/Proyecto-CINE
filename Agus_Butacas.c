#include "utils.h"
#define FILAS 5
#define COLUMNAS 10
#define Boleto_MIn 0
#define Boleto_Max 50
/*Aver q es esto */
#ifdef _WIN32 // _WIN32 es la condicion del if quie decir en caso de q el sistema operativo sea windous hacer lo siguiente .
                // ifdef  & endif son comandos especiales para el compilador , no ocupan espacio de memoria son efimeras 

#include <windows.h>// Libreria de windous ,|HANDLE ->es un tipo de dato que fucniona como Identificador de recursos (Puntero a un elemneto interno)|
                                          //|DWORD -> es un tipo de dato q equivalea 32 bits osea 32 espacios q pueden almacenar 1 o 0 y sirve para activar o desactivar funciones de ANSI
                                          //|getStdHandle()-> es la forma en la se obtiene la pantalla de muestra (la temrinal)
                                          //|STD_OUTPUT_HANDLE -> te da el identificador para q la terminaliterprete los colores en este caso RGB
void habilitarModoANSI() // Habilita la interpretacion de colores RGB de la terminal para Windows 
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE)
    {
        DWORD dwMode = 0; // Guardará la configuración actual de la consola
        GetConsoleMode(hOut, &dwMode);//  Leemos cómo está configurada la consola ahora mismo y lo guardamos en 'dwMode'
        dwMode |= 0x0004; // Activamos el modo "Terminal Virtual" (0x0004),Esto permite que la consola entienda códigos ANSI (para colores, mover cursor, etc.)
        SetConsoleMode(hOut, dwMode); // Aplicaa la nueva configuración modificada a la consola
    }
}
#endif
//--------------------------------------------------------------
typedef struct// Estrucutra que representa los colores en RGB 
{
    int a;
    int b;
    int c;

} Color;
//--------------------------------------------------------------------------------------------------
void Tablero_Asientos_RGB(Color *lugares, int fila, int columna);// Crea la matriz de Asientos 
//--------------------------------------------------------------------------------------------------
void Mostrar_Matriz_RGB(Color *a, int fila, int columnas);// Muestra la Matriz de asientos 
// ------------------------------------------------------------------------------------------------------------
int Busqueda_Asientos();  // Realiza una busqueda (binarai/dicotomica) del asiento seleccionado por el usuario y devuelde la posicion de esa butaca / asiento.
//  ------------------------------------------------------------------------------------------------------------
int Validacion_asientos(int min, int max, cadena mensaje); // valida que el usuario no ingrese nuemros fuera de rango o caracteres de otro tipo , (letras o signos etc)
// NUeva fucion para acortar el codigo de la funcion antes llamada Asignacion de asientos
void Asignacion_asientos(int Posicion_Asiento, Color *lugares, int *contador);// Verifica q el asiento este ocupado en caso de estar libre le cambia el color a rojo y le resta  1 al contador de asientos .

int main()
{
#ifdef _WIN32 
    habilitarModoANSI();
#endif
    Color Butacas[FILAS][COLUMNAS];
    Tablero_Asientos_RGB(&Butacas[0][0], FILAS, COLUMNAS);
    Mostrar_Matriz_RGB(&Butacas[0][0], FILAS, COLUMNAS);
    Color Asiento_Reservado = {255, 0, 0};

    int Can_Asientos = 1;
    int contador_Asientos = Boleto_Max;
    int Posicion_Asiento = 0;

    while (Can_Asientos != 0)
    {
        printf("\t================================\n");
        printf("\t         ASIENTOS\n");
        printf("\t================================\n");
        printf("Asientos: %d\n", Boleto_Max);
        printf("| 'Numero' => asiento libre || \t \033[48;2;%d;%d;%dm \033[0m <= Asiento Ocupado|\n\n", Asiento_Reservado.a, Asiento_Reservado.b, Asiento_Reservado.c);
        printf(" INgrese |0 | para salir \n");
        printf("Cantidad de asientos Libres => |%d |\n", contador_Asientos); // aqui el contador de asientos se modifica utilizando punteros

        Mostrar_Matriz_RGB(&Butacas[0][0], FILAS, COLUMNAS);                                                    // esta linea despue se borra
        Can_Asientos = Validacion_asientos(Boleto_MIn, Boleto_Max, "Ingrese Cuantos Asientos va a comparar\n"); // Verificar q pasa si el usuario ingresa un 0 o numeors negativos ?
        limpiarPantalla();
        Mostrar_Matriz_RGB(&Butacas[0][0], FILAS, COLUMNAS);
        for (int i = 0; i < Can_Asientos; i++)
        {
            
            Posicion_Asiento = Busqueda_Asientos();
            Asignacion_asientos(Posicion_Asiento, (Color *)Butacas, &contador_Asientos);
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

int Busqueda_Asientos()
{
    // Implementaremos la busqueda binaria / dicotomica
    int op = 0;                                             //  posicion q buscamos
    int inzquierada = Boleto_MIn, Derecha = Boleto_Max - 1; // SOn ls extremos de la matriz
    int posicion_Media;                                     // seria el centro de la "matriz"
    int vandera = -1;                                       // esta variable cumple una doble funcion es una bandera q indica cuando cortar el bucle while y tambien almacena la poscion que se va a alterar;
    

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
 return vandera;
}
//---------------------------------------------------------------------------------------------------------------
void Asignacion_asientos(int Posicion_Asiento, Color *lugares, int *contador)
{
    int pos_asiento = 0;
    Color Asiento_Reservado = {255, 0, 0};
    Color *aux = lugares + Posicion_Asiento;
    if (aux->a == Asiento_Reservado.a && aux->b == Asiento_Reservado.b && aux->c == Asiento_Reservado.c)//Verifica si el asiento esta pintaod de color rojo o noup.
    {
        printf("Error , el asieto seleccionado esta ocupado , Eliga otro ");
        pos_asiento = Busqueda_Asientos(lugares);
        Asignacion_asientos( pos_asiento,  lugares, contador);
        
    }
    else
    {
        *aux = Asiento_Reservado; // una ves encontrado el valor se altera utilizando punteros ;
        (*contador)--;            // se le resta 1 al numero total de asientos disponibles;
    }
}
//------------------------------------------------------------------------------------------------------------------
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