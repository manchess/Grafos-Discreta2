#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned int u32;
typedef struct GrafoSt *Grafo;
typedef struct DuplaSt *Dupla;
typedef struct TriplaSt *Tripla;
typedef struct QueueSt *Queue;

#define BUFFSIZE 80
#define UINTMAX 4294967295

#define RED     "\x1b[91m"
#define GREEN   "\x1b[92m"
#define YELLOW  "\x1b[93m"
#define BLUE    "\x1b[94m"
#define MAGENTA "\x1b[95m"
#define CYAN    "\x1b[96m"
#define RESET   "\x1b[0m"

// 4 - Funciones De Construccion/Destruccion/Copia del grafo

Grafo ConstruccionDelGrafo();
void DestruccionDelGrafo(Grafo G);
Grafo CopiarGrafo(Grafo G);


// 5 - Funciones de coloreo

u32 Greedy(Grafo G);
int Bipartito(Grafo G);


// 6 - Funciones para extraer informacion de datos del grafo

u32 NumeroDeVertices(Grafo G);
u32 NumeroDeLados(Grafo G);
u32 NumeroDeColores(Grafo G);


// 7 - Funciones de los vertices

u32 NombreDelVertice(Grafo G, u32 i);
u32 ColorDelVertice(Grafo G, u32 i);
u32 GradoDelVertice(Grafo G, u32 i);
u32 ColorJotaesimoVecino(Grafo G, u32 i,u32 j);
u32 NombreJotaesimoVecino(Grafo G, u32 i,u32 j);


// 8 - Funciones de ordenacion

char OrdenNatural(Grafo G);
char OrdenWelshPowell(Grafo G);
char SwitchVertices(Grafo G, u32 i, u32 j);
char RMBCnormal(Grafo G);
char RMBCrevierte(Grafo G);
char RMBCchicogrande(Grafo G);
char SwitchColores(Grafo G, u32 i, u32 j);

// Extras
int sortDupla(const void * _a, const void * _b);
int sortOrdenWelshPowell(const void * _a, const void * _b);
int sortRMBCnormal(const void * _a, const void * _b);
int sortRMBCrevierte(const void * _a, const void * _b);
int sortRMBCchicogrande(const void * _a, const void * _b);
u32 busquedaBinaria(u32 vNombre[], u32 inicio, u32 final, u32 key);
void liberar (void * a);

// Cola
Queue QueueCreate(Queue q, u32 n);
void QueueDestroy(Queue q);
int QueuePeek(Queue q);
bool QueueIsEmpty(Queue q);
bool QueueIsFull(Queue q);
int QueueSize(Queue q);
Queue QueueInsert(Queue q, u32 data);
u32 QueueRemoveData(Queue q);
