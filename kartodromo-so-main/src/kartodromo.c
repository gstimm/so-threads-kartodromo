#include "kartodromo.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // Para a função sleep

void inicializa_kartodromo(Kartodromo *k) {
    k->karts_disponiveis = NUM_KARTS;
    k->capacetes_disponiveis = NUM_CAPACETES;
    pthread_mutex_init(&k->mutex_karts, NULL);
    pthread_mutex_init(&k->mutex_capacetes, NULL);
    pthread_mutex_init(&k->mutex_tempo_espera, NULL);
    pthread_cond_init(&k->cond_karts, NULL);
    pthread_cond_init(&k->cond_capacetes, NULL);

    k->criancas_menores_14 = 0;
    k->tempo_total_espera = 0;
    k->total_recursos_utilizados_karts = 0;
    k->total_recursos_utilizados_capacetes = 0;
}

void destroi_kartodromo(Kartodromo *k) {
    pthread_mutex_destroy(&k->mutex_karts);
    pthread_mutex_destroy(&k->mutex_capacetes);
    pthread_cond_destroy(&k->cond_karts);
    pthread_cond_destroy(&k->cond_capacetes);
}

void imprime_kartodromo(Kartodromo *k) {
    printf("\n\n\nKartódromo:\n");
    printf("Disponíveis: Karts: %d, Capacetes: %d\n", k->karts_disponiveis, k->capacetes_disponiveis);
    printf("Total de clientes atendidos: %d\n", k->total_recursos_utilizados_karts);
    printf("Total de recursos utilizados (Karts): %d\n", k->total_recursos_utilizados_karts);
    printf("Total de recursos utilizados (Capacetes): %d\n", k->total_recursos_utilizados_capacetes);
    if (k->total_recursos_utilizados_karts > 0) {
        printf("Tempo total de espera: %d horas\n", k->tempo_total_espera);
        double media_tempo_espera = (double)k->tempo_total_espera / k->total_recursos_utilizados_karts;
        printf("Média de tempo de espera: %.2lf horas\n", media_tempo_espera);
    } else {
        printf("Média de tempo de espera: N/A\n");
    }
}

typedef struct {
    Piloto *piloto;
    Kartodromo *kartodromo;
} PilotoArg;


void *piloto_thread(void *arg) {
    PilotoArg *piloto_arg = (PilotoArg *)arg;
    Piloto *piloto = piloto_arg->piloto;
    Kartodromo *kartodromo = piloto_arg->kartodromo;

    time_t chegada = time(NULL);

    printf("Piloto %s, idade: %d chegou ao kartódromo.\n", piloto->nome, piloto->idade);

    if (piloto->idade < 18) {
        // Crianças menores de 14 têm prioridade na escolha do capacete
        //         Se o piloto tem menos de 14 anos, incrementa kartodromo->criancas_menores_14 e verifica se há capacetes disponíveis ou se outras crianças menores de 14 anos também estão esperando. Se sim, o piloto espera até que seja a sua vez.

        //          Crianças entre 14 e 18 anos só pegam capacetes quando as crianças menores de 14 anos não estão na fila de espera.
        pthread_mutex_lock(&kartodromo->mutex_capacetes);
        if (piloto->idade < 14) {
            kartodromo->criancas_menores_14++;
            while (kartodromo->capacetes_disponiveis == 0) {
                pthread_cond_wait(&kartodromo->cond_capacetes, &kartodromo->mutex_capacetes);
            }
        } else {
            while (kartodromo->capacetes_disponiveis == 0 || kartodromo->criancas_menores_14 > 0) {
                pthread_cond_wait(&kartodromo->cond_capacetes, &kartodromo->mutex_capacetes);
            }
        }

        printf("Piloto %s, idade: %d pegou um capacete.\n", piloto->nome, piloto->idade);
        kartodromo->capacetes_disponiveis--;
        kartodromo->total_recursos_utilizados_capacetes++;

        if (piloto->idade < 14) {
            kartodromo->criancas_menores_14--;
        }
        pthread_mutex_unlock(&kartodromo->mutex_capacetes);

        // Agora pegar kart
        pthread_mutex_lock(&kartodromo->mutex_karts);
        while (kartodromo->karts_disponiveis == 0) {
            pthread_cond_wait(&kartodromo->cond_karts, &kartodromo->mutex_karts);
        }
        kartodromo->karts_disponiveis--;
        kartodromo->total_recursos_utilizados_karts++;
        pthread_mutex_unlock(&kartodromo->mutex_karts);
    } else {
        // Adultos pegam primeiro kart
        pthread_mutex_lock(&kartodromo->mutex_karts);
        while (kartodromo->karts_disponiveis == 0) {
            pthread_cond_wait(&kartodromo->cond_karts, &kartodromo->mutex_karts);
        }
        kartodromo->karts_disponiveis--;
        kartodromo->total_recursos_utilizados_karts++;
        pthread_mutex_unlock(&kartodromo->mutex_karts);

        // Agora pegar capacete
        pthread_mutex_lock(&kartodromo->mutex_capacetes);
        while (kartodromo->capacetes_disponiveis == 0) {
            pthread_cond_wait(&kartodromo->cond_capacetes, &kartodromo->mutex_capacetes);
        }

        printf("Piloto %s, idade: %d pegou um capacete.\n", piloto->nome, piloto->idade);
        kartodromo->capacetes_disponiveis--;
        kartodromo->total_recursos_utilizados_capacetes++;
        pthread_mutex_unlock(&kartodromo->mutex_capacetes);
    }

    sleep(rand() % 2 + 1);
    // 1s = 1h
    time_t saida = time(NULL);

    int tempo_espera = (int)difftime(saida, chegada);
    printf("Tempo de espera do piloto %s: %d horas\n", piloto->nome, tempo_espera);
    printf("VRUUUUUUM Piloto %s, idade: %d está na pista!\n", piloto->nome, piloto->idade);

    // Devolver capacete
    pthread_mutex_lock(&kartodromo->mutex_capacetes);
    kartodromo->capacetes_disponiveis++;
    pthread_cond_signal(&kartodromo->cond_capacetes); // sinaliza que tem capacete disponivel (comunica com cond_wait)
    pthread_mutex_unlock(&kartodromo->mutex_capacetes);

    // Devolver kart
    pthread_mutex_lock(&kartodromo->mutex_karts);
    kartodromo->karts_disponiveis++;
    pthread_cond_signal(&kartodromo->cond_karts); // sinaliza que tem kart disponivel (comunica com cond_wait)
    pthread_mutex_unlock(&kartodromo->mutex_karts);

    // Incrementar tempo total de espera
    pthread_mutex_lock(&kartodromo->mutex_tempo_espera);
    kartodromo->tempo_total_espera += tempo_espera;
    pthread_mutex_unlock(&kartodromo->mutex_tempo_espera);

    // todo: armazenar tempo de espera e tempo de corrida
    printf("Piloto %s, idade: %d saiu da pista!\n", piloto->nome, piloto->idade);

    return NULL;
}

void chegada_grupo(Kartodromo *k, int num_pilotos) {
    Piloto *pilotos[num_pilotos];
    PilotoArg *piloto_args[num_pilotos];

    for (int i = 0; i < num_pilotos; i++) {
        pilotos[i] = malloc(sizeof(Piloto));
        if (pilotos[i] == NULL) {
            perror("Erro ao alocar memória para piloto");
            exit(EXIT_FAILURE);
        }

        snprintf(pilotos[i]->nome, 50, "Piloto%d", i + 1);

        int idade = (rand() % 17) + 5; // Pseudo idade (para garantir ter mais crianças)
        if (idade > 18) {
            pilotos[i]->idade = (rand() % 33) + 18; // Garantir maior diversidade entre adultos
        } else {
            pilotos[i]->idade = idade;
        }

        pilotos[i]->prioridade = (pilotos[i]->idade <= 14) ? 1 : 0;

        piloto_args[i] = malloc(sizeof(PilotoArg));
        if (piloto_args[i] == NULL) {
            perror("Erro ao alocar memória para piloto_args");
            exit(EXIT_FAILURE);
        }

        // Montando estrutura para passar para a thread
        piloto_args[i]->piloto = pilotos[i];
        piloto_args[i]->kartodromo = k;

        // Criação da thread
        if (pthread_create(&pilotos[i]->thread, NULL, piloto_thread, (void *)piloto_args[i]) != 0) {
            perror("Erro ao criar thread");
            exit(EXIT_FAILURE);
        }
    }

    // Aguardando que todas as threads do grupo terminem
    for (int i = 0; i < num_pilotos; i++) {
        pthread_join(pilotos[i]->thread, NULL); // Espera a thread do piloto terminar
        free(pilotos[i]); // Libera a memória alocada
        free(piloto_args[i]); // Libera a memória do argumento
    }

    imprime_kartodromo(k);
}
