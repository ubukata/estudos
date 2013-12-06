//VITOR UBUKATA NUMERO USP: 6776640
#define TRUE 1
#define FALSE 0

typedef struct VerticeStruct {
	int id;
	char cidade[100];
	char pais[100];
	char codigo[10];
	
	double latitude;
	double longitude;
	
	int na;
	int marca;
	int distancia;
	
	struct VerticeStruct * proximo;
	struct VerticeStruct * pai;
	struct ArestaStruct * arestas;

} Vertice;

typedef struct ArestaStruct {

	Vertice * dst;
	int peso;
	struct ArestaStruct * prox;

} Aresta;

typedef struct {

	int nv, na;

	Vertice * vertices;

} Grafo;
