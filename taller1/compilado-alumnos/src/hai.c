#include <signal.h> /* constantes como SIGINT*/
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int attempts = 5;

void child_hand() {
	printf("Ya va!\n");
	attempts--;
}

void parent_hand() {
	pid_t my_child = wait(NULL);
}

int main(int argc, char* argv[]) {
	pid_t my_child = fork();

	char* bin_name = argv[1];

	if (!my_child) {
		signal(SIGURG, child_hand);
		while (attempts) continue;

		pid_t my_parent = getppid(); 

		kill(my_parent, SIGINT);

		execvp(bin_name, argv+1);
	} else {
		signal(SIGINT, parent_hand);

		for (int i = 0; i < 5; i++) {
			sleep(1);
			printf("sup!\n");
			kill(my_child, SIGURG);
		}
	}

	return 0;
}
