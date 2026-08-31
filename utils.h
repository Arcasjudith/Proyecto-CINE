#include <stdio.h>
#include <stdlib.h> // Necesario para system()
#include <ctype.h>// Para evaluar cadenas 
#include <stdbool.h> // Para usar el tipo de dato bool
#include <string.h>
#include <unistd.h>

typedef char cadena[150]; // Para tratar a los arrays de chars como 'cadena'

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
void limpiarPantalla();
bool esPar(int numero);
bool esVocal(char letra);
void imprimirSeparador();
char aMayuscula(char letra);
void imprimirCaracteres(char caracter, int cantidad);
int obtenerMayor(int num1, int num2);
int obtenerMenor(int num1, int num2);
int generarAleatorio(int min, int max);
bool esPrimo(int numero);
bool esMultiplo(int numero, int divisor);
int obtenerResto(int dividendo, int divisor);
int cantidadDivisores(int numero);
bool esPerfecto(int numero);
void LimpiarBuffer();




/////////////////////////  IMPLEMENTACIONES  //////////////////////////////


int leerEntero(cadena mensaje) {
    int numero;
    printf("%s: ", mensaje);
    fflush(stdin);
    scanf("%d", &numero);
    return numero;
}
float leerFloat(cadena mensaje) {
    float numero;
    printf("%s: ", mensaje);
    fflush(stdin);
    scanf("%f", &numero);
    return numero;
}

char leerCaracter(cadena mensaje) { 
    char caracter;
    printf("%s: ", mensaje);
    fflush(stdin);
    scanf(" %c", &caracter);
    return caracter;
}

int leerEnteroEntre(int valorMin, int valorMax, cadena mensaje) {
    int numero;
    printf("%s:", mensaje);
    scanf("%d", &numero);
    while(numero<valorMin || numero>valorMax){
        printf("Error.");
        printf("El numero ingresado esta fuera de rango.\n");
        printf("vuelve a intentarlo.\n");
        printf("%s:", mensaje);
        scanf("%d", &numero);
    }
    return numero;
}

float leerFloatEntre(float valorMin, float valorMax, cadena mensaje) {
    float numero;
    printf("%s:", mensaje);
    scanf("%f", &numero);
    while(numero<=valorMin || numero>=valorMax){
        printf("Error.");
        printf("El numero ingresado esta fuera de rango.\n");
        printf("vuelve a intentarlo.\n");
        printf("%s:", mensaje);
        scanf("%f", &numero);
    }
    return numero;
}

bool confirmaUsuario(cadena mensaje) {
    char opcion;
    printf("%s:", mensaje);
    printf("[S/N]");
    scanf(" %c", &opcion);
    return opcion=='S' || opcion=='s'; 
}
float calcularPromedio(float sumaTotal, int cantidadElementos) {
    return sumaTotal / cantidadElementos;
}
float aplicarPorcentaje(float valorTotal, float porcentaje) {
    return (valorTotal * porcentaje) / 100.0;
}
float calcularPorcentaje(int cantidad, int cantidadTotal) {
    float porcentaje = (cantidad / cantidadTotal) * 100;    
    return porcentaje;
}
void limpiarPantalla() {
    system("cls");
}
bool esPar(int numero) {
    return (numero % 2 == 0);
}
bool esVocal(char letra) {
    letra = tolower(letra); // La pasa a minúscula temporalmente
    return (letra == 'a' || letra == 'e' || letra == 'i' || letra == 'o' || letra == 'u');
}
char aMayuscula(char letra) {
    // char tipo = aMayuscula(leerCaracter("Ingrese tipo (A/B/C)"));
    // if (tipo == 'A') { ... } // ¡Solo evaluás una vez!
    return toupper(letra);
}
void imprimirCaracteres(char caracter, int cantidad) {
    for (int i = 0; i < cantidad; i++) {
        printf("%c", caracter);
    }
}
int obtenerMayor(int num1, int num2) {
    int mayor;
    if (num1 > num2) {
        mayor = num1;
    }else mayor =num2;
    return mayor;
}

int obtenerMenor(int num1, int num2) {
    int menor;
    if (num1 < num2) {
        menor = num1;
    }else menor =num2;
    return menor;
}
int generarAleatorio(int min, int max) {
    return rand() % (max - min + 1) + min;
}
int obtenerResto(int dividendo, int divisor) {
    return dividendo % divisor;
}
int cantidadDivisores(int numero) {
    int contador = 0;
    for (int i = 1; i <= numero; i++) {
        if (numero % i == 0) {
            contador++;
        }
    }
    return contador;
}
// Función auxiliar para limpiar el buffer
void LimpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void PantallaDeEspera() {
    sleep(4); 
    system("clear"); 
}