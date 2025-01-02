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
