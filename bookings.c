//bookings.c


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEATS 35
#define MAX_NAME_LENGTH 50
#define FILENAME "bookings.txt"

struct Booking {
    char name[MAX_NAME_LENGTH];
    int seatNo;
    char source[MAX_NAME_LENGTH];
    char destination[MAX_NAME_LENGTH];
};

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int isSeatAvailable(int seatNo) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) return 1;
    
    struct Booking temp;
    while(fread(&temp, sizeof(struct Booking), 1, file)) {
        if(temp.seatNo == seatNo) {
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    return 1;
}

int getTotalBookings() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) return 0;
    
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fclose(file);
    return size / sizeof(struct Booking);
}

void bookSeat() {
    if (getTotalBookings() >= MAX_SEATS) {
        printf("\nSorry! All seats are booked.\n");
        return;
    }
    
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        file = fopen(FILENAME, "w");
        if (file == NULL) {
            printf("\nError: Cannot access bookings database.\n");
            return;
        }
    }
    
    struct Booking newBooking;
    
    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║               NEW BOOKING                    ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    
    clearInputBuffer();
    printf("\nEnter passenger full name: ");
    fgets(newBooking.name, MAX_NAME_LENGTH, stdin);
    newBooking.name[strcspn(newBooking.name, "\n")] = '\0';
    
    printf("Enter seat number (1-%d): ", MAX_SEATS);
    scanf("%d", &newBooking.seatNo);
    
    if (newBooking.seatNo < 1 || newBooking.seatNo > MAX_SEATS) {
        printf("\nError: Seat number must be between 1 and %d\n", MAX_SEATS);
        fclose(file);
        return;
    }
    
    if (!isSeatAvailable(newBooking.seatNo)) {
        printf("\nError: Seat %d is already booked!\n", newBooking.seatNo);
        printf("Please choose a different seat.\n");
        fclose(file);
        return;
    }
    
    clearInputBuffer();
    printf("Enter boarding location/city: ");
    fgets(newBooking.source, MAX_NAME_LENGTH, stdin);
    newBooking.source[strcspn(newBooking.source, "\n")] = '\0';
    
    printf("Enter destination city: ");
    fgets(newBooking.destination, MAX_NAME_LENGTH, stdin);
    newBooking.destination[strcspn(newBooking.destination, "\n")] = '\0';
    
    fwrite(&newBooking, sizeof(struct Booking), 1, file);
    fclose(file);
    
    printf("\n══════════════════════════════════════════════\n");
    printf("          BOOKING CONFIRMED!\n");
    printf("══════════════════════════════════════════════\n");
    printf("Passenger: %s\n", newBooking.name);
    printf("Seat No.:  %d\n", newBooking.seatNo);
    printf("Route:     %s → %s\n", newBooking.source, newBooking.destination);
    printf("Booking ID: T%d%03d\n", newBooking.seatNo, rand() % 1000);
    printf("══════════════════════════════════════════════\n");
}

void bookingMenu() {
    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║            BOOKING SYSTEM                    ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    
    int choice;
    do {
        printf("\n1. Make a New Booking\n");
        printf("2. Check Available Seats\n");
        printf("3. Return to Main Menu\n");
        printf("\nEnter your choice (1-3): ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                bookSeat();
                break;
            case 2:
                {
                    FILE *file = fopen(FILENAME, "r");
                    if (file == NULL) {
                        printf("\nAll %d seats are available!\n", MAX_SEATS);
                    } else {
                        fseek(file, 0, SEEK_END);
                        int size = ftell(file);
                        fclose(file);
                        int booked = size / sizeof(struct Booking);
                        printf("\nAvailable seats: %d out of %d\n", MAX_SEATS - booked, MAX_SEATS);
                    }
                }
                break;
            case 3:
                printf("\nReturning to Main Menu...\n");
                break;
            default:
                printf("\nInvalid choice! Please enter 1, 2, or 3.\n");
        }
    } while(choice != 3);
}