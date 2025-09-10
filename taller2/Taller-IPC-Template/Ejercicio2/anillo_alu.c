#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "constants.h"
#include <syscall.h>
#include <errno.h>
#include <signal.h>

int generate_random_number(){
	return (rand() % 50);
}

int **pipe_list;
int id;
int max_childs;

void start_child(){
	int number;
	
	read(pipe_list[0][PIPE_WRITE], &number, sizeof(int));


}

void next_child(){
	int number;
	
	read(pipe_list[id+1][PIPE_WRITE], &number, sizeof(int));

	if (id == max_childs){
		write(pipe_list[1], &number, sizeof(int));
	}
	write(pipe_list[(id+1)][PIPE_WRITE], &number+1, sizeof(int));
	//kill(children[(id+1)%max_childs], SIGUSR2);
}

int main(int argc, char **argv)
{	
	//Funcion para cargar nueva semilla para el numero aleatorio
	srand(time(NULL));

	int status, pid, n, start, buffer;
	n = atoi(argv[1]);
	buffer = atoi(argv[2]);
	start = atoi(argv[3]);

	n = 3;

	if (argc != 4){ printf("Uso: anillo <n> <c> <s> \n"); exit(0);}
    
  	/* COMPLETAR */
    printf("Se crearán %i procesos, se enviará el caracter %i desde proceso %i \n", n, buffer, start);
    
    /* COMPLETAR */

	/* Crear los pipes de la ronda */
	/* Crear el pipe del padre a los hijos, uno solo todos lo conocen */
	/* Crear los hijos, asi heredan los fd  */
	/* Los hijos deben cerrar los pipes con respecto a sus hermanos, excepto el write al siguiente y el read del anterior */
	/* Todos los hijos handlean una señal para saber que son ellos quienes comienzan la ronda */
	/* El padre escribe en el pipe[star][PIPE_WRITE] el buffer */
	/* El padre envia una señal al hijo start */
	/* El hijo start handlea la señal que le envio su padre */
		/* Saca un numero al azar */
		/* Lee el numero escrito en el pipe */
		/* Suma 1 al numero */
		/* Escribe ese numero en el pipe del siguiente */
		/* Manda una señal al siguiente */

	pid_t *children = malloc(sizeof(*children) * n);
    
	pipe_list = malloc(sizeof(int*) * (n));

	for (int i = 0; i < n - 1; i++) {
		pipe_list[i] = malloc(sizeof(int) * 2);

		pipe(pipe_list[i]);

		printf("File descs: %d y %d\n", pipe_list[i][0], pipe_list[i][1]);
	}

	/* el pipe i va para la conexion entre el hijo i y el hijo i+1 */
	/* el pipe n va para la conexion entre el ultimo hijo y el primer hijo */
	/* Faltaria otro pipe para la conexion entre el padre y el que empieza la ronda */
	/* Al iniciar la ronda el padre envia al hijo que comienza la ronda el numero con el cual inicia */
	/* Faltaria agregar pipes del padre a los hijos para esa comunicacion */
	/* Luego cada hijo excepto el que empezo la ronda cierra el pipe con su padre */

	for (int i = 0; i < n; i++) {
		pid_t pid = fork();
		if (pid == 0){
			id = i;
			signal(SIGUSR1, start_child);
			signal(SIGUSR2, next_child);
		}
		else {
			children[i] = pid;
		}
	}
	write (pipe_list[0][PIPE_WRITE], &buffer, sizeof(int));
	kill(children[start], SIGUSR1);
	pause();
}

