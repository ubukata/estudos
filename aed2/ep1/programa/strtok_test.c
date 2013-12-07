//VITOR UBUKATA NUMERO USP: 6776640
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * A função strtok serve para particionar uma string (array de caracteres) em
 * tokens (campos). Ela recebe dois parâmetros: a string que será particionada 
 * e uma segunda string que define o delimitador que separa os tokens. A cada
 * chamada da função strtok o próximo token encontrado é devolvido. Assim, para
 * se obter todos os tokens de uma string deve-se executar sucessivas chamadas 
 * para esta função.
 * 
 * Observe, contudo, que a string a ser pariticionada só deve ser passada como
 * parâmetro para a função strtok na primeira chamada feita (a chamada que devolve
 * o primeiro token). Nas demais chamadas (que devolvem do segundo token em diante)
 * deve-se passar o valor NULL como primeiro parâmetro da função.
 * 
 * Quando não existirem mais tokens a serem devolvidos, a função irá devolver NULL.
 * 
 * Alguns cuidados que devem ser tomados:
 * 
 *  - Uma sequência de dois ou mais delimitadores consecutivos é considerada como um 
 *    delimitador único. Por exemplo, para a string "a|b||c" e delimitador "|", os
 *    tokens que serão obtidos pelas chamadas à função strtok serão "a", "b" e "c".
 *    Contudo, outra interpretação possível é a de que a string possui 4 tokens, sendo 
 *    um deles vazio: "a", "b", "" e "c". Por sorte, tal situação não afeta o processamento
 *    dos arquivos de dados para o EP. No arquivo airports.dat não há nenhuma ocorrência de
 *    delimitadores consecutivos, e as ocorrências que existem no arquivo routes.dat ocorrem
 *    depois dos tokens de interesse para o EP.
 *
 *  - A função strtok modifica o contúdo original da string a ser particionada. Não deve ser
 *    um problema para o EP, mas é bom estar ciente disso!
 * 
 *  - O token devolvido pela função strtok é um ponteiro para uma região contida dentro da
 *    string original. Se a string original tiver o conteúdo alterado (essa situação acontece 
 *    quando, por exemplo, fazemos um loop para ler as linhas de um arquivo, e usamos a mesma
 *    variável para armazenar o conteúdo da linha a cada execução do loop), os tokens serão
 *    corrompidos. Para evitar tal situação, é preciso copiar o token devolvido para uma outra
 *    string (veja as funções "teste3" e "teste4" que ilustram esta situação).
 */


void test1(char * s, char * delim){


	char * token = strtok(s, delim); 

	while(token){

		printf("Token: '%s'\n", token);

		token = strtok(NULL, delim);
	}		
}

void test2(char * s, char * delim){

	char * campo2;
	char * campo4;

	// devolve 1.o campo (retorno ignorado)
	strtok(s, delim);

	// devolve o 2.o campo
	campo2 = strtok(NULL, delim);

	// devolve 3.o campo (retorno ignorado)
	strtok(NULL, delim);

	// devolve 4.o campo
	campo4 = strtok(NULL, delim);

	printf("2.o campo = '%s', 4.o campo = '%s'\n", campo2, campo4);
}

void test3(){

	char linha[200];
	char * delim = ",";

	char * t1;
	char * t2;
	char * t3;

	// vamos guardar o segundo token de cada linha...

	strcpy(linha, "1,22,333");
	printf("segundo token da linha '%s' = ", linha);
	strtok(linha, delim);
	t1 = strtok(NULL, delim);
	printf("'%s'\n", t1);

	strcpy(linha, "4444,55555,666666");
	printf("segundo token da linha '%s' = ", linha);
	strtok(linha, delim);
	t2 = strtok(NULL, delim);
	printf("'%s'\n", t2);

	strcpy(linha, "7777777,88888888,999999999");
	printf("segundo token da linha '%s' = ", linha);
	strtok(linha, delim);
	t3 = strtok(NULL, delim);
	printf("'%s'\n", t3);
	
	printf("tokens: '%s', '%s', '%s'\n", t1, t2, t3);
}

void test4(){

	char linha[200];
	char * delim = ",";

	char * temp;
	char t1[20];
	char t2[20];
	char t3[20];

	// vamos guardar o segundo token de cada linha...

	strcpy(linha, "1,22,333");
	printf("segundo token da linha '%s' = ", linha);
	strtok(linha, delim);
	temp = strtok(NULL, delim);
	strcpy(t1, temp);
	printf("'%s'\n", t1);

	strcpy(linha, "4444,55555,666666");
	printf("segundo token da linha '%s' = ", linha);
	strtok(linha, delim);
	temp = strtok(NULL, delim);
	strcpy(t2, temp);
	printf("'%s'\n", t2);

	strcpy(linha, "7777777,88888888,999999999");
	printf("segundo token da linha '%s' = ", linha);
	strtok(linha, delim);
	temp = strtok(NULL, delim);
	strcpy(t3, temp);
	printf("'%s'\n", t3);
	
	printf("tokens: '%s', '%s', '%s'\n", t1, t2, t3);
}

/*int main(){

	char s[1000];

	strcpy(s, "1,\"Goroka\",\"Goroka\",\"Papua New Guinea\",\"GKA\",\"AYGA\",-6.081689,145.391881,5282,10,\"U\"");
	test1(s, ",");
	
	printf("--------------------------------------------------------\n");

	strcpy(s, "aaaa|bbb|ccc|ddd"); // experimente comentar essa linha e veja o que acontece...
	test2(s, "|");

	printf("--------------------------------------------------------\n");

	test3();

	printf("--------------------------------------------------------\n");

	test4();

	return 0;
}*/

