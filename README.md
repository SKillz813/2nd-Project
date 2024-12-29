# 2nd-Project
# README.md

## Περιγραφή Προγράμματος
Αυτό το πρόγραμμα υλοποιεί ένα e-shop που εξυπηρετεί παραγγελίες μέσω ανώνυμων αγωγών (anonymous pipes). Το κατάστημα διαθέτει έναν κατάλογο προϊόντων με περιορισμένη διαθεσιμότητα και επικοινωνεί με πελάτες που υποβάλλουν αιτήματα για αγορά προϊόντων.

## Τεχνικές Πληροφορίες
- Χρησιμοποιούνται οι κλήσεις συστήματος `pipe()` και `fork()` για τη διαδιεργασιακή επικοινωνία (IPC).
- Οι πελάτες επικοινωνούν με το κατάστημα μέσω ανώνυμων διοχετεύσεων.
- Το κατάστημα διαχειρίζεται παραγγελίες, δεσμεύει προϊόντα και καταγράφει αποτυχημένες παραγγελίες.
- Το πρόγραμμα παράγει αναφορά στο τέλος εκτέλεσης με πληροφορίες για την απόδοση του συστήματος.

## Δομή του Αποθετηρίου
- `src/`
  - `main.c`: Ο κύριος κώδικας του προγράμματος.
  - `functions.c`: Υλοποίηση βοηθητικών συναρτήσεων.
  - `functions.h`: Δηλώσεις συναρτήσεων.

## Οδηγίες Μεταγλώττισης και Εκτέλεσης
1. **Μεταγλώττιση**:
   Εκτελέστε την εντολή:
   ```sh
   make
   ```
   Το εκτελέσιμο αρχείο θα δημιουργηθεί στον φάκελο `build/` με το όνομα `eshop`.

2. **Εκτέλεση**:
   Μεταβείτε στον φάκελο `build/` και εκτελέστε:
   ```sh
   ./eshop
   ```

3. **Καθαρισμός**:
   Για να διαγράψετε τα παραγόμενα αρχεία:
   ```sh
   make clean
   ```

## Παραδείγματα Χρήσης
```sh
$ ./eshop
Client 0 received: Order successful for Product 3, $12.50
Client 1 received: Order failed for Product 7: Out of stock
