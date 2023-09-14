#define MAX_CONEXOES 10
#define SEM_CONEXOES_DISPONIVEIS -1
int proxima_iteracao = 0;

MPI_Request conexoes[MAX_CONEXOES];
int conexoes_id[MAX_CONEXOES];

void inicializa_array_conexoes()
{
	for(int i = 0; i < MAX_CONEXOES; i++)
	{
		conexoes[i] = MPI_REQUEST_NULL;
		conexoes_id[i] = -1;
	}
}

int get_indice_nova_comunicacao()
{
	if(comunicacoes_em_andamento == MAX_CONEXOES)
		return SEM_CONEXOES_DISPONIVEIS;
		
	for(int i = 0; i < MAX_CONEXOES; i++)
		if(conexoes[i] == MPI_REQUEST_NULL)
			return i;

	return SEM_CONEXOES_DISPONIVEIS;
}

void verifica_conexoes_concluidas()
{
	int indice_conexao;
	do
	{
		for(int i = 0; i < MAX_CONEXOES; i++)
		{
			if(conexoes_id[i] != -1 && MPI_Test(conexoes[i])
			{
				escreve_em_disco();
				conexoes_id[i] = -1;
			}
		}
		sleep(2 segundos);
	}
	while(indice_conexao = get_indice_nova_comunicacao()) == SEM_CONEXOES_DISPONIVEIS)
		
	MPI_Irecv(buff, count, MPI_Float, 0, proxima_iteracao, MPI_COMM_WORLD, &conexoes[indice_conexao]);
	conexoes_id[indice_conexao] = proxima_iteracao;
	proxima_iteracao++;
}


void looping()
{
	for (int it=1; it<=st; it++) {
	 	tSim = it * dt;
		if (tSim >= tOut) {
			temp++;
			verifica_conexoes_concluidas()
		}
	}
	MPI_WaitAll();
}
