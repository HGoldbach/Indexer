#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "arvore.c"


int main(int argc, char* argv[]) {

	Arvore* a = cria_arv_vazia();  // Inicializa a árvore como nula
    int count = 0;
    int n_palavras;

    // Verifica se o número de argumentos está correta
    if (argc < 4 || argv[2] == NULL || argv[3] == NULL)  {
        erroEncerrarPrograma("Quantidade incorreta de argumentos!");
    }

    // Verifica se a opção selecionada é --freq
    if (strcmp(argv[1], "--freq") == 0) {
        n_palavras = atoi(argv[2]);
        FILE* file = fopen(argv[3], "r+");
        int ocorrencia_arr[n_palavras];
        char* palavra_arr[n_palavras];

        // Verifica se o arquivo foi aberto
        if (file == NULL) {
			erroEncerrarPrograma("Erro ao tentar abrir o arquivo!");
        } else {
            char* palavra;
            printf("\nLeitura do arquivo em andamento, por favor aguarde\n");
			
            // Lê as palavras do arquivo e as insere na árvore
            do {
				palavra = obterPalavra(file);
				if (palavra && strlen(palavra) > 2) {
					a = arv_insere(a, palavra);
					count++;
				}
			} while (palavra);

			printf("\nO top %s de palavras mais presentes no arquivo %s são:\n", argv[2], argv[3]);
        }
        fclose(file);

        // Inicializa o array ocorrencia com zeros
        for (int i = 0; i < n_palavras; i++) {
            ocorrencia_arr[i] = 0;
        }

        // Ordena os termos na árvore e os armazena nos arrays
        arv_ordena(a, n_palavras, ocorrencia_arr, palavra_arr);

        // Imprime os termos mais frequentes
		printf("\nPalavra\t\t| Aparicoes\n------------------------------");
        for (int i = 0; i < n_palavras; i++) {
            printf("\n%s\t\t| %d\n------------------------------", palavra_arr[i], ocorrencia_arr[i]);
        }
        printf("\n");

    } else if (strcmp(argv[1], "--freq-word") == 0) {
        FILE* file = fopen(argv[3], "r+");

        // Verifica se o arquivo foi aberto com sucesso
        if (file == NULL) {
            erroEncerrarPrograma("Erro ao tentar abrir o arquivo!");
        } else {
            char* palavra;

            printf("\nLeitura do arquivo em andamento, por favor aguarde\n");

            // Conta o número de ocorrências do termo específico
			do {
				palavra = obterPalavra(file);
				if (palavra && strcmp(palavra, argv[2]) == 0) {
					count++;
				}
			} while (palavra);

            printf("\nA palavra %s apareceu %d vezes no arquivo %s\n\n", argv[2], count, argv[3]);
        }
        fclose(file);

    } else if (strcmp(argv[1], "--search") == 0) {

        int qtde_arquivos = argc - 3;
        double TF[qtde_arquivos]; // Term Frequency - (Número de vezes que a palavra aparece no arquivo) / (Total de palavras no arquivo)
        double IDF; // Inverse Document Frequency - log[ (Número de Documentos) / (Número de documentos em que a palavra está presente) ]
        double TFIDF[qtde_arquivos]; // TF * IDF
        double TFIDF_TOTAL[qtde_arquivos];
        char* nome_arquivo[qtde_arquivos];

        char * palavras = strtok(argv[2], " ");
        while(palavras != NULL) {
            int count_interno = 0;
            // Para cada documento fornecido
            for (int i = 0; i < qtde_arquivos; i++) {
                FILE* file = fopen(argv[i+3], "r+");

                // Verifica se o arquivo foi aberto com sucesso
                if (file == NULL) {
                    erroEncerrarPrograma("Erro ao tentar abrir o arquivo!");
                } else {
                    char* palavra;
                    printf("\n\nLeitura do arquivo %s em andamento, por favor aguarde", argv[i+3]);
                    
                    int total_palavras = 0;
                    int ocorrencia = 0;   
                    

                    printf("\n\nPalavra buscada = %s", palavras);
                    // Conta o número de ocorrências da palavra específico no documento
                    do {
                        palavra = obterPalavra(file);
                        if (palavra) {
                            total_palavras++;
                            if (strcmp(palavra, palavras) == 0) {
                                ocorrencia++;
                            }
                        }
                    } while (palavra);
                    printf("\nQuantidade total de palavras = %d\nQuantidade de ocorrencia = %d\n", total_palavras, ocorrencia);

                    // Se o termo foi encontrado
                    if (ocorrencia > 0) {
                        count_interno++;
                    }

                    TF[i] += ((float)ocorrencia / total_palavras);
                    nome_arquivo[i] = argv[i+3];
                    printf("TF = %.10f", TF[i]);   
                    if (count <= count_interno) {
                        count = count_interno;
                    } 
                    

                }
                fclose(file);
            }


            if(count_interno == 0) {
                printf("\n\nO termo %s não está presente em nenhum dos arquivos\n\n", palavras);
                break;
            } else {
                IDF = log10((double)qtde_arquivos / count); 
                printf("\n\nIDF = %.10f", IDF);

                if(IDF == 0) {
                    printf("\nIDF zerado devido ao logaritmo ser 1");
                }
            }

            for (int i = 0; i < qtde_arquivos; i++) {
                TFIDF[i] = TF[i] * IDF; 
                TFIDF_TOTAL[i] += TFIDF[i];
            }

            float aux;
            char* aux2;

            // Ordena os documentos com base na relevância TFIDF
            for (int i = 0; i < qtde_arquivos; i++) {
                for (int j = i + 1; j < qtde_arquivos; j++) {
                    if (TFIDF[i] < TFIDF[j]) {
                        aux = TFIDF[i];
                        aux2 = nome_arquivo[i];
                        TFIDF[i] = TFIDF[j];
                        nome_arquivo[i] = nome_arquivo[j];
                        TFIDF[j] = aux;
                        nome_arquivo[j] = aux2;
                    }
                }
            }

            // Imprime a relevância da palavara nos arquivos
            printf("\n\nRelevância da palavra %s para os arquivos\n", palavras);
            for (int i = 0; i < qtde_arquivos; i++) {
                printf("%d - %s\tTFIDF = %.10f\n", i + 1, nome_arquivo[i],TFIDF[i]);
            }
            
            palavras = strtok(NULL, " ");

        }

        // Se nenhum documento possui o termo, imprime mensagem e libera a memória antes de sair
        if (count <= 0) {
            printf("\n\nO termo %s não está presente em nenhum dos arquivos\n\n", argv[2]);
            arv_libera(a);
            exit(0);
        } 

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
    } else {
        // Se os parâmetros são inválidos, imprime mensagem de erro e sai do programa
		erroEncerrarPrograma("Tipo incorreto de argumentos");
    }

    arv_libera(a);  // Libera a memória antes de sair
    exit(0);
}
