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


// Função para ordenar os termos da árvore com base na quantidade de ocorrências
void arv_ordena(Arvore* a, int N, int arr[], char* str[]) {
    if (verifica_arv_vazia(a)) {
        return;
    } else {
        arv_ordena(a->esq, N, arr, str);
    }

    if (a->ocorrencia > arr[N - 1]) {
        // Se a quantidade de ocorrências é maior que a menor quantidade na lista ordenada
        int x = 0;
        int y;
        char* c;

        // Encontra a posição correta para inserir o termo na lista ordenada
        while (arr[x] >= a->ocorrencia) {
            x++;
        }

        // Insere o termo na posição correta da lista ordenada
        for (int i = x; i < N; i++) {
            for (int j = i + 1; j < N; j++) {
                if (arr[i] < arr[j]) {
                    y = arr[i];
                    c = str[i];

                    arr[i] = arr[j];
                    str[i] = str[j];

                    arr[j] = y;
                    str[j] = c;
                }
            }
        }

        arr[x] = a->ocorrencia;
        str[x] = a->info;
    }
    arv_ordena(a->dir, N, arr, str);
}

Arvore* arv_libera(Arvore* a) {
    if(!verifica_arv_vazia(a)) {
        arv_libera(a->esq);
        arv_libera(a->dir);
        free(a);
    }
    return NULL;
}
