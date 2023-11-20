#include "../MPI.envio.h"


pthread_mutex_t mutexEnvio[NUM_THREADS];
pthread_cond_t condEnvio[NUM_THREADS];
pthread_t thread;
int passo_atual = 0;
int tamanho_onda;
float *buffer;

// Producer thread function
void* sender() {
    MPI_Send((void *) buffer, tamanho_onda, MPI_FLOAT, 1, passo_atual, MPI_COMM_WORLD);
}


void inicializar_envio(int sx, int sy, int sz)
{
    tamanho_onda = sx * sy * sz;
    buffer = malloc(sizeof(float) * tamanho_onda);
}

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr, SlicePtr p)
{
    memcpy(buffer, ondaPtr, tamanho_onda * sizeof(float));
    passo_atual++;
    if(passo_atual == 1)
    {
        pthread_create(&thread, NULL, &sender, NULL);
    }
    else
    {
        pthread_join(thread, NULL);
        pthread_create(&thread, NULL, &sender, NULL);

    }
    p->itCnt++;
}

void finalizar_envio()
{
    pthread_join(thread, NULL);
    MPI_Finalize();
    free(buffer);
}