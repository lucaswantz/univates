#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/param.h>

float obterUsoDaCPUPeloProcesso(int pid, int tempoAtualizacao) {
	
    int vNULLI;
    char vNULLC[100];
    char nomeArquivo[100];
	
    int pid_stat;
	
    int timeUsuario;
    int timeKernel;
    float timeTotal1;
    float timeTotal2;
    float percent_usage;
	
    FILE* arquivo_stat;

	// Grava string com nome do arquivo stat do processo selecionado
    sprintf(nomeArquivo, "/proc/%d/stat", pid);

    // Abre aquivo /proc/PID/stat.
    if ((arquivo_stat = fopen(nomeArquivo, "r")) != NULL ){
        fscanf(arquivo_stat, "%d\t%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d", &pid_stat, vNULLC, vNULLC, &vNULLI, &vNULLI, &vNULLI, &vNULLI, &vNULLI, &vNULLI, &vNULLI, &vNULLI, &vNULLI, &vNULLI, &timeUsuario, &timeKernel);
        fclose(arquivo_stat);

        // Soma total de time.
        timeTotal1 = (float)(timeUsuario + timeKernel);
    }
    sleep(tempoAtualizacao);

    // Abre aquivo /proc/PID/stat.
    if ( (arquivo_stat = fopen(nomeArquivo, "r")) != NULL ){
        fscanf(arquivo_stat, "%d\t%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d", &vNULLI, vNULLC, vNULLC, &vNULLI, &vNULLI, &vNULLI, &vNULLI, &vNULLI, &vNULLI, &vNULLI, &vNULLI, &vNULLI, &vNULLI, &timeUsuario, &timeKernel);
        fclose(arquivo_stat);

        // Soma total de time.
        timeTotal2 = (float)(timeUsuario + timeKernel);
    }
	
    percent_usage = (( (timeTotal2 - timeTotal1) / (HZ * tempoAtualizacao) ) * 100.00);

    return percent_usage;
}

int obtemMemoriaTotalProcesso(int pid)
{
    int vNULLI;
    FILE* statm;
    char nomeArquivo[100];
    int numeroDePaginasMemoria;
    int memoriaEmKb = 0;

    // Obtém o nome do arquivo stat.
    sprintf(nomeArquivo, "/proc/%d/statm", pid);

    // Abre o arquivo statm do processo.
    if ((statm = fopen(nomeArquivo, "r")) != NULL ) {
        fscanf(statm, "%d\t%d\t%d", &vNULLI, &numeroDePaginasMemoria, &vNULLI);

        fclose(statm);

        // Obtém o uso de memória em Kbytes.
        memoriaEmKb = numeroDePaginasMemoria * (getpagesize() / 1024);
    }
	
    // obter a memoria
    memoriaEmKb = (memoriaEmKb * 100) / GetRamInKB();

    return memoriaEmKb;
}

int GetRamInKB(void)
{
    FILE *meminfo = fopen("/proc/meminfo", "r");
    char line[256];
    while(fgets(line, sizeof(line), meminfo))
    {
        int ram;
        if(sscanf(line, "MemTotal: %d kB", &ram) == 1)
        {
            fclose(meminfo);
            return ram;
        }
    }

    fclose(meminfo);
    return -1;
}

int main (int argc, char *argv[])
{
    int pid;
    int tempo_atualizacao = 2;
    int i = 0;

    printf("ID |   PID   |  Mémoria  |   CPU    |\n");
    printf(" \n");

    pid = atoi(argv[1]);

    // LAÇO DE REPETIÇÃO QUE FICA IMPRIMINDO AS INFORMAÇÕES
    while(1) {

        if (i < 10){
            printf("0");
        }
        printf("%d |   %d  |    %d   %  |  %.1f   %  \n", i, pid, obtemMemoriaTotalProcesso(pid), obterUsoDaCPUPeloProcesso(pid, tempo_atualizacao));
        i++;
    }
}
