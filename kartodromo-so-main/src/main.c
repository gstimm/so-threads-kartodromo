#include "kartodromo.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN_PILOTOS_GRUPO 25
#define MAX_PILOTOS_GRUPO 35

int main() {
    srand(time(NULL));

    // 1. Defini e inicia kartodromo
    Kartodromo kartodromo;
    inicializa_kartodromo(&kartodromo);
    imprime_kartodromo(&kartodromo);
    
    // 2. Logia de chegada de grupos
    for (int i = 0; i < NUM_GRUPOS; i++) {
        // Um grupo chega com um número aleatório de pilotos (entre 25 e 35)
        int num_pilotos = MIN_PILOTOS_GRUPO + rand() % (MAX_PILOTOS_GRUPO - MIN_PILOTOS_GRUPO + 1);
        printf("\nGrupo %d chegando com %d pilotos\n", i + 1, num_pilotos);

        // 3. Chegada de pilotos
        chegada_grupo(&kartodromo, num_pilotos);
    }


    // 3. Destroi pilotos e katodromo
    destroi_kartodromo(&kartodromo);

    return 0; 
}