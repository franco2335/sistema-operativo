#include <signal.h> /* constantes como SIGINT*/
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int attemps = 5;

void child_hand() {
	attemps--;
	printf("Ya va!\n");
}

void parent_hand() {
	pid_t child_pid = wait(NULL);
}

int main(int argc, char* argv[]) {
	char* bin_name = argv[1];

	pid_t child_pid = fork();

	if (child_pid == 0) {
		signal(SIGURG, child_hand);

		while (attemps) continue;

		pid_t my_parent = getppid(); 

		execvp(bin_name, argv+1);

	} else {
		signal(SIGINT, parent_hand);

		for (int i = 0; i < 5; i++) {
			sleep(1);
			printf("sup!\n");
			kill(child_pid, SIGURG);
		}
	}

	return 0;
}
