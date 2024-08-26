#ifndef KARTODROMO_H
#define KARTODROMO_H

#include <pthread.h>

#define NUM_KARTS 10
#define NUM_CAPACETES 10
#define NUM_GRUPOS 2 // Número de grupos de pessoas que chegam durante o dia

typedef struct {
    char nome[50];
    int idade;
    pthread_t thread; // Identificador da thread
    int prioridade; // 1 para crianças até 14 anos, 0 para outros
} Piloto;

typedef struct {
    int karts_disponiveis;
    int capacetes_disponiveis;
    pthread_mutex_t mutex_karts;
    pthread_mutex_t mutex_capacetes;
    pthread_mutex_t mutex_tempo_espera;
    pthread_cond_t cond_karts;
    pthread_cond_t cond_capacetes;
    int total_recursos_utilizados_karts;
    int total_recursos_utilizados_capacetes;
    int tempo_total_espera;
    int criancas_menores_14;
} Kartodromo;

void inicializa_kartodromo(Kartodromo *k);
void destroi_kartodromo(Kartodromo *k);
void chegada_grupo(Kartodromo *k, int num_pilotos);
void imprime_kartodromo(Kartodromo *k);
void *piloto_thread(void *arg);

#endif
