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

    // Ignora caracteres não alfabéticos
    c = fgetc(file);
    while (c != EOF && !isalpha(c)) {
        c = fgetc(file);
    }

    // Se o final do arquivo for alcançado, retorna NULL indicando que não há mais termos
    if (c == EOF) {
        return NULL;
    }

    // Lê o termo e converte para minúsculas
    while (c != EOF && isalpha(c)) {
        palavra[i] = tolower(c);
        i++;
        c = fgetc(file);
    }

    // Adiciona o caractere nulo ao final do termo
    palavra[i] = '\0';

    // Retorna uma cópia alocada dinamicamente do termo
    return strdup(palavra);
}

// Função para inserir um termo na árvore
Arvore* arv_insere(Arvore* a, char* palavra) {
    if (verifica_arv_vazia(a)) {
        // Se o nó não existe, aloca memória para a árvore
        a = (Arvore*)malloc(sizeof(Arvore));
        // Se a alocação não ocorrer encerra o programa
        if (verifica_arv_vazia(a)) {
            erroEncerrarPrograma("Erro durante a alocação de memória");
        }
        a->info = strdup(palavra);
        if (a->info == NULL) {
            erroEncerrarPrograma("Erro durante a alocação de memória para a palavra");
        }
        a->ocorrencia = 1;
        a->esq = a->dir = NULL;
    } else if (strcmp(palavra, a->info) < 0) {
        // Se o termo é menor, insere à esquerda
        a->esq = arv_insere(a->esq, palavra);
    } else if (strcmp(palavra, a->info) > 0) {
        // Se o termo é maior, insere à direita
        a->dir = arv_insere(a->dir, palavra);
    } else {
        // Se o termo já existe, incrementa a quantidade
        a->ocorrencia++;
    }
    return a;
}

// Função para buscar uma palavra e retornar o número de ocorrências
int arv_busca(Arvore* a, char* palavra) {
    if (verifica_arv_vazia(a)) {
        return 0;
    }

    if (strcmp(palavra, a->info) == 0) {
        return a->ocorrencia;
    } else if(strcmp(palavra, a->info) < 0) {
        return arv_busca(a->esq, palavra);
    } else {
        return arv_busca(a->dir, palavra);
    }
}

// Função para imprimir a árvore
void arv_imprime(Arvore* a) {
    if (verifica_arv_vazia(a)) {
        return;
    }

    // Visita os nós da árvore em ordem
    arv_imprime(a->esq);
    printf("%s: %d\n", a->info, a->ocorrencia);
    arv_imprime(a->dir);
}

// Função para ordenar a árvore e armazenar os termos e ocorrências nos arrays fornecidos
void arv_ordena(Arvore* a, int n_palavras, int ocorrencia_arr[], char* palavra_arr[]) {
    if (verifica_arv_vazia(a)) {
        return;
    }

    arv_ordena(a->esq, n_palavras, ocorrencia_arr, palavra_arr);

    // Verifica se a ocorrência atual é maior do que a menor ocorrência na lista ordenada
    if (a->ocorrencia > ocorrencia_arr[n_palavras - 1]) {
        int i = n_palavras - 1;

        // Encontra a posição correta para inserir o termo na lista ordenada
        while (i > 0 && a->ocorrencia > ocorrencia_arr[i - 1]) {
            i--;
        }

        // Desloca os elementos para abrir espaço para o novo termo
        for (int j = n_palavras - 1; j > i; j--) {
            ocorrencia_arr[j] = ocorrencia_arr[j - 1];
            palavra_arr[j] = palavra_arr[j - 1];
        }

        // Insere o novo termo na posição correta da lista ordenada
        ocorrencia_arr[i] = a->ocorrencia;
        palavra_arr[i] = a->info;
    }

    arv_ordena(a->dir, n_palavras, ocorrencia_arr, palavra_arr);
}

// Função para liberar a memória alocada para a árvore
Arvore* arv_libera(Arvore* a) {
    if(!verifica_arv_vazia(a)) {
        arv_libera(a->esq);
        arv_libera(a->dir);
        free(a);
    }
    return NULL;
}
