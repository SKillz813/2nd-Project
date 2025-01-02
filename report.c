#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "functions.h"

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
