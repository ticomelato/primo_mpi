#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int e_primo(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i <= sqrt(n); i += 2)
        if (n % i == 0) return 0;
    return 1;
}

int compare_ints(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int main(int argc, char** argv) {
    int rank, size;
    const int N = 100000;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 1) {
        if (rank == 0)
            printf("Erro: Numero de processos é zero.\n");
        MPI_Finalize();
        return 1;
    }

    int inicio = (rank * N) / size;
    int fim = ((rank + 1) * N) / size;

    // Aloca vetor local de primos
    int* primos_locais = malloc((fim - inicio) * sizeof(int));
    int contador_local = 0;

    for (int i = inicio; i < fim; i++) {
        if (e_primo(i)) {
            primos_locais[contador_local++] = i;
        }
    }

    if (rank == 0) {
        // Processo mestre
        int total_primos = contador_local;
        int* primos_totais = malloc(N * sizeof(int)); // Aloca espaço máximo possível
        for (int i = 0; i < contador_local; i++) {
            primos_totais[i] = primos_locais[i];
        }

        for (int i = 1; i < size; i++) {
            int contador_remoto;
            MPI_Recv(&contador_remoto, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            int* buffer = malloc(contador_remoto * sizeof(int));
            MPI_Recv(buffer, contador_remoto, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for (int j = 0; j < contador_remoto; j++) {
                primos_totais[total_primos++] = buffer[j];
            }

            free(buffer);
        }

        // Ordena o vetor final
        qsort(primos_totais, total_primos, sizeof(int), compare_ints);

        // Exibe resultado
        printf("Total de primos encontrados: %d\n", total_primos);
        printf("Exibindo os 10 primeiros e os 10 ultimos:\n");
        for (int i = 0; i < 10 && i < total_primos; i++) {
            printf("%d ", primos_totais[i]);
        }
        printf("...\n");
        for (int i = (total_primos > 10 ? total_primos - 10 : 0); i < total_primos; i++) {
            printf("%d ", primos_totais[i]);
        }
        printf("\n");

        free(primos_totais);
    } else {
        // Outros processos enviam resultados
        MPI_Send(&contador_local, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(primos_locais, contador_local, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    free(primos_locais);
    MPI_Finalize();
    return 0;
}
