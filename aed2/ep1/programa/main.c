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
	vertice.pai = NULL;
	vertice.proximo = NULL;
	vertice.distancia = -1;
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

Aresta * pegaAresta(Grafo * g, int u_id, int v_id){

	int achou = FALSE;
	Vertice * u = pegaVertice(g, u_id);
	Aresta * a = u->arestas;
	
	while(a && !achou){
		
		if(a->dst->id == v_id){
			return a;
		}

		a = a->prox;
	}

	if(!achou){
	    return NULL;
	}
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
	g->vertices = vertices;
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
	
    return grafo;
}

void buscaEmLargura(Grafo * g, Vertice * origem){
    Vertice *v, *w, *ativo, *ultimo, *temp;  
    Aresta *a;
    
    int i;
    for(i = 0; i < g->nv; i++){
          v = &g->vertices[i];
          v->marca = 0;
          v->proximo = NULL;
          v->pai = NULL;
	}
	
    origem->marca = 1;
    
    ativo = ultimo = origem;  
    ultimo->proximo = NULL;
    
    while (ativo != NULL) {
        v = ativo;
	    a = v->arestas;
        while(a){
            w = a->dst;
            if (w->marca == 0) {
                w->marca = 1;
                w->pai=v; 
                ultimo->proximo = w;
                ultimo = w; 
            } 
            a = a->prox;
        }
       ativo = ativo->proximo; 
    }
}

void gerarSaida(Grafo * g, DadosEntrada entrada, FILE * saida){
	fprintf(saida, "%s %s %d %d\n", entrada.origem, entrada.algoritmo, g->nv, g->na);
	
	Vertice *origem, *destino, *temp, *verificando, *ultimoVisitado;
	origem = pegaVerticePorCodigo(g, entrada.origem);
	char **aeroportos = malloc(100 * sizeof(char*));
	int i;
	for(i = 0; i < 100; i++) {
	  aeroportos[i] = malloc((4) * sizeof(char));
	}
	for(i=0; i < entrada.quantidadeDestinos; i++){
		ultimoVisitado = NULL;
		int distancia, quantidadeAeroportos;
		quantidadeAeroportos = 0;
		destino = pegaVerticePorCodigo(g, entrada.destinos[i]);
		temp = destino;
		verificando = destino->pai;
		distancia = 0;
		while(verificando->id != origem->id){
			Aresta * a = pegaAresta(g, verificando->id, temp->id);
			distancia = distancia + a->peso;
			aeroportos[quantidadeAeroportos][0]=verificando->codigo[0];
			aeroportos[quantidadeAeroportos][1]=verificando->codigo[1];
			aeroportos[quantidadeAeroportos][2]=verificando->codigo[2];
			aeroportos[quantidadeAeroportos][3]='\0';
			quantidadeAeroportos++;
			ultimoVisitado = verificando;
			temp = verificando;
			verificando = verificando->pai;
		}
		
		if(ultimoVisitado == NULL){
			Aresta * a = pegaAresta(g, origem->id, destino->id);
			distancia = a->peso;
		}
		else{
			Aresta * a = pegaAresta(g, origem->id, ultimoVisitado->id);
			distancia = distancia + a->peso;
		}
		
		char listaAeroportos[(quantidadeAeroportos + 2)*4];
		
		listaAeroportos[0] = origem->codigo[0];
		listaAeroportos[1] = origem->codigo[1];
		listaAeroportos[2] = origem->codigo[2];
		listaAeroportos[3] = ' ';
		int contadorLista = 4;
		
		int contador;
		for(contador = quantidadeAeroportos - 1; contador >= 0; contador--){
			listaAeroportos[contadorLista] = aeroportos[contador][0];
			contadorLista++;
			listaAeroportos[contadorLista] = aeroportos[contador][1];
			contadorLista++;
			listaAeroportos[contadorLista] = aeroportos[contador][2];
			contadorLista++;
			listaAeroportos[contadorLista] = ' ';
			contadorLista++;
		}
		
		listaAeroportos[contadorLista] = destino->codigo[0];
		contadorLista++;
		listaAeroportos[contadorLista] = destino->codigo[1];
		contadorLista++;
		listaAeroportos[contadorLista] = destino->codigo[2];
		contadorLista++;
		listaAeroportos[contadorLista] = '\0';
		
		fprintf(saida, "%s %d %d %s\n", destino->codigo, quantidadeAeroportos + 2, distancia, listaAeroportos);
	}
	
	fclose(saida);
}

Vertice * obterVerticeComMenorDistancia(Grafo * g){
	int i;
	Vertice *v = NULL;
	for(i = 0; i < g->nv; i++){
		if(v == NULL && g->vertices[i].marca == -1 && g->vertices[i].distancia != -1) {
			v = &g->vertices[i];
		}
		if(v != NULL && g->vertices[i].marca != 1 && g->vertices[i].distancia != -1 && g->vertices[i].distancia < v->distancia){
			v = &g->vertices[i];
		}
	}
	return v;
}


void dijkstra(Grafo * g, Vertice * origem){
	Vertice *v, *w, *temp;
	Aresta *a;
	origem->distancia = 0;
	while(1){
		v = obterVerticeComMenorDistancia(g);
		if(v == NULL){
			break;
		}
		v->marca = 1;
		a = v->arestas;
        while(a){
            w = a->dst;
            if (w->marca != 1) {
            	int possivelDistancia = v->distancia + a->peso;
            	if(w->distancia == -1 || possivelDistancia < w->distancia){
            		w->distancia = possivelDistancia;
            		w->pai = v;
            	}
            } 
            a = a->prox;
        }
	}
}

int main(int argc, char *argv[]) {
	FILE *entrada = fopen(argv[1], "r" );
	FILE *saida = fopen(argv[2], "w" );

    if (entrada == 0)
    {
        printf( "Arquivo de entrada chamado: '%s' nao encontrado\n", argv[1]);
        return -1;
    }
    if (saida == 0)
    {
        printf( "Arquivo de saida chamado: '%s' nao encontrado\n", argv[2]);
        return -1;
    }
    
    DadosEntrada dadosEntrada = obterDadosDeEntrada(entrada);
    
    Grafo * grafo = obterGrafo(dadosEntrada);
    
    if(strncmp(dadosEntrada.algoritmo, "ESCALAS", 7) == 0){
        buscaEmLargura(grafo, pegaVerticePorCodigo(grafo, dadosEntrada.origem));
        gerarSaida(grafo, dadosEntrada, saida);
    }
    else if(strncmp(dadosEntrada.algoritmo, "DISTANCIA", 9) == 0){
        dijkstra(grafo, pegaVerticePorCodigo(grafo, dadosEntrada.origem));
        gerarSaida(grafo, dadosEntrada, saida);
    	
    }
    else{
    	printf("Algoritmo nao implementado, somente: ESCALAS ou DISTANCIA");
    }

	return 0;
}
