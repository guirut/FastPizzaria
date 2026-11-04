/*
 * Projeto e Analise de Algoritmos II - Projeto 02 - Fast Pizzaria
 *
 * Integrantes do grupo:
 *   Nome completo: Beatriz Savazzi Del Bianco  RA: 10735936
 *   Nome completo: Guilherme Ruotolo da Costa  RA: 10736376
 *   Nome completo: Julio Cesar Nunes Mechi  RA: 10737836
 *   Nome completo: Laura Yumi Rodrigues Shiguemori  RA: 10735619
 *
 * Compilacao: gcc -o main.exe *.c
 * Execucao:   main.exe   (le o arquivo pizzaria.txt na mesma pasta)
 */

#include <stdio.h>
#include <stdlib.h>

#define INFINITO 1000000000
#define ARQUIVO "pizzaria.txt"

// Mapa M: matriz de adjacencia com o tempo de ir da esquina i ate a j 
int **mapa;
int n; // numero de esquinas (numeradas de 1 a n) 

/* Le o arquivo de entrada e monta o mapa. Retorna a esquina de entrega. */
int lerArquivo(const char *nome){
    FILE *arq;
    int entrega, a, b, t, i, j;

    arq = fopen(nome, "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nome);
        exit(1);
    }

    fscanf(arq, "%d", &entrega);
    fscanf(arq, "%d", &n);

    // aloca a matriz (indices 0..n) sem ligacoes
    mapa = (int **) malloc((n + 1) * sizeof(int *));
    for (i = 0; i <= n; i++) {
        mapa[i] = (int *) malloc((n + 1) * sizeof(int));
        for (j = 0; j <= n; j++) {
            mapa[i][j] = INFINITO;
        }
    }

    // le as triplas ate encontrar o 0
    while (fscanf(arq, "%d", &a) == 1 && a != 0) {
        fscanf(arq, "%d %d", &b, &t);
        if (a >= 1 && a <= n && b >= 1 && b <= n) {
            // se houver ruas repetidas, fica a mais rapida
            if (t < mapa[a][b]) {
                mapa[a][b] = t;
            }
        }
    }

    fclose(arq);
    return entrega;
}

/*
 * Algoritmo Rota-mais-rapida(M)
 * Calcula T[] (tempos) e R[] (esquina anterior na rota mais rapida)
 * partindo da esquina 1.
 */
void rotaMaisRapida(int T[], int R[]){
    int E[n + 1];  //conjunto E: E[e] = 1 se a esquina e ainda esta em E
    int tamE;      // quantidade de esquinas em E
    int e, v;

    // 1. inicializar E com as esquinas de M
    // 2-3. para cada esquina e em E: T[e] = INFINITO 
    for (e = 1; e <= n; e++) {
        E[e] = 1;
        T[e] = INFINITO;
        R[e] = -1;
    }
    tamE = n;

    // 4. T[1] = 0
    T[1] = 0;

    //5. enquanto E nao estiver vazio
    while (tamE > 0) {
        // 6. v = esquina em E com menor custo em T[]
        v = -1;
        for (e = 1; e <= n; e++) {
            if (E[e] == 1) {
                if (v == -1 || T[e] < T[v]) {
                    v = e;
                }
            }
        }

        // 7. E = E - {v}
        E[v] = 0;
        tamE--;

        // esquina inalcancavel: nao ha como relaxar a partir dela
        if (T[v] == INFINITO) {
            continue;
        }

        // 8. para toda esquina interligada v -> e, com e em E
        for (e = 1; e <= n; e++) {
            if (E[e] == 1 && mapa[v][e] != INFINITO) {
                // 9-10. relaxamento
                if (T[e] > T[v] + mapa[v][e]) {
                    T[e] = T[v] + mapa[v][e];
                    R[e] = v; // guarda de onde veio (rota)
                }
            }
        }
    }
}

// Imprime a rota de 1 ate destino usando o vetor R[]
void imprimirRota(int R[], int destino){
    int caminho[n + 1];
    int qtd = 0;
    int atual = destino;
    int i;

    while (atual != -1) {
        caminho[qtd] = atual;
        qtd++;
        atual = R[atual];
    }

    for (i = qtd - 1; i >= 0; i--) {
        printf(" %d", caminho[i]);
    }
    printf("\n");
}

int main(){
    int entrega, i;
    int *T, *R;

    entrega = lerArquivo(ARQUIVO);

    if (entrega < 1 || entrega > n) {
        printf("Esquina de entrega #%d invalida.\n", entrega);
        return 1;
    }

    T = (int *) malloc((n + 1) * sizeof(int));
    R = (int *) malloc((n + 1) * sizeof(int));

    rotaMaisRapida(T, R);

    if (T[entrega] == INFINITO) {
        printf("Nao existe rota ate a esquina #%d.\n", entrega);
    } else {
        printf("Rota ate a esquina #%d:", entrega);
        imprimirRota(R, entrega);
        printf("Tempo calculado para rota = %d min.\n", T[entrega]);
    }

    for (i = 0; i <= n; i++) {
        free(mapa[i]);
    }
    free(mapa);
    free(T);
    free(R);

    return 0;
}
