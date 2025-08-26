#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int n;
int numero_maldito;
pid_t* processes;

int generate_random_number(){
    //Funcion para cargar nueva semilla para el numero aleatorio
    srand(time(NULL) ^ getpid());
    return (rand() % n);
}

void take_shoot() {
    int my_numer = generate_random_number();

    printf("Generé el numero %d y el maldito es %d\n", my_numer, numero_maldito);

    if (my_numer == numero_maldito) {
        printf("Proceso morido\n");
        exit(EXIT_SUCCESS);
    }

    sleep(11);

    exit(EXIT_FAILURE);
}

void remove_from_processes() {
    pid_t term_id = wait(NULL);

    printf("Ha muerdo el proceso %d\n", term_id);

    printf("LISTA DE PROCESOS:\n");
    for (int i = 0; i < n; i++) {
        if (processes[i] == term_id) {
            processes[i] = 0;
        }
        printf("Proceso %d es %d\n", i, processes[i]);
    }
    
}

int main(int argc, char const *argv[]){

    signal(SIGCHLD, remove_from_processes);
    
    n = atoi(argv[1]);
	int rondas = atoi(argv[2]);
	numero_maldito = atoi(argv[3]);

    processes = malloc;

    for (int i = 0; i< n; i++) {
        pid_t process_id = fork();

        if (process_id) {
            printf("Hijo numero %d es %d\n", i, process_id);
            processes[i] = process_id;
        } else {
            signal(SIGTERM, take_shoot);
            sleep(10);
            exit(EXIT_FAILURE);
        }
    }
    
    printf("Soy padre\n");


    for (int k = 0; k < rondas; k++) {
        for (int p = 0; p < n; p++) {
            pid_t p_id = processes[p];

            if (!p_id) {
                printf("Proceso muerto: %d", p);
                continue;
            }

            kill(p_id, SIGTERM);
            sleep(1);
        }

    }

    int success_processes = 0;

    for (int i = 0; i < n; i++) {
        pid_t pid = processes[i];

        if (pid) {
            success_processes++;
            printf("Quedó el proceso %d\n", pid);
            kill(pid, SIGKILL);
        }
    }

    printf("Han ganado %d procesos\n", success_processes);

    return 0;
}
