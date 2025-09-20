#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>


int server_socket;
int client_socket;
struct sockaddr_un server_addr;
struct sockaddr_un client_addr;
uint slen = sizeof(server_addr);
uint clen = sizeof(client_addr);

int calcular(const char *expresion) {
    int num1, num2, resultado;
    char operador;

    // Usamos sscanf para extraer los dos números y el operador de la expresión
    if (sscanf(expresion, "%d%c%d", &num1, &operador, &num2) != 3) {
        printf("Formato incorrecto\n");
        return 0;  // En caso de error, retornamos 0.
    }

    // Realizamos la operación según el operador
    switch (operador) {
        case '+':
            resultado = num1 + num2;
            break;
        case '-':
            resultado = num1 - num2;
            break;
        case '*':
            resultado = num1 * num2;
            break;
        case '/':
            if (num2 != 0) {
                resultado = num1 / num2;
            } else {
                printf("Error: División por cero\n");
                return 0;  // Si hay división por cero, retornamos 0.
            }
            break;
        default:
            printf("Operador no reconocido\n");
            return 0;  // Si el operador no es válido, retornamos 0.
    }

    return resultado;
}

void handle_child() {
    int client = accept(server_socket, (struct sockaddr *)&client_addr, &clen);

    if (client == -1) {
        perror("Accept Error");
        exit(EXIT_FAILURE);
    }

    printf("CHILD: Acepté al cliente\n");

    exit(EXIT_SUCCESS);
}

int main() {
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "unix_socket");
    unlink(server_addr.sun_path);

    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (bind(server_socket, (struct sockaddr *) &server_addr, slen) == -1) {
        perror("Error");
        exit(EXIT_FAILURE);
    }
    if (listen(server_socket, 1) == -1) {
        perror("Error");
        exit(EXIT_FAILURE);
    }


    while(1) {
        printf(".");
        int conn;
        struct timeval tv = { .tv_sec = 2, .tv_usec = 0 };  // tiempo de espera 0: no bloquea, revisa y regresa de inmediato
        fd_set socket_set;

        FD_ZERO(&socket_set);
        FD_SET(server_socket, &socket_set);

        int ret = select(server_socket + 1, &socket_set, NULL, NULL, &tv);

        if (ret == -1) {
            perror("select");
            exit(EXIT_FAILURE);
        }

        if (ret) {
            printf("servidor: nuevo cliente...\n");

            if (!fork()) {
                handle_child();
            }

            sleep(1);

            int client = accept(server_socket, (struct sockaddr *)&client_addr, &clen);

            if (client == -1) {
                perror("Accept Error");
                exit(EXIT_FAILURE);
            }

            printf("PADRE: Acepté al cliente\n");

        }
    }

    // COMPLETAR. Este es un ejemplo de funcionamiento básico.
    // La expresión debe ser recibida como un mensaje del cliente hacia el servidor.
    const char *expresion = "10+5";  
    int resultado = calcular(expresion);
    printf("El resultado de la operación es: %d\n", resultado);
    exit(0);
}
