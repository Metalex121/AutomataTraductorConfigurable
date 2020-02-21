#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define MAX_TRANSICIONES 10
#define MAX_ESTADOS 20
#define MAX_SALIDA 5

typedef struct
{
    char k; // simbolo del alfabeto que produce la transicion
    char salida[MAX_SALIDA]; // traduccion generada por la transicion
    int estadoDestino; // estado al que se transita mediante la transicion
}TransicionAFD;

typedef struct
{
    int identificador; // identificador unico para cada estado
    int numeroTransiciones; // transiciones existentes
    TransicionAFD transiciones[MAX_TRANSICIONES]; // array que almacena las transiciones posibles
    bool estadoFinal; // True-> estado final  False -> no final
}EstadoAFD;

typedef struct
{
    int estadoInicial; // identificador estado inicial
    int estadoActual; // identificador estado actual
    int numeroEstados; // numero de estados del automata
    EstadoAFD* estados[MAX_ESTADOS]; // conjunto de estados
}AFD;

AFD* crearAFD() // constructor de automatas que devuelve un puntero al objeto creado e inicializa sus atributos
{
    AFD* afd = (AFD*)malloc(sizeof(AFD));// se reserva espacio en memoria para el automata y se obtiene su puntero
    afd -> numeroEstados = 0;
    afd -> estadoInicial = 0;
    afd -> estadoActual = 0;
    return afd;
}

void addEstadoAFD(EstadoAFD* nuevoEstado, AFD* afd) // añadir estado a un automata
{
    nuevoEstado ->identificador = afd ->numeroEstados;
    afd ->estados[afd ->numeroEstados] = nuevoEstado;
    afd ->numeroEstados++;
}

void addTransicion(AFD* afd,  int estadoOrigen, int estadoDestino, char simbolo, char* salida) // añadir transicion a un estado
{
    EstadoAFD* estado = afd -> estados[estadoOrigen];
    estado ->transiciones[estado ->numeroTransiciones].estadoDestino = estadoDestino;
    estado ->transiciones[estado ->numeroTransiciones] .k = simbolo;
    strcpy(estado ->transiciones[estado ->numeroTransiciones].salida, salida );
    estado ->numeroTransiciones++;

}

EstadoAFD* crearEstado(bool tipo) // crear estado e inicializar sus atributos
{
    EstadoAFD* nuevoEstado = (EstadoAFD*)malloc(sizeof(EstadoAFD));
    nuevoEstado ->numeroTransiciones = 0;
    nuevoEstado ->identificador = -1;
    nuevoEstado ->estadoFinal = tipo;
    return nuevoEstado;
}

char* procesarSimbolo(AFD* afd, char simbolo,char* traduccion) // procesar simbolo de una palabra, realizar transición en el automata y devolver la traducción
{
    EstadoAFD* estadoActual = afd ->estados[afd ->estadoActual];
    if(estadoActual->numeroTransiciones != 0)
    {
        for (int i = 0; i < estadoActual->numeroTransiciones; i++)
        {
            if (estadoActual->transiciones[i].k == simbolo)
            {
                strcpy(traduccion,estadoActual->transiciones[i].salida);
                afd->estadoActual = estadoActual->transiciones[i].estadoDestino;
            };
        };
    }
    return traduccion;
}

void resetearAutomata(AFD* afd)
{
    afd->estadoActual = afd->estadoInicial;
}

char* trim(char* str)// limpiar string
{
    while(*str == '\t'|| *str == ' '||*str == '\n')
    {
        str++;
    }
    char* aux = str;
    while(*aux) aux++;
    aux--;
    while(*aux == '\t'|| *aux == ' '||*aux == '\n')
    {
        aux--;
    }

    aux++;
    *aux = 0;
    return str;
}

int main()
{

    //Variables auxiliares
    FILE *config;
    char reglas[256]= "",aux[256]= "",salida[256]= "",buffaux[256];
    char *paux,*paux2,*ptr1;
    int i=0,j=0,k=0;

    //Variables del automata
    AFD* afd = crearAFD();
    EstadoAFD* estadoAux;
    char tipo, simbolo;
    char traduccion[256];
    int estadoDestino;

    //Primera etapa, procesado de la configuración del automata

    config = fopen("C:\\Users\\metal\\CLionProjects\\Pratica1TLP\\Confg_Automatas\\Automata2.txt", "r");
    if ( config == NULL )
    {
        printf( "No se ha encontrado dicho fichero" ) ;
        return 1;
    }

    fgets(aux,256,config); // se ignora la primera linea de instrucciones

    while(fgets(reglas,256,config) != NULL)
    {
        paux = strtok_r(reglas," ",&ptr1); // se particiona la linea leida del fichero de texto
        while (paux != NULL)
        {
            switch (i)
            {
                case 0: break; // Primer campo: el identificador de estado no importa ya que se asigna de forma secuencial, sin embargo se incluye en el fichero por comodidad del usuario

                case 1: // Segundo campo: tipo de estado Final o No final
                {
                    if(*paux == 'F')
                        tipo = true;//final
                    else
                        tipo = false;// no final
                    estadoAux = crearEstado(tipo);// se crea el objeto y se añade al automata
                    addEstadoAFD(estadoAux,afd);
                    break;
                }

                default: // tercer campo: transiciones, se particiona de nuevo cada una de ellas y se procesan

                {
                    char *ptr2;

                    if (*paux == 'E')
                    {
                        simbolo = 'E';
                    }
                    else
                    {
                        strcpy(buffaux,paux);
                        paux2 = strtok_r(buffaux, "/",&ptr2);
                        while (paux2 != NULL)
                        {
                            switch (k)
                            {
                                case 0: // simbolo de transicion
                                {
                                    simbolo = *paux2;
                                    break;
                                }
                                case 1: // estado destino, convertido desde un char
                                {
                                    estadoDestino = (int) *paux2 - 48;
                                    break;
                                }
                                case 2: // traduccion de transicion
                                {
                                    if(*paux2 != 'E')// se verifica si la traduccion es la palabra vacia
                                    {
                                        strcpy(traduccion, paux2);
                                        trim(traduccion);
                                    } else
                                        strcpy(traduccion,"");

                                    break;
                                }
                            }
                            paux2 = strtok_r(NULL, "/",&ptr2); // se actualiza  el token
                            k++; // contador de campos por transicion
                        };
                        if(simbolo != 'E')
                            addTransicion(afd,j,estadoDestino,simbolo,traduccion);// se añade cada transición al estado
                        k = 0;
                        break;
                    }
                }
            }
            paux = strtok_r (NULL, " ",&ptr1); // se actualiza  el token
            i++; // contador de campos por linea
        };
        i=0;
        j++; // contador de lineas y debido a que  estas estan ordenadas, de estados.
    };

    //Segunda etapa, procesado de palabras

    FILE *listaPalabras;
    char palabras[256] = "",buffer[256] = "";

    listaPalabras = fopen("C:\\Users\\metal\\CLionProjects\\Pratica1TLP\\Listas_Palabras\\Palabras1.txt", "r");
    if ( listaPalabras == NULL )
    {
        printf( "No se ha encontrado dicho fichero" ) ;
        return 1;
    }

    while(fgets(palabras,256,listaPalabras) != NULL)
    {
        i=0;
        while(palabras[i] != '\0')
        {
            strcat(salida,procesarSimbolo(afd,palabras[i],buffer));
            memset(buffer,0,256); // se limpia el buffer de almacenamiento de traducción
            i++;
        };
        if(afd->estados[afd->estadoActual]->estadoFinal)
        {
            printf("Palabra %s aceptada con traduccion: %s\n",trim(palabras),salida);
        }else
        {
            printf("Palabra %s no aceptada\n",trim(palabras));
        }
        memset(salida,0,256);//se limpia el buffer de almacenamiento de palabra de salida
        resetearAutomata(afd); // se resetea el automata para procesar una nueva palabra

    };

};