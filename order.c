#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "functions.h"


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
