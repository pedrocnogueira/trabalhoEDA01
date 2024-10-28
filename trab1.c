/*  Integrantes:
    Hannah Barbosa Goldstein - 2310160
    Pedro Carneiro Nogueira - 2310540 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TABLE_SIZE 1319  // Tamanho >= n de entradas; tamanho primo; alpha ~= 0.76
#define EMPTY_SLOT 0ULL // Valor de uma posição vazia

unsigned long long hashTable[TABLE_SIZE]; // Tabela hash
int colisoes = 0; // Contador de colisoes

/* Prototipos */
void zeraTabela();
void imprimeTabela();
int hash(unsigned long long x);
int leInsere(FILE* arq, int qtd);
void insereHash(unsigned long long x);
int buscaHash(unsigned long long value);
unsigned long long converteCPF(char* c);

/* Bloco Principal */
int main(void) {
    // Open the input file for reading
    FILE* file = fopen("cpfs.txt", "r");
    if (file == NULL) {
        printf("Error opening input file.\n");
        return EXIT_FAILURE;
    }

    // Open the output CSV file for writing
    FILE* csvFile = fopen("collisions.csv", "w");
    if (csvFile == NULL) {
        printf("Error opening output CSV file.\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    fprintf(csvFile, "Entries,Collisions\n"); // Write CSV header

    // Loop over 10 iterations, inserting 100, 200, ..., 1000 entries
    for (int qtd = 100; qtd <= 1000; qtd += 100) {
        zeraTabela(); // Initialize the hash table
        colisoes = 0; // Reset collision counter

        // Reset file pointer to the beginning
        fseek(file, 0, SEEK_SET);

        // Read and insert 'qtd' numbers
        int inserted = leInsere(file, qtd);

        printf("Inserted %d numbers into the hash table.\n", inserted);
        printf("Total collisions encountered: %d\n", colisoes);
        printf("Total empty positions: %d\n", TABLE_SIZE - inserted);

        // Write the result to the CSV file
        fprintf(csvFile, "%d,%d\n", qtd, colisoes);
    }

    fclose(file);     // Close the input file
    fclose(csvFile);  // Close the CSV file

    return 0;
}

/* Funcoes Auxiliares */

// Funcao para insereir valor vazio em todas as posições
void zeraTabela() {
        for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = EMPTY_SLOT;
    }
}

// Funcao para printar a tabela em um txt para testes
void imprimeTabela() {
    FILE* arq = fopen("hashTable.txt", "w");
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hashTable[i] == EMPTY_SLOT) {
            fprintf(arq, "> %04d: Vazio\n", i);
        } else {
            fprintf(arq, "> %04d: %011llu\n", i, hashTable[i]);
        }
    }
}

// Funcao hash 1 - metodo da divisao com mult por cte
int hashParcial2(unsigned long long x) {
    unsigned long long hash = x * 2654435761;
    return (int)(hash % TABLE_SIZE);
}

// Funcao hash 2 - metodo da dobra xor
int hashParcial1(unsigned long long x) {
    unsigned int first_half = (unsigned int)(x >> 32);
    unsigned int last_half = (unsigned int)(x & 0xFFFFFFFF);
    return (first_half ^ last_half) % TABLE_SIZE;
}

// Funcao para inserir na tabela hash
void insereHash(unsigned long long x) {
    int k = 0;
    int maxProbes = TABLE_SIZE;
    int originalHash = hashParcial1(x) % TABLE_SIZE;
    int hashResult = originalHash;

    while (hashTable[hashResult] != EMPTY_SLOT) {
        if (hashTable[hashResult] == x) {
            // The value already exists in the hash table; no need to insert
            return;
        }
        k++;
        colisoes++; // Increment collision counter for each collision
        hashResult = (originalHash + (k * hashParcial2(x))) % TABLE_SIZE;

        if (k >= maxProbes) {
            // All slots have been probed; the table is full
            printf("Hash table is full. Cannot insert %llu.\n", x);
            return;
        }
    }

    // Insert the value into the empty slot
    hashTable[hashResult] = x;
}

// Funcao para buscar na tabela hash
int buscaHash(unsigned long long value) {
    int k = 0;
    int maxProbes = TABLE_SIZE;
    int originalHash = hashParcial1(value);
    int hashResult = originalHash;

    while (hashTable[hashResult] != EMPTY_SLOT) {
        if (hashTable[hashResult] == value) {
            // Value found at index 'hashResult'
            return hashResult;
        }
        k++;
        hashResult = (originalHash + (k * hashParcial2(value))) % TABLE_SIZE;

        if (k >= maxProbes) {
            // All slots have been probed; the value is not in the table
            break;
        }
    }

    // Value not found
    return -1;
}

// Funcao para ler o arquivo e inserir cada linha na tabela
int leInsere(FILE* arq, int qtd) {
    unsigned long long num;
    int count = 0;
    while (count < qtd && fscanf(arq, "%llu", &num) == 1) {
        insereHash(num);
        count++;
    }
    return count;
}