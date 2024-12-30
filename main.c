#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

// Ορισμός σταθερών για τον αριθμό προϊόντων, πελατών, παραγγελιών ανά πελάτη και μέγεθος buffer
#define PRODUCT_COUNT 20
#define CLIENT_COUNT 5
#define ORDERS_PER_CLIENT 10
#define BUFFER_SIZE 256

// Struct για το προΐον
typedef struct {
    char description[50];
    float price;
    int item_count;
    int requests;
    int sold;
    char failed_clients[BUFFER_SIZE];
} Product;

// Πρωτότυπα συναρτήσεων
void initialize_catalog(Product catalog[]); // Αρχικοποίηση καταλόγου προϊόντων
void handle_order(int client_id, int product_id, Product catalog[], int client_pipe[2]);
void generate_report(Product catalog[]);

int main() {
    Product catalog[PRODUCT_COUNT];
    initialize_catalog(catalog);

    int server_pipe[CLIENT_COUNT][2]; // Pipes για ανάγνωση παραγγελιών από τους πελάτες
    int client_pipe[CLIENT_COUNT][2]; // Pipes για αποστολή απαντήσεων στους πελάτες

    // Δημιουργια των pipe
    for (int i = 0; i < CLIENT_COUNT; i++) {
        if (pipe(server_pipe[i]) == -1 || pipe(client_pipe[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    
    // Δημιουργία διεργασιών πελατών (clients) (fork)
    for (int i = 0; i < CLIENT_COUNT; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Αρχή λειτουργίας του client
            srand(time(NULL) ^ (getpid() << 16)); // θετει αρχική τιμή για επιλογή τυχαίου αριθμού

            close(server_pipe[i][0]); // Κλείνει τα αχρησιμοποίητα read του server_pipe
            close(client_pipe[i][1]); // Κλείνει τα αχρησιμοποίητα write του client_pipe

            for (int j = 0; j < ORDERS_PER_CLIENT; j++) {
                int product_id = rand() % PRODUCT_COUNT;  // Τυχαία επιλογή προϊόντος
                write(server_pipe[i][1], &product_id, sizeof(int)); // Αποστολή παραγγελίας στο server

                char response[BUFFER_SIZE];
                read(client_pipe[i][0], response, BUFFER_SIZE); // Ανάγνωση απόκρισης από τον server
                printf("Client %d received: %s\n", i, response);

                sleep(1); // Περιμενει 1 δευτερολεπτο αναμεσα στις παραγγελιες 
            }

            close(server_pipe[i][1]);
            close(client_pipe[i][0]);
            exit(EXIT_SUCCESS);
        }
    }

    // Διαχείριση από τον server (parent)
    for (int i = 0; i < CLIENT_COUNT; i++) {
        close(server_pipe[i][1]); // Κλείνει τα αχρησιμοποίητα write
        close(client_pipe[i][0]); // Κλείνει τα αχρησιμοποίητα read
    }
      // Επεξεργασία παραγγελιών
    for (int orders_remaining = CLIENT_COUNT * ORDERS_PER_CLIENT; orders_remaining > 0; ) {
        for (int i = 0; i < CLIENT_COUNT; i++) {
            int product_id;
            if (read(server_pipe[i][0], &product_id, sizeof(int)) > 0) {
                handle_order(i, product_id, catalog, client_pipe[i]);
                orders_remaining--;
                sleep(1); // Προσομοίωση χρόνου επεξεργασίας
            }
        }
    }

    generate_report(catalog);
    // Κλείσιμο pipes
    for (int i = 0; i < CLIENT_COUNT; i++) {
        close(server_pipe[i][0]);
        close(client_pipe[i][1]);
    }

    return 0;
}
// Αρχικοποίηση του καταλόγου προϊόντων
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
// Διαχείριση παραγγελίας από πελάτη
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
// Δημιουργία αναφοράς για τα προϊόντα
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

