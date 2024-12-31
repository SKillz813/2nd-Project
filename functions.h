#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define PRODUCT_COUNT 20
#define CLIENT_COUNT 5
#define ORDERS_PER_CLIENT 10
#define BUFFER_SIZE 256

typedef struct {
    char description[50];
    float price;
    int item_count;
    int requests;
    int sold;
    char failed_clients[BUFFER_SIZE];
} Product;

void initialize_catalog(Product catalog[]);
void handle_order(int client_id, int product_id, Product catalog[], int client_pipe[2]);
void generate_report(Product catalog[]);
void client_process(int client_id, int server_pipe[2], int client_pipe[2]);

#endif // FUNCTIONS_H


