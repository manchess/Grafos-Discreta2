#include "Rii.h"

struct GrafoSt
{
    u32 n;
    u32 m;
    u32 colores;
    u32 *vNombre;       // Array nombre real de los vertices
    Dupla vColor;       // Array color de los vertices
    Dupla vGrado;       // Array grado de los vertices
    u32 **vLados;       // Array de lados de los vertices
    u32 *vOrden;
};

struct DuplaSt
{
    u32 v1;
    u32 v2;
};

struct TriplaSt
{
    u32 gradoColor;
    u32 inicio;
    u32 fin;
};

struct QueueSt
{
    u32 size;
    u32 *array;
    u32 front;
    int rear;
    u32 itemCount;
};


// --------------------------------------------------------------
// 4 - Funciones De Construccion/Destruccion/Copia del grafo
// --------------------------------------------------------------
Grafo ConstruccionDelGrafo()
{
    Grafo g = NULL;
    Dupla vArray = NULL;

    char buffer[BUFFSIZE];
    u32 n, m;       // Vertices y lados
    u32 v1, v2;     // Vertices de e v1 v2
    u32 pvn;        // posicion del vertice
    u32 visitado;   // vertice visto?
    u32 limite;     // para ciclos
    u32 grado;
    u32 totVistos;
    int pos;        // lo utilizamos para la busqueda binaria

    // Ignoramos los comentarios
    do
    {
        if ( fgets(buffer, BUFFSIZE, stdin) == NULL )
        {
            return NULL;
        }
    } while(buffer[0] == 'c');


    // Obtenemos n & m --> p edge n m
    sscanf(buffer, "p edge %u %u", &n, &m);

    // Si la primera linea que no es comentario no es de la forma: p edge n m
    if (buffer[0] != 'p')
    {
        printf(GREEN "ERROR EN PRIMERA LINEA SIN COMENTARIO\n" RESET);
        return NULL;
    }

    // Arreglo de duplas
    vArray = (Dupla) malloc(2*m*sizeof(struct DuplaSt));

    // No se pudo asignar memoria para vArray
    if (!vArray)
    {
        printf(CYAN "failed memory allocation for" RED " 'vArray'" RESET);
        return NULL;
    }

    // Leemos las m lineas, 
    for(u32 i = 0; i < m; i++)
    {
        if ( fgets(buffer, BUFFSIZE, stdin) == NULL )
        {
            liberar(vArray);
            return NULL;
        }

        // Una de las m lineas que no comienza con e
        if (buffer[0] != 'e')
        {
            liberar(vArray);
            printf(GREEN "ERROR DE LECTURA EN LADO " MAGENTA "%u\n" RESET, i+1);
            return NULL;
        }

        // Obtenemos e v1 v2
        sscanf(buffer, "%*c %u %u", &v1, &v2);

        vArray[i].v1 = v1;
        vArray[i].v2 = v2;
        // Damos vuelta la duplas -> (3,1) -> (1,3)
        vArray[i+m].v1 = v2;
        vArray[i+m].v2 = v1;
    }

    // Ordenamos el arreglo de duplas
    qsort(vArray, 2*m, sizeof(struct DuplaSt), sortDupla);

/*Alocamos y liberamos memoria en caso de fallo para toda la estructura grafo*/
    g = malloc(sizeof(struct GrafoSt));

    if (!g)
    {
        liberar(vArray);
        printf(CYAN "failed memory allocation for" RED " 'g'" RESET);
        return NULL;
    }

    g->vNombre = (u32*) malloc(n*sizeof(unsigned int));

    if (!g->vNombre)
    {
        liberar(vArray);
        liberar(g);
        printf(CYAN "failed memory allocation for" RED " 'g->vNombre'" RESET);
        return NULL;
    }

    g->vColor = (Dupla) malloc(n*sizeof(struct DuplaSt));

    if (!g->vColor)
    {
        liberar(vArray);
        liberar(g->vNombre);
        liberar(g);
        printf(CYAN "failed memory allocation for" RED " 'g->vColor'" RESET);
        return NULL;
    }

    g->vGrado = (Dupla) malloc(n*sizeof(struct DuplaSt));

    if (!g->vGrado)
    {
        liberar(vArray);
        liberar(g->vNombre);
        liberar(g->vColor);
        liberar(g);
        printf(CYAN "failed memory allocation for" RED " 'g->vGrado'" RESET);
        return NULL;
    }

    g->vLados = malloc(n*sizeof(unsigned int*));

    if (!g->vLados)
    {
        liberar(vArray);
        liberar(g->vNombre);
        liberar(g->vColor);
        liberar(g->vGrado);
        liberar(g);
        printf(CYAN "failed memory allocation for" RED " 'g->vLados'" RESET);
        return NULL;
    }

    g->vOrden = malloc(n*sizeof(unsigned int));

    if (!g->vOrden)
    {
        liberar(vArray);
        liberar(g->vNombre);
        liberar(g->vColor);
        liberar(g->vGrado);
        liberar(g->vLados);
        liberar(g);
        printf(CYAN "failed memory allocation for" RED " 'g->vOrden'" RESET);
        return NULL;
    }
/*---------------------------------------------------------------------------*/


/*Metemos el nombre, color, grado y orden en sus arreglos correspondientes*/
    pvn = 0;
    visitado = vArray[0].v1;
    limite = 2*m;   // Esto para que no evalue en cada iteracion

    g->n = n;
    g->m = m;

    g->vNombre[0] = visitado;
    g->vColor[0].v2 = 0;    // 
    g->vGrado[0].v1 = 1;    // Grado
    g->vGrado[0].v2 = 0;    // Grado orden
    g->vOrden[0] = 0;
    totVistos = 1;

    for(u32 i = 1; i < limite; i++)
    {
        if (vArray[i].v1 != visitado)
        {
            visitado = vArray[i].v1;
            pvn++;
            g->vNombre[pvn] = visitado;     // Meto en el array el nombre del vertice
            g->vColor[pvn].v2 = pvn;
            g->vGrado[pvn].v1 = 1;             // 1 por su vecino v2 de la dupla
            g->vGrado[pvn].v2 = pvn;
            g->vOrden[pvn] = pvn;
            totVistos++;
        }
        else
        {
            g->vGrado[pvn].v1 += 1;
        }
    }

    if (totVistos != n)
    {
        liberar(vArray);
        liberar(g->vNombre);
        liberar(g->vColor);
        liberar(g->vGrado);
        liberar(g->vLados);
        liberar(g);
        printf(GREEN "CANTIDAD DE VERTICES LEIDOS NO ES LA DECLARADA\n" RESET);
        return NULL;
    }

    // Lados
    for(u32 i = 0; i < n; i++)
    {
        g->vLados[i] = (unsigned int*) malloc(g->vGrado[i].v1*sizeof(unsigned int));
    }

    pvn = 0;
    visitado = vArray[0].v1;
    grado = 0;
    
    pos = busquedaBinaria(g->vNombre, 0, n-1, vArray[0].v2);
    g->vLados[0][0] = pos;

    for(u32 i = 1; i < limite; i++)
    {
        if (vArray[i].v1 != visitado)
        {
            visitado = vArray[i].v1;
            grado = 0;
            pvn++;
            
        }
        else
        {
            grado++;
        }

        pos = busquedaBinaria(g->vNombre, 0, n-1, vArray[i].v2);
        g->vLados[pvn][grado] = pos;
    }

    g->colores = Greedy(g);

/*    liberamos vArray ya que no lo necesitamos mas*/
    liberar(vArray);

    return g;
}

void DestruccionDelGrafo(Grafo G)
{
    if (G == NULL)
        return;

    u32 n = NumeroDeVertices(G);

    liberar(G->vNombre);
    liberar(G->vColor);
    liberar(G->vGrado);
    liberar(G->vOrden);

    for(u32 i = 0; i < n; i++)
        liberar(G->vLados[i]);

    liberar(G->vLados);
    liberar(G);
}

Grafo CopiarGrafo(Grafo G)
{
    if (G == NULL)
        return NULL;

    u32 n = NumeroDeVertices(G);
    Grafo gp = NULL;

    gp = malloc(sizeof(struct GrafoSt));

    if (!gp)
    {
        printf(CYAN "failed memory allocation for" RED " 'gp'" RESET);
        return NULL;
    }

    gp->vNombre = (u32*) malloc(n*sizeof(unsigned int));

    if (!gp->vNombre)
    {
        liberar(gp);
        printf(CYAN "failed memory allocation for" RED " 'gp->vNombre'" RESET);
        return NULL;
    }

    gp->vColor = (Dupla) malloc(n*sizeof(struct DuplaSt));

    if (!gp->vColor)
    {
        liberar(gp->vNombre);
        liberar(gp);
        printf(CYAN "failed memory allocation for" RED " 'gp->vColor'" RESET);
        return NULL;
    }

    gp->vGrado = (Dupla) malloc(n*sizeof(struct DuplaSt));

    if (!gp->vGrado)
    {
        liberar(gp->vNombre);
        liberar(gp->vColor);
        liberar(gp);
        printf(CYAN "failed memory allocation for" RED " 'gp->vGrado'" RESET);
        return NULL;
    }

    gp->vLados = malloc(n*sizeof(unsigned int*));

    if (!gp->vLados)
    {
        liberar(gp->vNombre);
        liberar(gp->vColor);
        liberar(gp->vGrado);
        liberar(gp);
        printf(CYAN "failed memory allocation for" RED " 'gp->vLados'" RESET);
        return NULL;
    }

    gp->vOrden = malloc(n*sizeof(unsigned int));

    if (!gp->vOrden)
    {
        liberar(gp->vNombre);
        liberar(gp->vColor);
        liberar(gp->vGrado);
        liberar(gp->vLados);
        liberar(gp);
        printf(CYAN "failed memory allocation for" RED " 'gp->vOrden'" RESET);
        return NULL;
    }


    gp->n = n;
    gp->m = NumeroDeLados(G);
    gp->colores = NumeroDeColores(G);


    for(u32 i = 0; i < n; i++)
    {
        gp->vLados[i] = (unsigned int*) malloc(G->vGrado[i].v1*sizeof(unsigned int));
    }

    for(u32 i = 0; i < n; i++)
    {
        gp->vNombre[i] = G->vNombre[i];
        gp->vColor[i].v1 = G->vColor[i].v1;
        gp->vColor[i].v2 = G->vColor[i].v2;
        gp->vGrado[i].v1 = G->vGrado[i].v1;
        gp->vGrado[i].v2 = G->vGrado[i].v2;
        gp->vOrden[i] = G->vOrden[i];

        for(u32 j = 0; j < G->vGrado[i].v1; j++)
            gp->vLados[i][j] = G->vLados[i][j];
    }

    return gp;
}

// --------------------------------------------------------------
// 5 - Funciones de coloreo
// --------------------------------------------------------------
u32 Greedy(Grafo G)
{
    if (G == NULL)
        return 0;    

    u32 g = 0;  // grado de v1
    u32 i = 0;  // para los ciclos
    u32 j = 0;  // para los ciclis
    u32 n = NumeroDeVertices(G);
    u32 v = 0;  // v1
    u32 w = 0;  // vecino de v1
    u32 *tmp = NULL;
    u32 colores = 0;

    tmp = malloc(n*sizeof(unsigned int));

    if (!tmp)
        return 0;

    // Descoloreamos todo los vertices
    for(i = 0; i < n; i++)
    {
        G->vColor[i].v1 = UINTMAX;
        //tmp[i] = 0;
    }

    v = G->vOrden[0];
    G->vColor[v].v1 = 0;
    tmp[0] = 0; // el color 0 disponible
    colores = 1;

    // Recorremos el array vOrden
    for(i = 1; i < n; i++)
    {
        v = G->vOrden[i];
        g = G->vGrado[v].v1;

        // Vemos los vecinos de V y que color no esta disponible
        for(j = 0; j < g; j++)
        {
            w = G->vLados[v][j];

            // Si ya esta pintado, marcamos como no disponible c(w)
            if (G->vColor[w].v1 != UINTMAX)
            {
                tmp[G->vColor[w].v1] = 1;
            }
        }

        // Vemos si hay alguno de los colores disponible, si lo hay le damos el minimo
        for(j = 0; j < colores; j++)
        {
            if (tmp[j] == 0) // Es decir si el color j esta disponible y es el minimo
            {
                G->vColor[v].v1 = j;
                break;
            }
        }

        // Todos los colores estan ocupados, hay que darle uno nuevo
        if (j == colores)
        {
            G->vColor[v].v1 = colores;
            colores++;
        }

        /*g = G->vGrado[v].v1;

        for(j = 0; j < g; j++)
        {
            w = G->vLados[v][j];
            if (tmp[G->vColor[w].v1] == 1)
                tmp[G->vColor[w].v1] = 0;
        }*/

        // Ponemos todos los colores disponibles
        for(j = 0; j < colores; j++)
            tmp[j] = 0;
    }

    liberar(tmp);
    G->colores = colores;

    printf("x(G)=" YELLOW "%u\n" RESET, G->colores);
    return G->colores;
}

int Bipartito(Grafo G)
{
    if (G == NULL)
        return 0;    

    u32 i = 0;
    u32 j = 0;
    u32 n = NumeroDeVertices(G);
    u32 p = 0;
    u32 w = 0;
    u32 x = 0;

    // Creamos una cola
    Queue q = NULL;
    q = QueueCreate(q, n);

    // Descoloreamos todo los vertices
    for(i = 0; i < n; i++)
        G->vColor[i].v1 = UINTMAX;

    j = 0; // #vertices coloreados

    while(j < n)
    {
        for(i = x; i < n; i++)
        {
            if (G->vColor[i].v1 == UINTMAX)
            {
                x = i+1;
                break;
            }
        }

        G->vColor[x-1].v1 = 0;
        j++;

        QueueInsert(q, x-1);

        while(!QueueIsEmpty(q))
        {
            p = QueueRemoveData(q);

            for(w = 0; w < G->vGrado[p].v1; w++)
            {
                if (G->vColor[ G->vLados[p][w] ].v1 == UINTMAX)
                {
                    q = QueueInsert(q, G->vLados[p][w]);
                    j++;
                    G->vColor[G->vLados[p][w]].v1 = 1-(G->vColor[p].v1);
                }
            }
        }
    }

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < G->vGrado[i].v1; j++)
        {
            if (G->vColor[i].v1 == G->vColor[G->vLados[i][j]].v1)
            {
                QueueDestroy(q);
                Greedy(G);
                return 0;
            }
        }
    }

    QueueDestroy(q);
    return 1;      
}


// --------------------------------------------------------------
// 6 - Funciones para extraer informacion de datos del grafo
// --------------------------------------------------------------
u32 NumeroDeVertices(Grafo G)
{
    if (G == NULL)
        return UINTMAX;

    return G->n;
}

u32 NumeroDeLados(Grafo G)
{
    if (G == NULL)
        return UINTMAX;

    return G->m;
}

u32 NumeroDeColores(Grafo G)
{
    if (G == NULL)
        return UINTMAX;

    return G->colores;
}


// --------------------------------------------------------------
// 7 - Funciones de los vertices
// --------------------------------------------------------------
u32 NombreDelVertice(Grafo G, u32 i)
{
    if (G == NULL)
        return UINTMAX;

    return G->vNombre[G->vOrden[i]];   
}

u32 ColorDelVertice(Grafo G, u32 i)
{
    if (G == NULL || i >= NumeroDeVertices(G))
        return UINTMAX;

    return G->vColor[G->vOrden[i]].v1;
}

u32 GradoDelVertice(Grafo G, u32 i)
{
    if (G == NULL || i >= NumeroDeVertices(G))
        return UINTMAX;

    return G->vGrado[G->vOrden[i]].v1;
}

u32 ColorJotaesimoVecino(Grafo G, u32 i,u32 j)
{
    if (G == NULL || i >= NumeroDeVertices(G)|| j >= GradoDelVertice(G, i))
        return UINTMAX;

    return G->vColor[G->vLados[G->vOrden[i]][j]].v1;
}

u32 NombreJotaesimoVecino(Grafo G, u32 i,u32 j)
{
    if (i >= NumeroDeVertices(G))
        return UINTMAX;

    return G->vNombre[G->vLados[G->vOrden[i]][j]];
}


// --------------------------------------------------------------
// 8 - Funciones de ordenacion
// --------------------------------------------------------------
char OrdenNatural(Grafo G)
{
    if (G == NULL)
        return 0x01;

    u32 n = NumeroDeVertices(G);

    for(u32 i = 0; i < n; i++)
        G->vOrden[i] = i;

    return 0x00;
}

char OrdenWelshPowell(Grafo G)
{
    if (G == NULL)
        return 0x01;

    u32 n = NumeroDeVertices(G);

    Dupla tmp = NULL;
    tmp = (Dupla) malloc(n*sizeof(struct DuplaSt));
    
    if (!tmp)
        return 0x01;

    for(u32 i = 0; i < n; i++)
    {
        tmp[i].v1 = G->vGrado[i].v1; // Grado de Vi
        tmp[i].v2 = G->vGrado[i].v2; // posicion para encontrar Vi
    }

    qsort(tmp, n, sizeof(struct DuplaSt), sortOrdenWelshPowell);

    for(u32 i = 0; i < n; i++)
        G->vOrden[i] = tmp[i].v2;

    liberar(tmp);

    return 0x00;
}

char SwitchVertices(Grafo G, u32 i, u32 j)
{
    if (G == NULL || i >= NumeroDeVertices(G) || j >= NumeroDeVertices(G))
        return 0x01;

    u32 tmp = G->vOrden[i];
    G->vOrden[i] = G->vOrden[j];
    G->vOrden[j] = tmp;

    return 0x00;
}

char RMBCnormal(Grafo G)
{
    if (G == NULL)
        return 0x01;

    u32 n = NumeroDeVertices(G);
    Dupla tmp = NULL;

    tmp = (Dupla) malloc(n*sizeof(struct DuplaSt));
    
    if (tmp == NULL)
        return 0x01;

    for(u32 i = 0; i < n; i++)
    {
        tmp[i].v1 = G->vColor[i].v1;
        tmp[i].v2 = G->vColor[i].v2;
    }

    qsort(tmp, n, sizeof(struct DuplaSt), sortRMBCnormal);

    for(u32 i = 0; i < n; i++)
        G->vOrden[i] = tmp[i].v2;

    liberar(tmp);

    return 0x00;
}

char RMBCrevierte(Grafo G)
{
    if (G == NULL)
        return 0x01;

    u32 n = NumeroDeVertices(G);
    Dupla tmp = NULL;

    tmp = (Dupla) malloc(n*sizeof(struct DuplaSt));
    
    if (tmp == NULL)
        return 0x01;

    for(u32 i = 0; i < n; i++)
    {
        tmp[i].v1 = G->vColor[i].v1;
        tmp[i].v2 = G->vColor[i].v2;
    }

    qsort(tmp, n, sizeof(struct DuplaSt), sortRMBCrevierte);
    
    for(u32 i = 0; i < n; i++)
        G->vOrden[i] = tmp[i].v2;

    liberar(tmp);

    return 0x00;
}

char RMBCchicogrande(Grafo G)
{
    if (G == NULL)
        return 0x01;

    u32 i = 0;
    u32 j = 0;
    u32 k = 0;
    u32 n = NumeroDeVertices(G);
    u32 fin = 0;
    u32 pvn = 0;
    u32 colores = G->colores;
    u32 colorVisto = UINTMAX;
    Tripla tmp = NULL;
    u32 *vOrdenTmp = NULL;

    // Ordenamos para contar facil el grado de cada color
    RMBCnormal(G);

    tmp = (Tripla) malloc(colores*sizeof(struct TriplaSt));
    if (!tmp)
        return 0x01;

    vOrdenTmp = malloc(n*sizeof(unsigned int));
    if (!vOrdenTmp)
    {
        liberar(tmp);
        return 0x01;
    }

    // Hacemos una copia
    for(i = 0; i < n; i++)
        vOrdenTmp[i] = G->vOrden[i];

    tmp[0].inicio = 0;
    tmp[0].gradoColor = 1;
    colorVisto = ColorDelVertice(G, 0);

    // Obtenemos el grado de cada color, marcamos el inicio y fin
    for(i = 1; i < n; i++)
    {
        if (ColorDelVertice(G, i) != colorVisto)
        {
            // El colorVisto termina en i-1, osea la posicion anterior al nuevo color
            tmp[pvn].fin = i-1;
            // Marcamos el nuevo colorVisto
            colorVisto = ColorDelVertice(G, i);
            pvn++;
            // Marcamos el inicio del nuevo colorVisto
            tmp[pvn].inicio = i;
            tmp[pvn].gradoColor = 1;
        }
        else
        {
            tmp[pvn].gradoColor++;
        }
    }

    tmp[pvn].fin = i-1;

    qsort(tmp, NumeroDeColores(G), sizeof(struct TriplaSt), sortRMBCchicogrande);

    for(i = 0; i < colores; i++)
    {
        fin = tmp[i].fin;

        for(j = tmp[i].inicio; j <= fin; j++)
        {
            G->vOrden[i+k] = vOrdenTmp[j];
            k++;
        }

        k--;
    }

    liberar(tmp);
    liberar(vOrdenTmp);

    return 0x00;
}

char SwitchColores(Grafo G, u32 i, u32 j)
{
    if (G == NULL || i >= NumeroDeColores(G) || j >= NumeroDeColores(G))
        return 0x01;

    u32 n = G->n;

    for(u32 a = 0; a < n; a++)
    {
        if (G->vColor[a].v1 == i)
            G->vColor[a].v1 = j;

        else if (G->vColor[a].v1 == j)
            G->vColor[a].v1 = i;
    }

    return 0x00;
}


// --------------------------------------------------------------
//                FUNCIONES DE ORDENACION
// --------------------------------------------------------------
int sortDupla(const void * _a, const void * _b)
{
    Dupla a, b;
    a = (Dupla) _a;
    b = (Dupla) _b;


    if (a->v1 == b->v1) {
        if (a->v2 < b->v2)
            return -1;
        else if (a->v2 == b->v2)
            return 0;
        else
            return 1;
    }
    else {
        if (a->v1 < b->v1)
            return -1;
        else if (a->v1 == b->v1)
            return 0;
        else
            return 1;
    }
}

int sortOrdenWelshPowell(const void * _a, const void * _b)
{
    Dupla a, b;
    a = (Dupla) _a;
    b = (Dupla) _b;

    if (a->v1 < b->v1)
        return 1;
    else if (a->v1 == b->v1)
        return 0;
    else
        return -1;
}

int sortRMBCnormal(const void * _a, const void * _b)
{
    Dupla a, b;
    a = (Dupla) _a;
    b = (Dupla) _b;

    if (a->v1 < b->v1)
        return -1;
    else if (a->v1 == b->v1)
        return 0;
    else
        return 1;
}

int sortRMBCrevierte(const void * _a, const void * _b)
{
    Dupla a, b;
    a = (Dupla) _a;
    b = (Dupla) _b;

    if (a->v1 < b->v1)
        return 1;
    else if (a->v1 == b->v1)
        return 0;
    else
        return -1;
}

int sortRMBCchicogrande(const void * _a, const void * _b)
{
    Tripla a, b;
    a = (Tripla) _a;
    b = (Tripla) _b;

    if (a->gradoColor < b->gradoColor)
        return -1;
    else if (a->gradoColor == b->gradoColor)
        return 0;
    else
        return 1;
}


// --------------------------------------------------------------
//                      COLA
// --------------------------------------------------------------
Queue QueueCreate(Queue q, u32 n)
{
    q = malloc(sizeof(struct QueueSt));
    if (!q)
        return NULL;

    q->size = n-1;

    q->array = malloc(q->size*sizeof(unsigned int));
    if (!q->array)
    {
        liberar(q);
        return NULL;
    }

    q->front = 0;
    q->rear = -1;
    q->itemCount = 0;

    return q;
}

void QueueDestroy(Queue q)
{
    if (!q)
        return;

    free(q->array);
    free(q);
    q = NULL;
}

int QueuePeek(Queue q) //
{
    return q->array[q->front];
}

bool QueueIsEmpty(Queue q)
{
    return q->itemCount == 0;
}

bool QueueIsFull(Queue q)
{
   return q->itemCount == q->size;
}

int QueueSize(Queue q) //
{
    return q->itemCount;
}  

Queue QueueInsert(Queue q, u32 data)
{
    if (!QueueIsFull(q))
    {
        if (q->rear == (int) (q->size-1))
            q->rear = -1;                  

        q->array[++(q->rear)] = data;
        q->itemCount++;
    }

    return q;
}

u32 QueueRemoveData(Queue q)
{
    u32 data = q->array[q->front++];

    if (q->front == q->size)
        q->front = 0;

    q->itemCount--;

    return data;  
}


// --------------------------------------------------------------
//                      EXTRAS
// --------------------------------------------------------------
u32 busquedaBinaria(u32 vNombre[], u32 inicio, u32 final, u32 key)
{
    u32 medio = -1;

    while(inicio <= final)
    {
        medio = (u32) ((inicio+final) / 2);

        if (vNombre[medio] == key)
            return medio;
        else if (key < vNombre[medio])
            final = medio - 1;
        else
            inicio = medio + 1;
    }

    return medio;
}

void liberar(void * a)
{
    free(a);
    a = NULL;
}

int main()
{
    Grafo g = NULL;
    g = ConstruccionDelGrafo();
    /*time_t t;
    srand((unsigned) time(&t));
    u32 i, j, n;

    n = NumeroDeVertices(g);

    OrdenWelshPowell(g);
    Greedy(g);

    for(u32 k = 0; k < 100; k++)
    {
        i = rand() % n;
        j = rand() % n;

        SwitchVertices(g, i, j);
        Greedy(g);
    }

    for(u32 k = 0; k < 1000; k++)
    {
        i = rand() % 3;

        if (i == 0)
            RMBCnormal(g);
        else if (i == 1)
            RMBCrevierte(g);
        else
            RMBCchicogrande(g);

        Greedy(g);
    }*/

    //printf("Bipartito: %s", Bipartito(g) ? GREEN "Si" RESET : RED "No" RESET); 

    OrdenWelshPowell(g);
    Greedy(g);

    DestruccionDelGrafo(g);

    return 0;
}
