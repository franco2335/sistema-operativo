#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "constants.h"

int generate_random_number(){
	return (rand() % 50);
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
	pid_t *children = malloc(sizeof(*children) * n);
    
	int **pipe_list = malloc(sizeof(int*) * (n));

	for (int i = 0; i < n - 1; i++) {
		pipe_list[i] = malloc(sizeof(int) * 2);

		pipe(pipe_list[i]);

		printf("File descs: %d y %d\n", pipe_list[i][0], pipe_list[i][1]);
	}

	for (int i = 0; i < n; i++) {
		pid_t pid = fork();
        if (i == start, pid == 0){
			
        
		}
		if (pid == 0) {
			while (1) {
				int my_number;
				read(pipe_list[i][PIPE_READ], &my_number, sizeof(my_number));
				my_number++;
				write(pipe_list[i+1][PIPE_READ], &my_number,sizeof(my_number));
			}
		} else {
			children[i] = pid;
		}
	}
}

