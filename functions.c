#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "functions.h"

// Αρχικοποίηση του καταλόγου προϊόντων
void initialize_catalog(Product catalog[]) {
    for (int i = 0; i < PRODUCT_COUNT; i++) {
        snprintf(catalog[i].description, 50, "Product %d", i);
        catalog[i].price = (float)(rand() % 100 + 1); // Τυχαία τιμή
        catalog[i].item_count = 2;
        catalog[i].requests = 0;
        catalog[i].sold = 0;
        catalog[i].failed_clients[0] = '\0';  // Αρχικοποίηση κενής λίστας
    }
}

// Διαχείριση παραγγελίας πελάτη
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

// Δημιουργία αναφοράς προϊόντων
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
}

