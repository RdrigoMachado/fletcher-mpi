#define MAX_CONEXOES    10
#define LIVRE            0
#define TRANSFERINDO     1

#define CHEIO           -1

int status[MAX_CONEXOES];
int posicao[MAX_CONEXOES];
MPI_Request request[MAX_CONEXOES];
float* buffers[MAX_CONEXOES];

void init()
{
    for(int i = 0; i < MAX_CONEXOES; i++)
	{
		request[i] = MPI_REQUEST_NULL;
		status[i]   = LIVRE;
        buffers[i]  = NULL;
        posicao[i]  = 0
	}
}

void escreve_em_disco(int id_request)
{  
    char nome_arquivo[128];
    fopen(nome_arquivo, "w+");
    fwrite((void *) buffers[id_request], sizeof(float), tamanho, arquivo);
    fclose(arquivo);
}

void atualiza_status()
{
    for(int i = 0; i < MAX_CONEXOES; i++)
    {
		if(request[i] == MPI_REQUEST_NULL && status[i] == TRANSFERINDO)
        {
            escreve_em_disco(i);
            status[i] = LIVRE;           
        }
    }
}

int get_indice_nova_comunicacao()
{
    atualiza_status();

	for(int i = 0; i < MAX_CONEXOES; i++)
		if(conexoes[i] == MPI_REQUEST_NULL && status[i] == LIVRE)
			return i;
    
    int proximo_indice;
    MPI_Status *status;
    MPI_Waitany(MAX_CONEXOES, request, &proximo_indice, status);
	
    return proximo_indice;
}

void nova_transferencia(int id_request, int ordem, int tamanho)
{
    status[id_request]   = TRANSFERINDO;    
    posicao[id_request]  = ordem;
    buffers[id_request] = malloc(sizeof(float) * tamanho); 
    MPI_Irecv((void *) buffers[id_request], tamanho, MPI_FLOAT, ordem, ordem, MPI_COMM_WORLD, &(request[id_request]));
}

void MPI_escrita_disco(int sx, int sy, int sz, char* nome_arquivo,
                const int st,  const float dtOutput, const float dt, float dx, float dy, float dz)
{
  {
    int tamanho = sx * sy * sz;
    float *onda = malloc(sizeof(float) * tamanho); 
    FILE *arquivo = abrirArquivo(nome_arquivo);
    int ixStart=0;
    int ixEnd=sx-1;
    int iyStart=0;
    int iyEnd=sy-1;
    int izStart=0;
    int izEnd=sz-1;

    float tSim=0.0;
    int nOut=1;
    float tOut=nOut*dtOutput;
    int itCnt = 1;
  }
    int ordem = 1;
    int id_request_atual = CHEIO;
    id_request_atual = get_indice_nova_comunicacao();
    

   
    nova_transferencia(id_request_atual, ordem, tamanho);
    ordem++;

    int temp = 1;
    for (int it=1; it<=st; it++) 
    {
        tSim=it*dt;
        if (tSim >= tOut) 
        {
            id_request_atual = get_indice_nova_comunicacao();
            nova_transferencia(id_request_atual, ordem, tamanho);
            ordem++;
            tOut=(++nOut)*dtOutput;
            itCnt++;
            temp++;
        }
    }
    salvarInformacoesExecucao(ixStart, ixEnd, iyStart, iyEnd, izStart, izEnd, dx, dy, dz, dt, itCnt, nome_arquivo);
    finalizar(arquivo);
}