/*  Integrantes:
    Hannah Barbosa Goldstein - 2310160
    Pedro Carneiro Nogueira - 2310540 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 1200  // Tamanho maior para evitar muitas colisões
#define CPF_LENGTH 12    // Cada CPF tem 11 dígitos + '\0'

// Estrutura para armazenar um CPF na tabela hash
typedef struct {
    char cpf[CPF_LENGTH];
    int ocupado; // 0: livre, 1: ocupado
} HashEntry;

HashEntry hashTable[TABLE_SIZE]; // Tabela hash

// Função de hash simples usando módulo da soma dos dígitos
int hashFunction(const char *cpf) {
    int soma = 0;
    for (int i = 0; cpf[i] != '\0'; i++) {
        soma += cpf[i] - '0';  // Converte char para int
    }
    return soma % TABLE_SIZE;
}

// Inserção na tabela hash com tratamento de colisões (Linear Probing)
int inserirCPF(const char *cpf) {
    int index = hashFunction(cpf);
    int colisoes = 0;

    while (hashTable[index].ocupado) {
        colisoes++;
        index = (index + 1) % TABLE_SIZE;  // Linear Probing
    }

    strcpy(hashTable[index].cpf, cpf);
    hashTable[index].ocupado = 1;

    return colisoes;
}

// Inicializa a tabela hash
void inicializarTabela() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i].ocupado = 0;
    }
}

// Leitura dos CPFs de um arquivo e inserção na tabela
int carregarCPFs(const char *nomeArquivo, int limite) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char cpf[CPF_LENGTH];
    int colisoesTotais = 0;
    int inseridos = 0;

    while (fscanf(arquivo, "%s", cpf) != EOF && inseridos < limite) {
        colisoesTotais += inserirCPF(cpf);
        inseridos++;
    }

    fclose(arquivo);
    return colisoesTotais;
}

// Função principal
int main() {
    inicializarTabela();

    FILE *saida = fopen("colisoes.txt", "w");
    if (!saida) {
        perror("Erro ao criar o arquivo de saída");
        return EXIT_FAILURE;
    }

    printf("Inserindo CPFs na tabela...\n");
    for (int i = 100; i <= 1000; i += 100) {
        int colisoes = carregarCPFs("cpfs.txt", i);
        printf("CPFs inseridos: %d, Colisões: %d\n", i, colisoes);
        fprintf(saida, "%d %d\n", i, colisoes);
    }

    fclose(saida);
    printf("Processo concluído! Dados salvos em 'colisoes.txt'.\n");
    return 0;
}
