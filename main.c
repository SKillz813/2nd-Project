
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "functions.h"

int main() {
    Product catalog[PRODUCT_COUNT];
    // Αρχικοποίηση του καταλόγου προϊόντων
    initialize_catalog(catalog);

    int server_pipe[CLIENT_COUNT][2]; // Pipes για επικοινωνία των πελατών με το κατάστημα
    int client_pipe[CLIENT_COUNT][2]; // Pipes για αποστολή απαντήσεων στους πελάτες
    
    // Δημιουργία των pipes
    for (int i = 0; i < CLIENT_COUNT; i++) {
        if (pipe(server_pipe[i]) == -1 || pipe(client_pipe[i]) == -1) {
            perror("pipe"); // Εμφάνιση σφάλματος σε περίπτωση αποτυχίας
            exit(EXIT_FAILURE);
        }
    }
     // Εμφάνιση σφάλματος σε περίπτωση αποτυχίας
     //Δημιουργει μία νέα διεργασία για κάθε πελάτη
    for (int i = 0; i < CLIENT_COUNT; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork"); // Εμφάνιση σφάλματος σε περίπτωση αποτυχίας του fork
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Λειτουργία πελάτη
            client_process(i, server_pipe[i], client_pipe[i]);
            exit(EXIT_SUCCESS); // Τερματισμός της διεργασίας πελάτη
        }
    }
    //Κλείσιμο pipe στον Γονέα
    for (int i = 0; i < CLIENT_COUNT; i++) {
        close(server_pipe[i][1]);
        close(client_pipe[i][0]);
    }

    for (int orders_remaining = CLIENT_COUNT * ORDERS_PER_CLIENT; orders_remaining > 0;) { //Διαβάζει παραγγελίες από κάθε πελάτη και επεξεργαζεται μέχρι να ολοκληρωθούν όλες
        for (int i = 0; i < CLIENT_COUNT; i++) {
            int product_id;
            if (read(server_pipe[i][0], &product_id, sizeof(int)) > 0) {
                handle_order(i, product_id, catalog, client_pipe[i]);
                orders_remaining--;
                sleep(1);
            }
        }
    }
    // Δημιουργία αναφοράς μετά την ολοκλήρωση όλων των παραγγελιών
    generate_report(catalog);

    for (int i = 0; i < CLIENT_COUNT; i++) {
        close(server_pipe[i][0]);
        close(client_pipe[i][1]);
    }

    return 0;
}

