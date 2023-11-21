// Estrutura para representar um nó da árvore
typedef struct arvore {
    char* info;
    int ocorrencia;
    struct arvore *esq;
    struct arvore *dir;
} Arvore;

// Cria uma arvore vazia
Arvore* cria_arv_vazia(void);

// Verifica se a arvore está vazia
int verifica_arv_vazia(Arvore* a);

// Obtém um termo do arquivo
char* obterPalavra(FILE *file);

// Insere um termo na árvore
Arvore* arv_insere(Arvore* a, char* c);

// Ordena a árvore e armazena os termos e ocorrências nos arrays fornecidos
void arv_ordena(Arvore* a, int N, int ocorrencia_arr[], char* palavra_arr[]);

// Imprime a árvore inordem (ordem crescente)
void arv_imprime_inordem(Arvore* a);

// Imprime a árvore preordem (da raiz)
void arv_imprime_preordem(Arvore* a);

// Libera a memória alocada para a árvore
Arvore* arv_libera(Arvore* a);

// Busca na árvore por uma palavra e retorna o número de ocorrências
int arv_busca(Arvore* a, char* palavra);