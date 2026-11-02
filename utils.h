#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif
typedef char cadena[150];

////////////////////////////  PROTOTIPOS  /////////////////////////////////
int leerEntero(cadena mensaje);
float leerFloat(cadena mensaje);
char leerCaracter(cadena mensaje);
int leerEnteroEntre(int valorMin, int valorMax, cadena mensaje);
float leerFloatEntre(float valorMin, float valorMax, cadena mensaje);
bool confirmaUsuario(cadena mensaje);
float calcularPromedio(float sumaTotal, int cantidadElementos);
float aplicarPorcentaje(float valorTotal, float porcentaje);
float calcularPorcentaje(int cantidad, int cantidadTotal);
void LimpiarBuffer();
void PantallaDeEspera(cadena mensaje);
void MostrarHorarioFormateado(int horario);
void leerCadena(cadena mensaje, cadena destino, int longitudMaxima);
void pausar();


int leerEntero(cadena mensaje)
{
    int numero;
    int validacion;

    do
    {
        printf("%s: ", mensaje);
        validacion = scanf("%d", &numero);

        if (validacion != 1)
        {
            printf("Error: Entrada invalida. Por favor, ingrese un numero.\n");
            LimpiarBuffer(); // Saca la letra o caracter erroneo del buffer
        }
    } while (validacion != 1);

    LimpiarBuffer(); // Limpia el '\n' residual para no afectar futuras lecturas de cadenas
    return numero;
}

int leerEnteroEntre(int valorMin, int valorMax, cadena mensaje)
{
    int numero;
    int valido = 0;
    printf("%s: ", mensaje);
    valido = scanf("%d", &numero);

    while ((numero < valorMin || numero > valorMax) && valido == 1)
    {
        printf("Error. El numero ingresado esta fuera de rango (%d a %d).\n", valorMin, valorMax);
        printf("Vuelve a intentarlo.\n");
        printf("%s: ", mensaje);
        scanf("%d", &numero);
        LimpiarBuffer();
    }
    return numero;
}

float leerFloat(cadena mensaje)
{
    float numero;
    printf("%s: ", mensaje);
    fflush(stdin);
    scanf("%f", &numero);
    return numero;
}

char leerCaracter(cadena mensaje)
{
    char caracter;
    printf("%s: ", mensaje);
    fflush(stdin);
    scanf(" %c", &caracter);
    return caracter;
}

float leerFloatEntre(float valorMin, float valorMax, cadena mensaje)
{
    float numero;
    printf("%s:", mensaje);
    scanf("%f", &numero);
    while (numero <= valorMin || numero >= valorMax)
    {
        printf("Error.");
        printf("El numero ingresado esta fuera de rango.\n");
        printf("vuelve a intentarlo.\n");
        printf("%s:", mensaje);
        scanf("%f", &numero);
    }
    return numero;
}

bool confirmaUsuario(cadena mensaje)
{
    char opcion;
    printf("%s:", mensaje);
    printf("[S/N]");
    scanf(" %c", &opcion);
    return opcion == 'S' || opcion == 's';
}

float calcularPromedio(float sumaTotal, int cantidadElementos)
{
    if (cantidadElementos == 0)
        return 0.0f;
    return sumaTotal / cantidadElementos;
}

float aplicarPorcentaje(float valorTotal, float porcentaje)
{
    return (valorTotal * porcentaje) / 100.0f;
}

float calcularPorcentaje(int cantidad, int cantidadTotal)
{
    if (cantidadTotal == 0)
        return 0.0f;
    float porcentaje = ((float)cantidad / cantidadTotal) * 100.0f;
    return porcentaje;
}

void limpiarPantalla()
{
    system("cls");
}

void LimpiarBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void PantallaDeEspera(cadena mensaje)
{
    printf(" %s", mensaje);
    limpiarPantalla();
}

void leerCadena(cadena mensaje, cadena destino, int longitudMaxima)
{
    printf("%s", mensaje);
    if (fgets(destino, longitudMaxima, stdin) != NULL)
    {
        size_t longitud = strlen(destino);
        if (longitud > 0 && destino[longitud - 1] == '\n')
        {
            destino[longitud - 1] = '\0';
        }
        else
        {
            LimpiarBuffer();
        }
    }
    else
    {
        destino[0] = '\0';
    }
}

void pausar()
{
    printf("\nPresione ENTER para continuar...");
    LimpiarBuffer();
}