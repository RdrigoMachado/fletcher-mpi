#include "../MPI.envio.h"


pthread_mutex_t mutexEnvio[NUM_THREADS];
pthread_cond_t condEnvio[NUM_THREADS];
pthread_t threads[NUM_THREADS];
int passo_atual = 0;
int ordem[NUM_THREADS];
int exit_flag = 0;
int tamanho_onda;
float *buffer[2];
int ultimo_id = 1;

// Producer thread function
void* sender(void* args) {

    int* myArgs = (int*)args;
    int id = myArgs[0];
    int destino = myArgs[1];
    int passo_anterior = -1;
    //sincroniza
    pthread_mutex_lock(&(mutexEnvio[id]));
    pthread_cond_signal(&(condEnvio[id]));
    pthread_mutex_unlock(&(mutexEnvio[id]));


    pthread_mutex_lock(&(mutexEnvio[id]));
    printf("sender %d\n", id);

    while(1)
    {
        printf("%d Aguardando cond\n", id);
        pthread_cond_wait(&(condEnvio[id]), &(mutexEnvio[id]));
        if(exit_flag == EXIT && passo_anterior == ordem[id])
        {
          printf("ending %d\n", id);
            pthread_mutex_unlock(&(mutexEnvio[id]));
            return NULL;
        }
        printf("from %d  to %d: sending %d\n", id, destino, ordem[id]);
        MPI_Send((void *) buffer[id], tamanho_onda, MPI_FLOAT, destino, ordem[id], MPI_COMM_WORLD);
        passo_anterior = ordem[id];
    }
}


void inicializar_envio(int sx, int sy, int sz)
{
    tamanho_onda = sx * sy * sz;

    for(int i = 0; i < NUM_THREADS; i++)
    {
        int *args= malloc(sizeof(int) * 2);
        args[0] = i;
        args[1] = i+1;
        
        pthread_mutex_init(&(mutexEnvio[i]), NULL);
        pthread_cond_init(&(condEnvio[i]), NULL);
        buffer[i] = malloc(sizeof(float) * tamanho_onda);
        pthread_mutex_lock(&(mutexEnvio[i]));
        
        if (pthread_create(&threads[i], NULL, &sender, (void*)args) != 0) 
        {
            perror("Failed to create thread");
        }

        pthread_cond_wait(&(condEnvio[i]), &(mutexEnvio[i]));
        pthread_mutex_unlock(&(mutexEnvio[i]));

    }
}

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr, SlicePtr p)
{
    int id = 0;
    if(ultimo_id == 0)
    {
      id = ultimo_id = 1;
    }
    else{
      ultimo_id = 0;
    }
    
    pthread_mutex_lock(&(mutexEnvio[id]));
    memcpy(buffer[id], ondaPtr, tamanho_onda * sizeof(float));
    passo_atual++;
    ordem[id] = passo_atual;
    p->itCnt++;
    printf("signaling to %d that step %d is ready\n", id, passo_atual);
    pthread_cond_signal(&(condEnvio[id]));
    pthread_mutex_unlock(&(mutexEnvio[id]));
}

void finalizar_envio()
{
    exit_flag = EXIT;
    for(int i = 0; i < NUM_THREADS; i++)
    {
        pthread_mutex_lock(&(mutexEnvio[i]));
        pthread_cond_signal(&(condEnvio[i]));
        pthread_mutex_unlock(&(mutexEnvio[i]));
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Failed to join thread");
        }
        pthread_mutex_destroy(&(mutexEnvio[i]));
        pthread_cond_destroy(&(condEnvio[i]));
        free(buffer[i]);
    }
    MPI_Finalize();

}