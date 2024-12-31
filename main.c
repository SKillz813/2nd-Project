
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "functions.h"

int main() {
    Product catalog[PRODUCT_COUNT];
    initialize_catalog(catalog);

    int server_pipe[CLIENT_COUNT][2];
    int client_pipe[CLIENT_COUNT][2];

    for (int i = 0; i < CLIENT_COUNT; i++) {
        if (pipe(server_pipe[i]) == -1 || pipe(client_pipe[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < CLIENT_COUNT; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            client_process(i, server_pipe[i], client_pipe[i]);
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < CLIENT_COUNT; i++) {
        close(server_pipe[i][1]);
        close(client_pipe[i][0]);
    }

    for (int orders_remaining = CLIENT_COUNT * ORDERS_PER_CLIENT; orders_remaining > 0;) {
        for (int i = 0; i < CLIENT_COUNT; i++) {
            int product_id;
            if (read(server_pipe[i][0], &product_id, sizeof(int)) > 0) {
                handle_order(i, product_id, catalog, client_pipe[i]);
                orders_remaining--;
                sleep(1);
            }
        }
    }

    generate_report(catalog);

    for (int i = 0; i < CLIENT_COUNT; i++) {
        close(server_pipe[i][0]);
        close(client_pipe[i][1]);
    }

    return 0;
}

