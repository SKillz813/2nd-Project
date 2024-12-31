#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define PRODUCT_COUNT 20 // Συνολικός αριθμός προϊόντων
#define CLIENT_COUNT 5  // Συνολικός αριθμός πελατών
#define ORDERS_PER_CLIENT 10  // Συνολικός αριθμός πελατών
#define BUFFER_SIZE 256 // Μέγεθος buffer για μηνύματα

// Δομή για την αναπαράσταση προϊόντων
typedef struct {
    char description[50]; // Περιγραφή προϊόντος
    float price;
    int item_count; // Διαθέσιμα τεμάχια
    int requests;
    int sold;
    char failed_clients[BUFFER_SIZE]; // Λίστα πελατών που απέτυχαν να αγοράσουν
} Product;

void initialize_catalog(Product catalog[]); // Αρχικοποίηση καταλόγου προϊόντων
void handle_order(int client_id, int product_id, Product catalog[], int client_pipe[2]);
void generate_report(Product catalog[]); // Δημιουργία αναφοράς
void client_process(int client_id, int server_pipe[2], int client_pipe[2]);// Λειτουργία πελάτη

#endif // FUNCTIONS_H


