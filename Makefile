# Ορισμός του compiler και των σημαιών
CC = gcc                     # Ο compiler που θα χρησιμοποιηθεί
CFLAGS = -Wall -Wextra -g    # Σημαίες για προειδοποιήσεις και debugging

# Στόχοι
TARGET = program             # Το όνομα του τελικού εκτελέσιμου αρχείου
OBJS = main.o functions.o    # Λίστα με τα αρχεία αντικειμένου (object files)

# Προεπιλεγμένος στόχος
all: $(TARGET)

# Κανόνας για τη δημιουργία του εκτελέσιμου αρχείου
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)  # Σύνδεση των object files και δημιουργία του εκτελέσιμου

# Κανόνας για τη μεταγλώττιση του main.c
main.o: main.c functions.h
	$(CC) $(CFLAGS) -c main.c             # Μεταγλώττιση του main.c σε αρχείο αντικειμένου

# Κανόνας για τη μεταγλώττιση του functions.c
functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c functions.c        # Μεταγλώττιση του functions.c σε αρχείο αντικειμένου

# Κανόνας για καθαρισμό των παραγόμενων αρχείων
clean:
	rm -f $(TARGET) $(OBJS)               # Διαγραφή του εκτελέσιμου και των αρχείων αντικειμένου

# Κανόνας για την εκτέλεση του προγράμματος
run: all
	./$(TARGET)                          

