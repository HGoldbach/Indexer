#include "arvore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Função para printar o erro ocorrido durante a execução
void erroEncerrarPrograma(const char *mensagem) {
	printf("\n%s\nPrograma Encerrado!\n\n", mensagem);
    exit(1);
}

// Função para criar uma arvore vazia
Arvore* cria_arv_vazia(void) {
    return NULL;
}

// Função para verificar se a arvore é vazia
int verifica_arv_vazia(Arvore* a) {
    return (a == NULL);
}

// Função para obter a próxima palavra do arquivo
char* obterPalavra(FILE *file) {
    char c;
    char palavra[100];
    int i = 0;

    // Ignora caracteres não alfabéticos até encontrar um caractere alfabético
    c = fgetc(file);
    while (c != EOF && !isalpha(c)) {
        c = fgetc(file);
    }

    // Se o final do arquivo for alcançado, retorna NULL indicando que não há mais termos
    if (c == EOF) {
        return NULL;
    }

    // Lê o termo e converte para minúsculas
    do {
        palavra[i] = tolower(c);
        i++;
        c = fgetc(file);
    } while (c != EOF && isalpha(c));

    // Adiciona o caractere nulo ao final do termo
    palavra[i] = '\0';

    // Retorna uma cópia alocada dinamicamente do termo
    return strdup(palavra);
}

// Função para inserir um termo na árvore
Arvore* arv_insere(Arvore* a, char* c) {
    if (verifica_arv_vazia(a)) {
        // Se o nó não existe, aloca memória para a árvore
        a = (Arvore*)malloc(sizeof(Arvore));
        // Se a alocação não ocorrer encerra o programa
        if (verifica_arv_vazia(a)) {
            erroEncerrarPrograma("Erro durante a alocação de memória");
        }
        a->info = c;
        a->ocorrencia = 1;
        a->esq = a->dir = NULL;
    } else if (strcmp(c, a->info) < 0) {
        // Se o termo é menor, insere à esquerda
        a->esq = arv_insere(a->esq, c);
    } else if (strcmp(c, a->info) > 0) {
        // Se o termo é maior, insere à direita
        a->dir = arv_insere(a->dir, c);
    } else {
        // Se o termo já existe, incrementa a quantidade
        a->ocorrencia++;
    }
    return a;
}

void arv_ordena(Arvore* a, int N, int ocorrencia_arr[], char* palavra_arr[]) {
    if (verifica_arv_vazia(a)) {
        return;
    }

    arv_ordena(a->esq, N, ocorrencia_arr, palavra_arr);

    // Verifica se a ocorrência atual é maior do que a menor ocorrência na lista ordenada
    if (a->ocorrencia > ocorrencia_arr[N - 1]) {
        int i = N - 1;

        // Encontra a posição correta para inserir o termo na lista ordenada
        while (i > 0 && a->ocorrencia > ocorrencia_arr[i - 1]) {
            i--;
        }

        // Desloca os elementos para abrir espaço para o novo termo
        for (int j = N - 1; j > i; j--) {
            ocorrencia_arr[j] = ocorrencia_arr[j - 1];
            palavra_arr[j] = palavra_arr[j - 1];
        }

        // Insere o novo termo na posição correta da lista ordenada
        ocorrencia_arr[i] = a->ocorrencia;
        palavra_arr[i] = a->info;
    }

    arv_ordena(a->dir, N, ocorrencia_arr, palavra_arr);
}

Arvore* arv_libera(Arvore* a) {
    if(!verifica_arv_vazia(a)) {
        arv_libera(a->esq);
        arv_libera(a->dir);
        free(a);
    }
    return NULL;
}
