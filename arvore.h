// Estrutura para representar um nó da árvore
typedef struct arvore {
    char* info;
    int ocorrencia;
    struct arvore *esq;
    struct arvore *dir;
} Arvore;

Arvore* cria_arv_vazia(void);
int verifica_arv_vazia(Arvore* a);

// Obtém um termo do arquivo
char* obterPalavra(FILE *file);

// Insere um termo na árvore
Arvore* arv_insere(Arvore* a, char* c);

// Ordena a árvore
void arv_ordena(Arvore* a, int N, int arr[], char* str[]);

// Imprime a árvore
void arv_imprime(Arvore* a);

Arvore* arv_libera(Arvore* a);
