#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MATRIX_SIZE 3
#define NUM_THREADS (MATRIX_SIZE * MATRIX_SIZE)

int matrixA[MATRIX_SIZE][MATRIX_SIZE];
int matrixB[MATRIX_SIZE][MATRIX_SIZE];
int resultMatrix[MATRIX_SIZE][MATRIX_SIZE];

int calculationCount = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int row;
    int col;
} thread_data_t;

void *multiply(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    int sum = 0;

    for (int i = 0; i < MATRIX_SIZE; i++) {
        int product = matrixA[data->row][i] * matrixB[i][data->col];
        sum += product;

        // Protect the counter with a mutex
        pthread_mutex_lock(&lock);
        calculationCount++;
        pthread_mutex_unlock(&lock);
    }

    resultMatrix[data->row][data->col] = sum;

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];

    // Initialize matrices A and B with random values
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrixA[i][j] = rand() % 10;
            matrixB[i][j] = rand() % 10;
        }
    }

    // Print matrix A
    printf("Matrix A:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", matrixA[i][j]);
        }
        printf("\n");
    }

    // Print matrix B
    printf("\nMatrix B:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", matrixB[i][j]);
        }
        printf("\n");
    }

    // Create one thread per cell of the result matrix
    int thread_index = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            thread_data[thread_index].row = i;
            thread_data[thread_index].col = j;
            pthread_create(&threads[thread_index], NULL, multiply, &thread_data[thread_index]);
            thread_index++;
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the result matrix
    printf("\nResult Matrix:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", resultMatrix[i][j]);
        }
        printf("\n");
    }

    // Print the total number of calculations
    printf("\nTotal number of calculations: %d\n", calculationCount);

    return 0;
}
