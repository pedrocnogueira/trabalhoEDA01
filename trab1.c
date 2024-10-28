/*  Integrantes:
    Hannah Barbosa Goldstein - 2310160
    Pedro Carneiro Nogueira - 2310540 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TABLE_SIZE 1009  // Tamanho >= n de entradas; tamanho primo
#define EMPTY_SLOT 0ULL // Define a value that represents an empty slot

// Estrutura para armazenar um CPF na tabela hash

unsigned long long hashTable[TABLE_SIZE]; // Tabela hash
int colisoes = 0;

void zeraTabela() {
        for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = EMPTY_SLOT;
    }
}

unsigned long long converteCPF(char* c) {
    unsigned long long n = 0;
    for (int i = 10; i > 0; i--) {
        n += (unsigned long long)(*c++ - '0') * pow(10, i);
    }
    return n;
}

int hash(unsigned long long x) {
    return x % TABLE_SIZE;
}

void insereHash(unsigned long long x) {
    int k = 0;
    int maxProbes = TABLE_SIZE;
    int originalHash = hash(x) % TABLE_SIZE;
    int hashResult = originalHash;

    while (hashTable[hashResult] != EMPTY_SLOT) {
        if (hashTable[hashResult] == x) {
            // The value already exists in the hash table; no need to insert
            return;
        }
        k++;
        colisoes++; // Increment collision counter for each collision
        hashResult = (originalHash + k + k*k) % TABLE_SIZE;

        if (k >= maxProbes) {
            // All slots have been probed; the table is full
            printf("Hash table is full. Cannot insert %llu.\n", x);
            return;
        }
    }

    // Insert the value into the empty slot
    hashTable[hashResult] = x;
}

int leInsere(FILE* arq, int qtd) {
    unsigned long long num;
    int count = 0;
    while (count < qtd && fscanf(arq, "%llu", &num) == 1) {
        insereHash(num);
        count++;
    }
    return count;
}

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
