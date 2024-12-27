#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

// Σταθερές
#define PRODUCT_COUNT 20
#define CLIENT_COUNT 5
#define ORDERS_PER_CLIENT 10
#define BUFFER_SIZE 256

// Δομή για τα προϊόντα
typedef struct {
    char description[50];
    float price;
    int item_count;
    int requests;
    int sold;
    char failed_clients[BUFFER_SIZE];
} Product;

// Δηλώσεις συναρτήσεων
void initialize_catalog(Product catalog[]);
void handle_order(int client_id, int product_id, Product catalog[], int client_pipe[2]);
void generate_report(Product catalog[]);

#endif // FUNCTIONS_H

