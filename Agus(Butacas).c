#include "utils.h"
#define FILAS 5
#define COLUMNAS 10
#define Boleto_MIn 0
#define Boleto_Max 50

// Funcion Tablero asientos crea la matriz incial.
void Tablero_Asientos(int lugares[][COLUMNAS]); // esta fucion si se requiere y es mas eficiente puede no usar punteros par ala creacion de tableros y ser simplemente una matriz comun
// Esta funcion Muestra la matriz
void Mostrar_Tablero_ASientos(int *lugares, int Indice1, int indice2); // para esta funcion si se tiene q usar punteros
// FUncion Asignacion ASientos :
void ASignacion_Asientos(int *lugares, int *contardor); // Capas y los parametros de fila y coiumna no son necesarios
// FUncion Contar asientos
int Validacion_asientos(int min, int max, cadena mensaje);

int main()
{
    int Asientos[FILAS][COLUMNAS];
    int Can_Asientos = 1, contador_Asientos = Boleto_Max;
    Tablero_Asientos(Asientos);
    while (Can_Asientos != 0)
    {
        printf("\t================================\n");
        printf("\t         ASIENTOS\n");
        printf("\t================================\n");
        printf("Asientos: %d\n", FILAS * COLUMNAS);
        printf("| 'Numero' => asiento libre || '0' => Asiento ocupado |\n\n");
        printf(" INgrese |0 | para salir \n");
        printf("Cantidad de asientos Libres => |%d |\n", contador_Asientos); // aqui el contador de asientos se modifica utilizando punteros

        Mostrar_Tablero_ASientos((int *)Asientos, FILAS, COLUMNAS);                                             // esta linea despue se borra
        Can_Asientos = Validacion_asientos(Boleto_MIn, Boleto_Max, "Ingrese Cuantos Asientos va a comparar\n"); // Verificar q pasa si el usuario ingresa un 0 o numeors negativos ?
        limpiarPantalla();
        Mostrar_Tablero_ASientos((int *)Asientos, FILAS, COLUMNAS);
        for (int i = 0; i < Can_Asientos; i++)
        {
            ASignacion_Asientos((int *)Asientos, &contador_Asientos);

            limpiarPantalla();
            Mostrar_Tablero_ASientos((int *)Asientos, FILAS, COLUMNAS);
        }

        limpiarPantalla();
    }

    return 0;
}
void Tablero_Asientos(int lugares[][COLUMNAS])
{

    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            lugares[i][j] = (i * COLUMNAS) + j + 1;
            /* code */
        }
    }
}
void Mostrar_Tablero_ASientos(int *lugares, int Indice1, int indice2)
{

    // EL indice de columnas se altera de forma q se muestren secuencialmente  todas las letras  desde la "A" hasta
    // la cantidad de columnas existenetes

    for (int i = 0; i < Indice1; i++)
    {
        for (int j = 0; j < indice2; j++)
        {
            printf(" %2d ", *(lugares + (i * indice2) + j));
        }
        printf("\n");
    }
}
void ASignacion_Asientos(int *lugares, int *contador)
{
    // Implementaremos la busqueda binaria / dicotomica
    int op = 0;                                         //  posicion q buscamos
    int inzquierada = Boleto_MIn, Derecha = Boleto_Max -1 ; // SOn ls extremos de la matriz
    int posicion_Media;                                 // seria el centro de la "matriz"
    int vandera = -1;                                   // esta variable cumple una doble funcion es una bandera q indica cuando cortar el bucle while y tambien almacena la poscion que se va a alterar;


    op = Validacion_asientos(Boleto_MIn, Boleto_Max, "Eliga su asiento \n");
    while (inzquierada <= Derecha && vandera == -1) //  Se repite miestras el rango de  izquiera(valor minimo "0") a derecha (Valor maximo (50)) sea validao ,y no se halla encontrado la posicion
    {
        posicion_Media = (inzquierada + Derecha) / 2; //  calcula la posicion media del vector ejm; (0+50)/2 = 25=> posicion media
        int numero_asiento_original = posicion_Media + 1;/*Esta linea impide q se rompa lafucnion en caso de q el usuario eliga el asiento 25*/
        if (op == numero_asiento_original)        // verifica si la posicion media osea el lugar / asineto 25 es justo la eleccion del usuario
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
        if (*(lugares + vandera) == 0)
        {
            printf("El Lugar => |%d| esta ocupado\n", op);
        }
        else
        {
            *(lugares + vandera) = 0; // una ves encontrado el valor se altera utilizando punteros ;
            (*contador)--;            // se le resta 1 al numero total de asientos disponibles;
        }
    }
    else
    {
        printf("El lugar seleccionado => |%d| no existe \n", op); // en caso de no ser encontrado sale este mensaje ;
    }
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
