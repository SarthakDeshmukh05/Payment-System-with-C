#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_USERS 100
#define MAX_STRING_LENGTH 50
#define MIN_PASSWORD_LENGTH 8
#define TRANSACTION_LIMIT 50
#define INACTIVITY_THRESHOLD 300

typedef struct {
    char username[MAX_STRING_LENGTH];
    char password[MAX_STRING_LENGTH];
    int tokens;
    time_t lastActivityTime;
} User;

User users[MAX_USERS];
int userCount = 0;

int isPasswordStrong(const char *password) {
    int minLength = MIN_PASSWORD_LENGTH;
    int hasUppercase = 0, hasLowercase = 0, hasDigit = 0, hasSpecialChar = 0;

    for (int i = 0; password[i] != '\0'; i++) {
        if (isupper(password[i])) {
            hasUppercase = 1;
        } else if (islower(password[i])) {
            hasLowercase = 1;
        } else if (isdigit(password[i])) {
            hasDigit = 1;
        } else {
            hasSpecialChar = 1;
        }
    }

    return (strlen(password) >= minLength) && hasUppercase && hasLowercase && hasDigit && hasSpecialChar;
}

void registerUser() {
    if (userCount >= MAX_USERS) {
        printf("User limit reached. Cannot register more users.\n");
        return;
    }

    printf("Enter your username: ");
    scanf("%s", users[userCount].username);

    char password[MAX_STRING_LENGTH];
    do {
        printf("Enter your password: ");
        scanf("%s", password);

        if (!isPasswordStrong(password)) {
            printf("Weak password. Password must be at least 8 characters long and include uppercase, lowercase, digit, and special character.\n");
        }
    } while (!isPasswordStrong(password));

    strcpy(users[userCount].password, password);
    users[userCount].tokens = 100;
    users[userCount].lastActivityTime = time(NULL);

    userCount++;
    printf("Registration successful!\n");
}

void makePayment(int senderIndex) {
    char username[MAX_STRING_LENGTH];
    int amount;
    printf("Enter recipient's username: ");
    scanf("%s", username);

    int recipientIndex = -1;
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            recipientIndex = i;
            break;
        }
    }

    if (recipientIndex == -1) {
        printf("Recipient not found.\n");
        return;
    }

    printf("Enter the amount of tokens to transfer: ");
    if (scanf("%d", &amount) != 1 || amount <= 0 || amount > users[senderIndex].tokens || amount > TRANSACTION_LIMIT) {
        printf("Invalid amount or exceeding transaction limit or insufficient tokens in the sender's account.\n");
        return;
    }

    users[senderIndex].tokens -= amount;
    users[recipientIndex].tokens += amount;
    printf("Payment successful!\n");
}

void checkBalance(int userIndex) {
    printf("Your token balance: %d\n", users[userIndex].tokens);
}

void adminModifyTokens() {
    char adminUsername[MAX_STRING_LENGTH];
    char adminPassword[MAX_STRING_LENGTH];
    printf("Enter admin username: ");
    scanf("%s", adminUsername);
    printf("Enter admin password: ");
    scanf("%s", adminPassword);

    // Implement more secure admin authentication here (e.g., hashing)

    if (strcmp(adminUsername, "admin") == 0 && strcmp(adminPassword, "adminpass") == 0) {
        char username[MAX_STRING_LENGTH];
        int amount;
        printf("Enter username to modify tokens: ");
        scanf("%s", username);

        int userIndex = -1;
        for (int i = 0; i < userCount; i++) {
            if (strcmp(users[i].username, username) == 0) {
                userIndex = i;
                break;
            }
        }

        if (userIndex != -1) {
            printf("Enter the amount of tokens to add (positive) or remove (negative): ");
            scanf("%d", &amount);

            users[userIndex].tokens += amount;
            printf("Tokens modified successfully!\n");
        } else {
            printf("User not found.\n");
        }
    } else {
        printf("Admin authentication failed. Access denied.\n");
    }
}

int authenticateUser(char *username, char *password) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return i; // Return the user index
        }
    }
    return -1; // User not found or incorrect password
}

void resetLastActivityTime(int userIndex) {
    users[userIndex].lastActivityTime = time(NULL);
}

int checkInactivity(int userIndex) {
    time_t currentTime = time(NULL);
    return difftime(currentTime, users[userIndex].lastActivityTime) > INACTIVITY_THRESHOLD;
}

int main() {
    int choice;
    while (1) {
        printf("\n1. Register\n2. Login\n3. Make Payment\n4. Check Balance\n5. Admin View\n6. Quit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please try again.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                {
                    char username[MAX_STRING_LENGTH];
                    char password[MAX_STRING_LENGTH];
                    printf("Enter your username: ");
                    scanf("%s", username);
                    printf("Enter your password: ");
                    scanf("%s", password);
                    
                    int userIndex = authenticateUser(username, password);
                    if (userIndex != -1) {
                        printf("Login successful!\n");

                        // Initialize and reset last activity time
                        resetLastActivityTime(userIndex);

                        while (1) {
                            printf("\n1. Make Payment\n2. Check Balance\n3. Logout\n");
                            printf("Enter your choice: ");
                            if (scanf("%d", &choice) != 1) {
                                printf("Invalid input. Please try again.\n");
                                while (getchar() != '\n'); // Clear input buffer
                                continue;
                            }
                            switch (choice) {
                                case 1:
                                    makePayment(userIndex);
                                    resetLastActivityTime(userIndex);
                                    break;
                                case 2:
                                    checkBalance(userIndex);
                                    resetLastActivityTime(userIndex);
                                    break;
                                case 3:
                                    printf("Logged out.\n");
                                    break;
                                default:
                                    printf("                                    Invalid choice. Please try again.\n");
                            }

                            // Check for inactivity and logout if necessary
                            if (checkInactivity(userIndex)) {
                                printf("Logout due to inactivity.\n");
                                break;
                            }

                            if (choice == 3) break;
                        }
                    } else {
                        printf("Login failed. User not found or incorrect password.\n");
                    }
                }
                break;
            case 4:
                {
                    char username[MAX_STRING_LENGTH];
                    printf("Enter your username: ");
                    scanf("%s", username);
                    int userIndex = -1;
                    for (int i = 0; i < userCount; i++) {
                        if (strcmp(users[i].username, username) == 0) {
                            userIndex = i;
                            break;
                        }
                    }
                    if (userIndex != -1) {
                        checkBalance(userIndex);
                    } else {
                        printf("User not found.\n");
                    }
                }
                break;
            case 5:
                adminModifyTokens();
                break;
            case 6:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}


