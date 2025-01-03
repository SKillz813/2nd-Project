# Ορισμός compiler και σημαιών
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Φάκελοι 
SRC_DIR = src # Πηγαίος κώδικας
INC_DIR = include # Ηeader files
OBJ_DIR = obj # Αρχεία αντικειμένων 

# Πηγαία και Αντικείμενα αρχεία
SOURCES = $(wildcard $(SRC_DIR)/*.c) # Όλα τα αρχεία .c στο φάκελο src
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES)) # Τα αντικείμενα αρχεία που παράγονται από τα πηγαία

# Εκτελέσιμο Αρχείο
OUT = project

# Default Target
all: $(OUT) # Δημιουργία εκτελέσιμου αρχείου

# Σύνδεση αντικειμένων για δημιουργία εκτελέσιμου
$(OUT): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Μεταγλώττιση κάθε πηγαίου αρχείου σε αντικέιμενο
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Δημιουργία φακέλου αντικειμένων αν δεν υπάρχει
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Καθαρισμός των δημιουργημένων αρχείων
clean:
	rm -rf $(OBJ_DIR) $(OUT)

# Phony Target 
.PHONY: all clean
