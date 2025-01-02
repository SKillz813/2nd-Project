#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "functions.h"


void client_process(int client_id, int server_pipe[2], int client_pipe[2]) {
    srand(time(NULL) ^ (getpid() << 16));

    close(server_pipe[0]);
    close(client_pipe[1]);

    for (int j = 0; j < ORDERS_PER_CLIENT; j++) {
        int product_id = rand() % PRODUCT_COUNT;
        write(server_pipe[1], &product_id, sizeof(int));

        char response[BUFFER_SIZE];
        read(client_pipe[0], response, BUFFER_SIZE);
        printf("Client %d received: %s\n", client_id, response);

        sleep(1);
    }

    close(server_pipe[1]);
    close(client_pipe[0]);
