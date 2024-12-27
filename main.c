#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define PRODUCT_COUNT 20
#define CLIENT_COUNT 5
#define ORDERS_PER_CLIENT 10
#define BUFFER_SIZE 256

// Struct for product
typedef struct {
    char description[50];
    float price;
    int item_count;
    int requests;
    int sold;
    char failed_clients[BUFFER_SIZE];
} Product;

// Function prototypes
void initialize_catalog(Product catalog[]);
void handle_order(int client_id, int product_id, Product catalog[], int client_pipe[2]);
void generate_report(Product catalog[]);

int main() {
    Product catalog[PRODUCT_COUNT];
    initialize_catalog(catalog);

    int server_pipe[CLIENT_COUNT][2]; // Pipes for reading orders
    int client_pipe[CLIENT_COUNT][2]; // Pipes for sending responses

    // Create pipes
    for (int i = 0; i < CLIENT_COUNT; i++) {
        if (pipe(server_pipe[i]) == -1 || pipe(client_pipe[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Fork clients
    for (int i = 0; i < CLIENT_COUNT; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Client process
            srand(time(NULL) ^ (getpid() << 16)); // Seed random number generator

            close(server_pipe[i][0]); // Close unused read end of server pipe
            close(client_pipe[i][1]); // Close unused write end of client pipe

            for (int j = 0; j < ORDERS_PER_CLIENT; j++) {
                int product_id = rand() % PRODUCT_COUNT;
                write(server_pipe[i][1], &product_id, sizeof(int));

                char response[BUFFER_SIZE];
                read(client_pipe[i][0], response, BUFFER_SIZE);
                printf("Client %d received: %s\n", i, response);

                sleep(1); // Wait between orders
            }

            close(server_pipe[i][1]);
            close(client_pipe[i][0]);
            exit(EXIT_SUCCESS);
        }
    }

    // Parent process (server)
    for (int i = 0; i < CLIENT_COUNT; i++) {
        close(server_pipe[i][1]); // Close unused write end of server pipe
        close(client_pipe[i][0]); // Close unused read end of client pipe
    }

    for (int orders_remaining = CLIENT_COUNT * ORDERS_PER_CLIENT; orders_remaining > 0; ) {
        for (int i = 0; i < CLIENT_COUNT; i++) {
            int product_id;
            if (read(server_pipe[i][0], &product_id, sizeof(int)) > 0) {
                handle_order(i, product_id, catalog, client_pipe[i]);
                orders_remaining--;
                sleep(1); // Simulate processing time
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

void initialize_catalog(Product catalog[]) {
    for (int i = 0; i < PRODUCT_COUNT; i++) {
        snprintf(catalog[i].description, 50, "Product %d", i);
        catalog[i].price = (float)(rand() % 100 + 1);
        catalog[i].item_count = 2;
        catalog[i].requests = 0;
        catalog[i].sold = 0;
        catalog[i].failed_clients[0] = '\0';
    }
}

void handle_order(int client_id, int product_id, Product catalog[], int client_pipe[2]) {
    char response[BUFFER_SIZE];

    catalog[product_id].requests++;
    if (catalog[product_id].item_count > 0) {
        catalog[product_id].item_count--;
        catalog[product_id].sold++;
        snprintf(response, BUFFER_SIZE, "Order successful for %s, $%.2f", catalog[product_id].description, catalog[product_id].price);
    } else {
        snprintf(response, BUFFER_SIZE, "Order failed for %s: Out of stock", catalog[product_id].description);
        snprintf(catalog[product_id].failed_clients + strlen(catalog[product_id].failed_clients), BUFFER_SIZE - strlen(catalog[product_id].failed_clients), "Client %d ", client_id);
    }

    write(client_pipe[1], response, strlen(response) + 1);
}

void generate_report(Product catalog[]) {
    printf("\n--- Final Report ---\n");

    int total_orders = 0;
    int successful_orders = 0;
    int failed_orders = 0;
    float total_revenue = 0.0f;

    for (int i = 0; i < PRODUCT_COUNT; i++) {
        printf("Product: %s\n", catalog[i].description);
        printf("  Requests: %d\n", catalog[i].requests);
        printf("  Sold: %d\n", catalog[i].sold);
        printf("  Failed Clients: %s\n", catalog[i].failed_clients[0] ? catalog[i].failed_clients : "None");

        total_orders += catalog[i].requests;
        successful_orders += catalog[i].sold;
        failed_orders += (catalog[i].requests - catalog[i].sold);
        total_revenue += (catalog[i].sold * catalog[i].price);
    }

    printf("\nSummary:\n");
    printf("  Total Orders: %d\n", total_orders);
    printf("  Successful Orders: %d\n", successful_orders);
    printf("  Failed Orders: %d\n", failed_orders);
    printf("  Total Revenue: $%.2f\n", total_revenue);
}

