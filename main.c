#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "arvore.c"


int main(int argc, char* argv[]) {

    int count = 0;
    int n_palavras;
    int qtde_arquivos = argc - 3;
    int total_palavras[qtde_arquivos];
    Arvore* a = cria_arv_vazia(); 
    Arvore** arvores = (Arvore**)malloc(qtde_arquivos * sizeof(Arvore*));

    if (argc < 4 || argv[2] == NULL || argv[3] == NULL)  {
        erroEncerrarPrograma("Quantidade incorreta de argumentos!");
    } else if (strcmp(argv[1], "--search") != 0) {
        FILE* file = fopen(argv[3], "r+");
        if (file == NULL) {
            erroEncerrarPrograma("Erro ao tentar abrir o arquivo!");
        } else {
            char* palavra;
            printf("\nLeitura do arquivo em andamento, por favor aguarde\n");

            // Lê as palavras do arquivo e insere na árvore se tiver mais de 2 caracteres
            while ((palavra = obterPalavra(file)) != NULL) {
                if (strlen(palavra) > 2) {
                    a = arv_insere(a, palavra);
                }
            }
        }
        fclose(file);
    } else {

        // Inicializa as árvores para cada arquivo e o array de total de palavras
        for (int i = 0; i < qtde_arquivos; i++) {
            arvores[i] = cria_arv_vazia();
            total_palavras[i] = 0;
        }

        // Para cada arquivo, abre e lê as palavras, inserindo na árvore correspondente
        for (int i = 0; i < qtde_arquivos; i++) {
            FILE* file = fopen(argv[i+3], "r+");
            printf("\nLeitura dos arquivos em andamento, por favor aguarde\n");
            if (file == NULL) {
                erroEncerrarPrograma("Erro ao tentar abrir o arquivo!");
            } else {
                char* palavra;
                while ((palavra = obterPalavra(file)) != NULL) {
                    if (strlen(palavra) > 2) {
                        arvores[i] = arv_insere(arvores[i], palavra);
                    }
                    total_palavras[i]++;
                }
            }
            fclose(file);
        }
    }

    // Verifica se a opção selecionada é --freq
    if (strcmp(argv[1], "--freq") == 0) {
        n_palavras = atoi(argv[2]);
        FILE* file = fopen(argv[3], "r+");
        int ocorrencia_arr[n_palavras];
        char* palavra_arr[n_palavras];

        // Inicializa o array ocorrencia com zeros
        for (int i = 0; i < n_palavras; i++) {
            ocorrencia_arr[i] = 0;
        }

        // Ordena os termos na árvore e os armazena nos arrays
        arv_ordena(a, n_palavras, ocorrencia_arr, palavra_arr);

        // Imprime os termos mais frequentes
		printf("\n%-20s| %-20s\n------------------------------", "Palavra","Aparicoes");
        for (int i = 0; i < n_palavras; i++) {
            printf("\n%-20s| %-20d\n------------------------------", palavra_arr[i], ocorrencia_arr[i]);
        }
        printf("\n");

    } else if (strcmp(argv[1], "--freq-word") == 0) {

         // Busca a quantidade de vezes que a palavra aparece na árvore
        int qtde = arv_busca(a, argv[2]);
        printf("\nA palavra %s apareceu %d vezes no arquivo %s\n\n", argv[2], qtde, argv[3]);

    } else if (strcmp(argv[1], "--search") == 0) {

        int qtde_arquivos = argc - 3;
        int count = 0;
        double TF[qtde_arquivos]; // Term Frequency - (Número de vezes que a palavra aparece no arquivo) / (Total de palavras no arquivo)
        double IDF; // Inverse Document Frequency - log[ (Número de Documentos) / (Número de documentos em que a palavra está presente) ]
        double TFIDF[qtde_arquivos]; // TF * IDF
        double TFIDF_TOTAL[qtde_arquivos];
        char* nome_arquivo[qtde_arquivos];

        // Inicializa o array TFIDF_TOTAL com zeros
        for (int i = 0; i < qtde_arquivos; i++) {
            TFIDF_TOTAL[i] = 0.0;
        }

        // Divide as palavras colocadas no argv
        char * palavras = strtok(argv[2], " ");
        
        while(palavras != NULL) {
            count = 0;
            int count_interno = 0;
            printf("\n\n%-20s | %-20s | %-30s | %-30s | %-10s", "Arquivo", "Palavra buscada", "Quantidade total de palavras", "Quantidade total de ocorrencia", "TF");
            
            // Para cada arquivo
            for (int i = 0; i < qtde_arquivos; i++) {

                TF[i] = 0;
                int ocorrencia = 0;   

                printf("\n%-20s | %-20s ", argv[i+3], palavras);

                // Busca a quantidade de ocorrências da palavra na árvore
                ocorrencia = arv_busca(arvores[i], palavras);

                printf("| %-30d | %-30d |", total_palavras[i], ocorrencia);
                //printf("\nQuantidade total de palavras = %d\nQuantidade de ocorrencia = %d\n", total_palavras, ocorrencia);

                // Se o termo foi encontrado incrementa o count interno
                if (ocorrencia > 0) {
                    count_interno++;
                }

                 // Calcula do Term Frequency (TF)
                TF[i] += ((float)ocorrencia / total_palavras[i]);
                nome_arquivo[i] = argv[i+3];
                printf(" %.10f", TF[i]);   

                // redefine count caso o mesmo seja menor que o interno
                if (count <= count_interno) {
                    count = count_interno;
                } 

              
            }

             // Verifica se a palavra não está presente em nenhum dos arquivos
            if(count_interno == 0) {
                printf("\n\nO termo %s não está presente em nenhum dos arquivos\n\n", palavras);
                break;
            } else {
                // Calcula o Inverse Document Frequency (IDF)
                IDF = log10((double)qtde_arquivos / count); 
                printf("\n\nIDF = %.10f", IDF);

                // Mensagem se IDF for zero (log(1) == 0)
                if(IDF == 0) {
                    printf("\nIDF zerado devido ao logaritmo ser 1");
                }
            }

            // Calcula o TFIDF para cada documento
            for (int i = 0; i < qtde_arquivos; i++) {
                TFIDF[i] = TF[i] * IDF; 
                TFIDF_TOTAL[i] += TFIDF[i];
            }

            // Imprime a relevância da palavara nos arquivos
            printf("\n\nRelevância da palavra %s para os arquivos\n", palavras);
            for (int i = 0; i < qtde_arquivos; i++) {
                printf("%s\tTFIDF = %.10f\n", nome_arquivo[i],TFIDF[i]);
            }
            
            // Obtém a próxima palavra
            palavras = strtok(NULL, " ");
        }

        // Variaveis auxiliares para ajudar na ordenação dos arquivos
        float aux;
        char* aux2;

        // Ordena os documentos com base na relevância TFIDF
        for (int i = 0; i < qtde_arquivos; i++) {
            for (int j = i + 1; j < qtde_arquivos; j++) {
                if (TFIDF_TOTAL[i] < TFIDF_TOTAL[j]) {
                    aux = TFIDF_TOTAL[i];
                    aux2 = nome_arquivo[i];
                    TFIDF_TOTAL[i] = TFIDF_TOTAL[j];
                    nome_arquivo[i] = nome_arquivo[j];
                    TFIDF_TOTAL[j] = aux;
                    nome_arquivo[j] = aux2;
                }
            }
        }

        // Imprime a relevância total
        printf("\n\nSoma Total de Relevância\n");
        for (int i = 0; i < qtde_arquivos; i++) {
            printf("%d - %s\tTFIDF = %.10f\n", i + 1, nome_arquivo[i],TFIDF_TOTAL[i]);
        }
        printf("\n");

        // Libera a memória alocada para as árvores
        for (int i = 0; i < qtde_arquivos; i++) {
            arv_libera(arvores[i]);  
        }
        exit(0);
    } else {
		erroEncerrarPrograma("Tipo incorreto de argumentos");
    }

    // Libera a memória alocada para a árvore antes de sair
    arv_libera(a);  
    exit(0);
}
