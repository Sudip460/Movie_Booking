#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MOVIES 5
#define MAX_SEATS 50

// Structure to store movie details
typedef struct {
    int movie_id;
    char title[50];
    int seats_available;
    int booked_seats[MAX_SEATS];  // Array to track booked seats
} Movie;

// Structure for each ticket node in the linked list
typedef struct TicketNode {
    int ticket_id;
    char movie_title[50];
    int seat_number;
    struct TicketNode *next;
} TicketNode;

// Structure for the queue
typedef struct QueueNode {
    int ticket_id;
    int movie_choice;
    int seat_choice;
    struct QueueNode *next;
} QueueNode;

// Queue with front and rear pointers
typedef struct {
    QueueNode *front, *rear;
} Queue;

Movie movies[MAX_MOVIES];
TicketNode *head = NULL;
int ticket_count = 0;

// Function to initialize movies
void initializeMovies() {
    strcpy(movies[0].title, "Avengers: Endgame");
    movies[0].movie_id = 1;
    movies[0].seats_available = 30;

    strcpy(movies[1].title, "Inception");
    movies[1].movie_id = 2;
    movies[1].seats_available = 20;

    strcpy(movies[2].title, "The Dark Knight");
    movies[2].movie_id = 3;
    movies[2].seats_available = 25;

    strcpy(movies[3].title, "Interstellar");
    movies[3].movie_id = 4;
    movies[3].seats_available = 40;

    strcpy(movies[4].title, "Titanic");
    movies[4].movie_id = 5;
    movies[4].seats_available = 15;

    // Initialize booked seats for each movie
    for (int i = 0; i < MAX_MOVIES; i++) {
        for (int j = 0; j < MAX_SEATS; j++) {
            movies[i].booked_seats[j] = 0;  // 0 means seat is available
        }
    }
}

// Function to initialize the queue
void initializeQueue(Queue *q) {
    q->front = q->rear = NULL;
}

// Function to check if the queue is empty
int isQueueEmpty(Queue *q) {
    return q->front == NULL;
}

// Function to enqueue a ticket request
void enqueue(Queue *q, int ticket_id, int movie_choice, int seat_choice) {
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    newNode->ticket_id = ticket_id;
    newNode->movie_choice = movie_choice;
    newNode->seat_choice = seat_choice;
    newNode->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

// Function to dequeue a ticket request
QueueNode* dequeue(Queue *q) {
    if (isQueueEmpty(q)) {
        return NULL;
    }
    QueueNode *temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    return temp;
}

// Function to add a ticket to the linked list
void addTicketToList(int ticket_id, char *movie_title, int seat_number) {
    TicketNode *newTicket = (TicketNode *)malloc(sizeof(TicketNode));
    newTicket->ticket_id = ticket_id;
    strcpy(newTicket->movie_title, movie_title);
    newTicket->seat_number = seat_number;
    newTicket->next = NULL;

    if (head == NULL) {
        head = newTicket;
    } else {
        TicketNode *temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newTicket;
    }
}

// Function to display movies
void displayMovies() {
    printf("Available Movies:\n");
    for (int i = 0; i < MAX_MOVIES; i++) {
        printf("%d. %s - Seats Available: %d\n", movies[i].movie_id, movies[i].title, movies[i].seats_available);
    }
}

// Function to check if a seat is already booked
int isSeatBooked(Movie *movie, int seat_number) {
    return movie->booked_seats[seat_number - 1];
}

// Function to book a seat (mark it as booked)
void bookSeat(Movie *movie, int seat_number) {
    movie->booked_seats[seat_number - 1] = 1;
    movie->seats_available--;
}

// Function to process ticket booking requests in the queue
void processBookings(Queue *q) {
    while (!isQueueEmpty(q)) {
        QueueNode *request = dequeue(q);
        if (request != NULL) {
            Movie *selected_movie = &movies[request->movie_choice - 1];
            
            if (selected_movie->seats_available > 0 && !isSeatBooked(selected_movie, request->seat_choice)) {
                bookSeat(selected_movie, request->seat_choice);
                addTicketToList(request->ticket_id, selected_movie->title, request->seat_choice);
                printf("Ticket ID %d booked for %s at Seat %d\n", request->ticket_id, selected_movie->title, request->seat_choice);
            } else {
                printf("Seat %d not available for Ticket ID %d, Movie: %s\n", request->seat_choice, request->ticket_id, selected_movie->title);
            }
            free(request);
        }
    }
}

// Function to book a ticket (adds request to queue)
void bookTicket(Queue *q) {
    int movie_choice, seat_choice;

    displayMovies();

    printf("Enter the Movie ID to book a ticket: ");
    scanf("%d", &movie_choice);

    if (movie_choice < 1 || movie_choice > MAX_MOVIES) {
        printf("Invalid movie choice.\n");
        return;
    }

    Movie *selected_movie = &movies[movie_choice - 1];

    printf("Enter seat number (1-%d): ", MAX_SEATS);
    scanf("%d", &seat_choice);

    if (seat_choice < 1 || seat_choice > MAX_SEATS || isSeatBooked(selected_movie, seat_choice)) {
        printf("Seat %d is not available for %s.\n", seat_choice, selected_movie->title);
        return;
    }

    ticket_count++;
    enqueue(q, ticket_count, movie_choice, seat_choice);
    printf("Booking request added to queue. Ticket ID: %d\n", ticket_count);
}

// Function to display booked tickets
void viewTickets() {
    if (head == NULL) {
        printf("No tickets booked yet.\n");
        return;
    }

    TicketNode *temp = head;
    printf("Booked Tickets:\n");
    while (temp != NULL) {
        printf("Ticket ID: %d, Movie: %s, Seat Number: %d\n", temp->ticket_id, temp->movie_title, temp->seat_number);
        temp = temp->next;
    }
}

int main() {
    Queue bookingQueue;
    initializeQueue(&bookingQueue);
    initializeMovies();
    int choice;

    do {
        printf("\n--- Movie Ticket Booking System ---\n");
        printf("1. Display Movies\n");
        printf("2. Book a Ticket\n");
        printf("3. Process Bookings\n");
        printf("4. View Booked Tickets\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayMovies();
                break;
            case 2:
                bookTicket(&bookingQueue);
                break;
            case 3:
                processBookings(&bookingQueue);
                break;
            case 4:
                viewTickets();
                break;
            case 5:
                printf("Thank you for using the Movie Ticket Booking System!\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}
