#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista_adj.h"
#include "geodist.h"

typedef struct {
	char arquivoAeroportos[100];
	char arquivoRotas[100];
	char algoritmo[10];
	char origem[10];
	int quantidadeDestinos;
	char **destinos;

} DadosEntrada;

void removeNewLine(char *texto) {
	char* p = strchr(texto,'\n');
    if (p)
      *p = '\0';
}

char * removeCharacter(char *texto, char character ){
	char retorno[strlen(texto)];
	int i = 0, c = 0; 
	for(; i < strlen(texto); i++)
	{
	    if (texto[i] != character)
	    {
	        retorno[c] = texto[i];
	        c++;
	    }
	}
	retorno[c] = '\0';
	
	return retorno;
}

DadosEntrada obterDadosDeEntrada(FILE *entrada) {
	DadosEntrada dados;
	
	char conteudo[100];
	
	fgets( dados.arquivoAeroportos, 100, entrada );
	fgets( dados.arquivoRotas, 100, entrada );
	fgets( dados.algoritmo, 100, entrada );
	fgets( dados.origem, 100, entrada );
	fgets( conteudo, 100, entrada );
	dados.quantidadeDestinos = atoi(conteudo);
	dados.destinos = malloc(dados.quantidadeDestinos * sizeof(char*));
	
	int i;
	for(i = 0; i < dados.quantidadeDestinos; i++) {
	  fgets( conteudo, 100, entrada );
	  dados.destinos[i] = malloc((10 + 1) * sizeof(char));
	  removeNewLine(conteudo);
	  strcpy(dados.destinos[i], conteudo);
	}
		
	removeNewLine(dados.arquivoAeroportos);
	removeNewLine(dados.arquivoRotas);
	removeNewLine(dados.algoritmo);
	removeNewLine(dados.origem);
	
	fclose(entrada);
	
	return dados;
}

void testarDadosEntrada(DadosEntrada dadosEntrada){
	printf("Arquivo de Aeroportos: %s\n", dadosEntrada.arquivoAeroportos);
	printf("Arquivo de Rotas: %s\n", dadosEntrada.arquivoRotas);
	printf("Algoritmo: %s\n", dadosEntrada.algoritmo);
	printf("Origem: %s\n", dadosEntrada.origem);
	printf("Quantidade Destinos: %d\n", dadosEntrada.quantidadeDestinos);
	int i;
	for(i = 0; i < dadosEntrada.quantidadeDestinos; i++) {
		printf("Destino %d: %s\n", (i+1), dadosEntrada.destinos[i]);
	}
}

Vertice obterVertice(char *linha) {
	Vertice vertice;
	
	vertice.na = 0;
	vertice.marca = -1;
	vertice.arestas = NULL;
	char * dados = strtok(linha, ",");
	vertice.id = atoi(dados);
	strtok(NULL, ",");
	
	strcpy(vertice.cidade,strtok(NULL, ","));
	strcpy(vertice.pais,strtok(NULL, ","));
	strcpy(vertice.codigo,strtok(NULL, ","));
	strtok(NULL, ",");
	vertice.latitude = atof(strtok(NULL, ","));
	vertice.longitude = atof(strtok(NULL, ","));
	
	strcpy(vertice.cidade, removeCharacter(vertice.cidade, '"'));
	strcpy(vertice.pais, removeCharacter(vertice.pais, '"'));
	strcpy(vertice.codigo, removeCharacter(vertice.codigo, '"'));
	
	if(strncmp(vertice.codigo, "\\N", 3) == 0 || strlen(vertice.codigo) == 0){
		vertice.id = 0;
	}
			
	return vertice;
}

Vertice * pegaVertice(Grafo * g, int id){
	
	int i;

	if(id <= g->nv && g->vertices[id].id == id){
		
		return &g->vertices[id];
	}

	for(i = 0; i < g->nv; i++){

		if(g->vertices[i].id == id){
		
			 return &g->vertices[i];
		}
	}

	return NULL;
}

Vertice * pegaVerticePorCodigo(Grafo * g, char * codigo){
	int i;
	for(i = 0; i < g->nv; i++){
		if(strncmp(g->vertices[i].codigo, codigo, 10) == 0){
			 return &g->vertices[i];
		}
	}
	return NULL;
}

int existeAresta(Grafo * g, int u_id, int v_id){

	int achou = FALSE;
	Vertice * u = pegaVertice(g, u_id);
	Aresta * a = u->arestas;
	
	while(a && !achou){
		
		if(a->dst->id == v_id){
			return 1;
		}

		a = a->prox;
	}

	if(!achou){
	           return 0;
	}
}

void adicionaAresta(Grafo * g, int u_id, int v_id, int peso){

	Vertice * u = pegaVertice(g, u_id);
	Vertice * v = pegaVertice(g, v_id);
	
	if(existeAresta(g, u_id, v_id) == 0){
		Aresta * a = (Aresta *) malloc (sizeof(Aresta));
	
		a->dst = v;
		a->prox = u->arestas;
		a->peso = peso;
		u->arestas = a;
		u->na++;

		g->na++;
	}
}

Vertice* obterVertices(char arquivoAeroportos[100], int *numeroVertices){
	FILE *aeroportos = fopen(arquivoAeroportos, "r" );
	if (aeroportos == 0)
    {
        printf( "Arquivo de aeroportos nao encontrado no caminho: %s\n", arquivoAeroportos);
        return;
    }
    
	char conteudo[1000];
	int totalVertices = 0;
	
	while(!feof(aeroportos)){
		if(fgets( conteudo, 1000, aeroportos)){
			totalVertices++;
		}
	}
	
	fclose(aeroportos);
	
	Vertice* vertices = malloc(totalVertices * sizeof(Vertice));
	
	aeroportos = fopen(arquivoAeroportos, "r" );
	int nv = 0;
	while(!feof(aeroportos)) {
		if(fgets( conteudo, 1000, aeroportos)){
			Vertice vertice = obterVertice(conteudo);
			if(vertice.id != 0){
				vertices[nv] = vertice;
				nv++;
			}
		}
	}
	
	fclose(aeroportos);
	
	*numeroVertices = nv;
	    
    return vertices;
}

Grafo * criaGrafo(int numeroVertices, Vertice* vertices){
	Grafo * g = (Grafo *) malloc (sizeof(Grafo));
	g->vertices = vertices;//(Vertice *) malloc (numeroVertices * sizeof(Vertice));
	g->nv = numeroVertices;
	g->na = 0;

	return g;
}

void obterArestas(Grafo * grafo, char arquivoRotas[100]){
	double earth_radius =  6371.009;
	FILE *rotas = fopen(arquivoRotas, "r" );
	if (rotas == 0)
    {
        printf( "Arquivo de rotas nao encontrado no caminho: %s\n", arquivoRotas);
        return;
    }
    
	char conteudo[1000];
	
	while(!feof(rotas)){
		if(fgets( conteudo, 1000, rotas)){
			char * dados = strtok(conteudo, ",");
			strtok(NULL, ",");
			strtok(NULL, ",");
			int idOrigem = atoi(strtok(NULL, ","));
			strtok(NULL, ",");
			int idDestino = atoi(strtok(NULL, ","));
			
			if(idOrigem != 0 && idDestino != 0){
				Vertice * verticeOrigem = pegaVertice(grafo, idOrigem);
				Vertice * verticeDestino = pegaVertice(grafo, idDestino);
				Coordinate cordenadaOrigem = coordinate(verticeOrigem->latitude, verticeOrigem->longitude);
				Coordinate cordenadaDestino = coordinate(verticeDestino->latitude, verticeDestino->longitude);
				int peso = distance(cordenadaOrigem, cordenadaDestino, earth_radius);
				adicionaAresta(grafo, idOrigem, idDestino, peso);
			}
		}
	}
	
	fclose(rotas);
}

Grafo * obterGrafo(DadosEntrada entrada){
	int numeroVertices = 0;
	Vertice* vertices = obterVertices(entrada.arquivoAeroportos, &numeroVertices);
	
	Grafo * grafo = criaGrafo(numeroVertices, vertices);
	
	obterArestas(grafo, entrada.arquivoRotas);
	
    printf( "numero de arestas: %d \n",  grafo->na);
	
    return grafo;
}
//#define marca x.I
//#define próximo y.V

void buscaEmLargura(Grafo * g, Vertice * origem){
    Vertice *v, *w, *ativo, *ultimo, *temp;  
    Aresta *a;
    
    int i;
    for(i = 0; i < g->nv; i++){
          v = &g->vertices[i];
          v->marca = 0;
	}
	
    origem->marca = 1;
    
    ativo = ultimo = origem;  
    ultimo->proximo = NULL;
    
    while (ativo != NULL) {
        v = ativo;
        int achou = FALSE;
	    a = v->arestas;
        while(a && !achou){
            //w = a–>tip;//?????????
            if (w->marca == 0) {
                w->marca = 1;
                w->proximo = NULL;
                ultimo->proximo = w;
                ultimo = w; 
            } 
            a = a->prox;
        }
       ativo = ativo->proximo; 
    }
    
    for(i = 0; i < g->nv; i++){
          temp = &g->vertices[i];
          if(temp->proximo == NULL){
               printf("atual %s, proximo --\n", temp->codigo);
          }
          else{
               printf("atual %s, proximo %d\n", temp->codigo, temp->proximo->id);
          }
	}
}

int main(int argc, char *argv[]) {
	FILE *entrada = fopen("entrada.txt", "r" );
	FILE *saida = fopen("saida.txt", "w" );

    if (entrada == 0)
    {
        printf( "Arquivo de entrada nao encontrado\n" );
    }
    if (saida == 0)
    {
        printf( "Arquivo de saida nao encontrado\n" );
    }
    
    DadosEntrada dadosEntrada = obterDadosDeEntrada(entrada);
    
    Grafo * grafo = obterGrafo(dadosEntrada);
    
    if(strncmp(dadosEntrada.algoritmo, "ESCALAS", 7) == 0){
    	printf("comecou busca em largura\n\n");
        buscaEmLargura(grafo, pegaVerticePorCodigo(grafo, dadosEntrada.origem));
        printf("terminou busca em largura\n\n");
    }
    else if(strncmp(dadosEntrada.algoritmo, "DISTANCIA", 9) == 0){
    	printf("DISTANCIA?");    
    	
    }
    else{
    	printf("Algoritmo nao implementado, somente: ESCALAS ou DISTANCIA");
    }
    
    //testarDadosEntrada(dadosEntrada);

	return 0;
}
