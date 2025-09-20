#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define READ 0
#define WRITE 1

int main(int argc, char *argv[]) {

    int n = atoi(argv[1]); // cant hijos
    int c = atoi(argv[2]); // valor 
    int s = atoi(argv[3]); // hijo inicial

    int pipes[n][2];
    for (int i = 0; i < n; i++) {
        pipe(pipes[i]);
    }

    int padre_pipe[2];
    pipe(padre_pipe);

    pid_t pid[n];

    for (int i = 0; i < n; i++) {
        pid[i] = fork();

        if (pid[i] == 0 && i != s) {      // Si no soy el que empieza
            for (int j = 0; j < n; j++) {    
                if (j != i) close(pipes[j][WRITE]);     // Cierro todos los de escritura menos el mio
                if (j != (i-1+n)%n) close(pipes[j][READ]); // todos los de lectura menos el del anterior
            }
            close(padre_pipe[READ]);
            close(padre_pipe[WRITE]);

            int num;
            while (1) {
                read(pipes[(i-1+n)%n][READ], &num, sizeof(int));    // Leo del anterior
                num++;
                write(pipes[i][WRITE], &num, sizeof(int));  // Escribo en el siguiente
            }
        }

        if (i == s && pid[i] == 0) { // Si soy el que empieza
            int num;

            for (int j = 0; j < n; j++) {   // Hago lo mismo pero no cierro el pipe del padre
                if (j != i) close(pipes[j][WRITE]);
                if (j != (i-1+n)%n) close(pipes[j][READ]);
            }

            int secreto = c + 1 + rand() % 20;  // Creo num aleatorio
            printf("Hijo %d: secreto= %d\n", i, secreto);
            fflush(stdout); 
            read(pipes[s][READ], &num, sizeof(int));    // El padre me manda el valor inicial

            while (1) {
                if (num >= secreto) {   // Si el num es más grande que el secreto hay que terminar
                    write(padre_pipe[WRITE], &num, sizeof(int));    // Se lo mando al padre
                    while (1) pause;
                }
                num++;
                write(pipes[i][WRITE], &num, sizeof(int));
                read(pipes[(i-1+n)%n][READ], &num, sizeof(int));
            }
        }
    }


    for (int i = 0; i < n; i++) {   // Cierro todos los pipes READ de mis hijos y el de escritura del que empieza
        close(pipes[i][READ]);
        if (i != s){ 
            close(pipes[i][WRITE]);
        }
    }

    close(padre_pipe[WRITE]);
    write(pipes[s][WRITE], &c, sizeof(int)); // el padre tiene q escribir en el pipe del hijo, si escribi en el suyo y que el hijo lea de ahi el read de abajo no se traba 

    int resultado;
    read(padre_pipe[READ], &resultado, sizeof(int)); 
    printf("Resultado final: %d\n", resultado);

    for (int i = 0; i < n; i++) {
        if (pid[i] > 0) kill(pid[i], SIGKILL);  // Mata a los hijos
    }

    for (int i = 0; i < n; i++) wait(NULL); // Espera a que mueran

    return 0;
}
