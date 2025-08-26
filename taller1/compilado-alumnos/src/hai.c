#include <signal.h> /* constantes como SIGINT*/
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int intentos = 5;

void ya_va_hand() {
	pid_t my_parent = getppid();

	printf("Quedan %d intentos\n", intentos);
	intentos--;
	printf("Ya va!\n");

	if (!intentos)
		kill(my_parent, SIGINT);

}

void interrupt_hand() {
	pid_t child = wait(NULL);
}

int main(int argc, char* argv[]) {
	char* bin_path = argv[1];

	int bin_args_q = argc - 1;

	pid_t my_child = fork();

	if (!my_child) {
		signal(SIGURG, ya_va_hand);
		printf("Soy hijo\n");
		sleep(10);
	} else {
		signal(SIGINT, interrupt_hand);
		sleep(1);
		printf("Soy padre\n");
		for (int i = 0; i < 5; i++) {
			sleep(1);
			printf("sup!\n");
			kill(my_child, SIGURG);
		}
		//exit(EXIT_SUCCESS);
	}

	execvp(bin_path, argv++);
	return 0;
}
