# Indexer
O trabalho consiste na criação de um programa capaz de indexar palavras de um ou mais documentos de texto. 

## Arquivos para testes
<http://200.236.3.126:8999/ds143-texts/>

## Descrição
O programa **indexer** realiza uma contagem de palavras em documentos de texto. A partir dessa contagem, ele ainda permite uma busca pelo número de ocorrências de uma palavra específica em um documento, ou a seleção de documentos relevantes para um dado termo de busca.
O programa **indexer** transforma todas as letras para minúsculas e ignora caracteres como números e pontuações.
Quando executado com a opção --freq, o programa **indexer** irá exibir o  número de ocorrências das N palavras mais frequentes no documento passado como parâmetro, em ordem decrescente de ocorrências.
Quando executado com a opção --freq-word, o programa **indexer** exibe a contagem de uma palavra específica no documento passado como parâmetro.
Por fim, quando executado com a opção --search, o programa **indexer**  apresenta uma listagem dos documentos mais relevantes para um dado termo de busca. Para tanto, o programa utiliza o cálculo TF-IDF (Term Frequency-Inverse Document Frequency).

## Compilação e Execução
Utilize o seguinte comando via terminal para compilar o código: **gcc -o indexer main.c -lm**
Dentro da pasta um arquivo executável com o nome de indexer será criado.

### Comandos
-  --freq N ARQUIVO
    Exibe o número de ocorrência das N palavras que mais aparecem em ARQUIVO, em
    ordem decrescente de ocorrência.
    **Exemplo de uso: ./indexer --freq 5 103.txt**

-  --freq-word PALAVRA ARQUIVO
    Exibe o número de ocorrências de PALAVRA em ARQUIVO. 
    **Exemplo de uso: ./indexer --freq-word project 103.txt**

-  --search TERMO ARQUIVO [ARQUIVO ...]
    Exibe uma listagem dos ARQUIVOS mais relevantes encontrados pela busca por 
    TERMO. A listagem é apresentada em ordem descrescente de relevância. 
    TERMO pode conter mais de uma palavra. Neste caso, deve ser indicado entre 
    àspas.
    **Exemplo de uso: ./indexer --search 'the was' 103.txt 128mb.txt**

## Funcionalidades do Programa
- Transforma todas os caracteres em minúsculo
- Ignora palavras com menos de 2 caracteres
- Ignora caracteres que não sejam letras, como números e pontuações
