#include "utils.h"
#define FILAS 5
#define COLUMNAS 10
void Tablero_Asientos(int fila, int columna, char Tablero[FILAS][COLUMNAS], char Tablero_visible[FILAS][COLUMNAS]);
bool Asignacion_Asientos(int fila, int columna, char Tablero[FILAS][COLUMNAS], char Tablero_visible[FILAS][COLUMNAS]);
void Mostar_Asientos(int fila, int columna, char Tablero[FILAS][COLUMNAS]);

// Esta funcion crea el tablero Tablero_Asientos lleno de "o" <- ese simbolo representara los asientos disponibles.
void Tablero_Asientos(int fila, int columna, char Tablero[FILAS][COLUMNAS], char Tablero_visible[FILAS][COLUMNAS])
{
    int i, j;
    for (i = 0; i < fila; i++)
    {
        for (j = 0; j < columna; j++)
        {

            Tablero[i][j] = 'x';
            Tablero_visible[i][j] = 'o';
            /* code */
        }
    }
}
// Esta funcion asigna los asientos a ocupar por el usuario
bool Asignacion_Asientos(int fila, int columna, char Tablero[FILAS][COLUMNAS], char Tablero_visible[FILAS][COLUMNAS])
{
    int contador_Asientos = 0;
  
    
        if ((fila >= 0 && fila < FILAS) && (columna >= 0 && columna < COLUMNAS))
        {
            if (Tablero_visible[fila][columna] == 'o')
            {
                Tablero_visible[fila][columna]= Tablero[fila][columna];
                printf("Asiento asignado con exito\n");
                contador_Asientos++;
                 return  false;
            }else
            {
                printf("Asiento ocupado\n");
                return  false;
            }
            
            
              
        }else
        {
            printf("Asiento Inexistente\n");
            return false;
        }
        
        
    
}

// Funcion muestra el tablero junto con 2 indices
// EL segnunod indice las filas se tiene q mostra como letras desde la a a la  z sea el caso q sea ;
void Mostar_Asientos(int fila, int columna, char Tablero[FILAS][COLUMNAS])
{
    int i, j;
    columna = COLUMNAS;
    
   
    printf("     ");
    //EL indice de columnas se altera de forma q se muestren secuencialmente  todas las letras  desde la "A" hasta
    //la cantidad de columnas existenetes 
    for (char Abecedario = 'A';Abecedario < ('A'+COLUMNAS);Abecedario++)
    {
        printf("%c  ", Abecedario);
    }
    printf("\n");

    for (i = 0; i < fila; i++)
    {
        printf("%d | ", i+1);
        for (j = 0; j < columna; j++)
        {
            printf(" %c ", Tablero[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main()
{
    int filaseleccionada; 
    int columnaseleccionada; 
    char Letra ='A'; // Variable provicional par las columnas ;
    char Tablero[FILAS][COLUMNAS];
    char Tablero_visible[FILAS][COLUMNAS];
    bool Proceso_Seleccion = true;
    bool asientoAsignadoCorrectamente = false;

    printf("\t================================\n");
    printf("\t         ASIENTOS\n");
    printf("\t================================\n");
    printf("Tablero: %dx%d\n", FILAS, COLUMNAS);
    printf("| 'o' => asiento libre || 'x' => Asiento ocupado |\n\n");

    // CREACION DEL TABLERO.
    Tablero_Asientos(FILAS, COLUMNAS, Tablero, Tablero_visible);
    Mostar_Asientos(FILAS, COLUMNAS, Tablero_visible);

    // ASIGNACION DE ASIENTOS.
  
    while (Proceso_Seleccion)// De momento el ciclo se va activar de esta forma con un bool pero a futuro cuando tenga la variable de "VOLETOS"
                            // El ciclo se repetira segun cunatos voletos se compre
    {
        while (!asientoAsignadoCorrectamente)
        {
            filaseleccionada = leerEntero("Ingres una fila (1-5)") -1 ;// se le rest 1 para que no se salga del rango de las filas al mostrarse el vector
            //--------------------
            // aqui se hace el pasaje de letra a numero con el ascii;
            printf("Ingrese una columna [A-Z]");
            scanf(" %c",&Letra);
            Letra = toupper(Letra);
            columnaseleccionada = Letra -'A';// este calculo se utiliza pera q las columnas no se salgan de rango ;
    
    
           asientoAsignadoCorrectamente = Asignacion_Asientos(filaseleccionada, columnaseleccionada, Tablero,Tablero_visible);
           Mostar_Asientos(FILAS,COLUMNAS,Tablero_visible);
            /* code */
        }
        

        
       Proceso_Seleccion = false;
    }
    Mostar_Asientos(FILAS,COLUMNAS,Tablero_visible);
    
    return 0;
}