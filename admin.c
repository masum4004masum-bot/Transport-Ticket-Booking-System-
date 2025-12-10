//admin.c

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

void showAvailableSeats() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("\nNo bookings yet. All %d seats are available:\n", MAX_SEATS);
        for(int i = 1; i <= MAX_SEATS; i++) {
            printf("%d ", i);
            if(i % 10 == 0) printf("\n");
        }
        printf("\n");
        return;
    }
    
    int bookedSeats[MAX_SEATS] = {0};
    struct Booking temp;
    
    while(fread(&temp, sizeof(struct Booking), 1, file)) {
        if(temp.seatNo >= 1 && temp.seatNo <= MAX_SEATS) {
            bookedSeats[temp.seatNo-1] = 1;
        }
    }
    fclose(file);
    
    printf("\nAvailable seats (%d available):\n", MAX_SEATS);
    int count = 0;
    for(int i = 0; i < MAX_SEATS; i++) {
        if(!bookedSeats[i]) {
            printf("%3d ", i+1);
            count++;
            if(count % 10 == 0) printf("\n");
        }
    }
    printf("\nTotal available: %d seats\n", count);
}

void editReservation() {
    int seatToEdit;
    printf("\nEnter seat number to edit: ");
    scanf("%d", &seatToEdit);
    
    FILE *file = fopen(FILENAME, "r+");
    if (file == NULL) {
        printf("\nNo reservations found in the system.\n");
        return;
    }
    
    struct Booking temp;
    int found = 0;
    long int pos;
    
    while(!feof(file)) {
        pos = ftell(file);
        if(fread(&temp, sizeof(struct Booking), 1, file) != 1) break;
        
        if(temp.seatNo == seatToEdit) {
            printf("\n--- Editing Seat %d ---\n", seatToEdit);
            printf("Current Passenger: %s\n", temp.name);
            printf("Current Route: %s to %s\n", temp.source, temp.destination);
            
            clearInputBuffer();
            printf("\nEnter NEW passenger name: ");
            fgets(temp.name, MAX_NAME_LENGTH, stdin);
            temp.name[strcspn(temp.name, "\n")] = '\0';
            
            printf("Enter NEW source: ");
            fgets(temp.source, MAX_NAME_LENGTH, stdin);
            temp.source[strcspn(temp.source, "\n")] = '\0';
            
            printf("Enter NEW destination: ");
            fgets(temp.destination, MAX_NAME_LENGTH, stdin);
            temp.destination[strcspn(temp.destination, "\n")] = '\0';
            
            fseek(file, pos, SEEK_SET);
            fwrite(&temp, sizeof(struct Booking), 1, file);
            printf("\nReservation for Seat %d updated successfully!\n", seatToEdit);
            found = 1;
            break;
        }
    }
    
    fclose(file);
    
    if(!found) {
        printf("\nNo reservation found for Seat %d\n", seatToEdit);
    }
}

void adminMenu() {
    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║            ADMINISTRATOR PANEL               ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    
    int choice;
    do {
        printf("\n1. Show Available Seats\n");
        printf("2. Edit/Update Reservation\n");
        printf("3. Return to Main Menu\n");
        printf("\nEnter your choice (1-3): ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                showAvailableSeats();
                break;
            case 2:
                editReservation();
                break;
            case 3:
                printf("\nReturning to Main Menu...\n");
                break;
            default:
                printf("\nInvalid choice! Please enter 1, 2, or 3.\n");
        }
    } while(choice != 3);
}