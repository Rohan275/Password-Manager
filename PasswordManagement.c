#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define USER_LIMIT 5
#define PASSWORD_LIMIT 10
// maximum of 5 user profiles and 10 passwords per user profile allowed

// a template for a password
// has 3 properties -> the name of the service/website,
// the username, and the password associated with it
struct password {
    char service[30];
    char username[20];
    char password[20];
};

// template for a user
// contains:
// - name and email of the user
// - the master password -> to login to the system
// - the number of passwords the user has saved
// - an array of passwords -> the actual passwords
struct user {
    char name[20];
    char email[50];
    char masterPassword[20];
    int numOfPasswords;
    struct password passwords[10];
};

struct user user[USER_LIMIT]; // an array of user objects -> they'll hold our user profiles
int numOfUsers = 0; // number of users in the system rn

// shows all the details -> service name + username + password
void showDetailed(struct password *p) {
    printf("-------------------------------------------------------\n");
    printf("Service name: %s\n", p->service);
    printf("Username: %s\n", p->username);
    printf("Password: %s\n", p->password);
    printf("-------------------------------------------------------\n");
}

// displays the brief details of a password -> Service name + password
void showBrief(struct password *p) {
    printf("Service name: %s\t Password: %s\n", p->service, p->password);
}

// shows all the details of a user
// personal details + all the passwords
// if the user has no passwords, it will display a message
// if there are passwords, it will display the brief details of each password
// the user can choose to view the details of a password by choosing the serial number
void showOverview(struct user *u) {
    printf("Name: %s\n", u->name);
    printf("Email: %s\n", u->email);
    printf("-------------------------------------------------------\n");
    if (u->numOfPasswords == 0) {
        printf("No passwords added yet.\n");
    } else {
        for (int i = 0; i < u->numOfPasswords; i++) {
            printf("%d. ", i + 1);
            showBrief(&u->passwords[i]);
        }    
        printf("-------------------------------------------------------\n");
        int service; char ch = 'y';
        do {
            printf("Enter serial number of service to view details: ");
            scanf("%d", &service); 
            if (service >= 1 && service <= u->numOfPasswords) {
                showDetailed(&u->passwords[service - 1]);
            } else {
                printf("Invalid serial number.\n");
            }
            printf("Try again? (y/n): "); getchar(); scanf("%c", &ch);
        } while (ch == 'y' || ch == 'Y');
    }
}

// adds a password to the user's account
// if the user has less than 10 passwords, it will add the password
// if the user has 10 passwords, it will display a message
void addPassword(struct user *u) {
    printf("-------------------------------------------------------\n");
    if (u->numOfPasswords < PASSWORD_LIMIT) {
        printf("Enter service name: ");
        scanf("%s", u->passwords[u->numOfPasswords].service);
        printf("Enter username: ");
        scanf("%s", u->passwords[u->numOfPasswords].username);
        printf("Enter password: ");
        scanf("%s", u->passwords[u->numOfPasswords].password);
        u->numOfPasswords++;
    } else {
        printf("You have reached the maximum number of passwords.\n");
    }
}

// searches for a specific password and displays the details
// if the password is not found, it will display a message
// if the password is found, it will display the details
// loops through all the passwords to find the password
void searchPassword(struct user *u) {
    if (u->numOfPasswords == 0) {
        printf("No passwords added yet.\n");
    } else {
        char service[30]; int found = 0;
        printf("Enter service name: "); scanf("%s", service);
        for (int i = 0; i < u->numOfPasswords; i++) {
            if (strcmp(u->passwords[i].service, service) == 0) {
                found = 1;
                showDetailed(&u->passwords[i]);
            }
        }
        if (found == 0) {
            printf("No such password saved.\n");
        }
    }
}

// deletes a password of a user
// takes the name of the service as input
// if the password is found, it will delete the password
void deletePassword(struct user *u) {
    if (u->numOfPasswords == 0) {
        printf("No passwords added yet.\n");
    } else {
        char service[30]; int deleted = 0;
        printf("Enter service name: "); scanf("%s", service);
        for (int i = 0; i < u->numOfPasswords; i++) {
            if (strcmp(u->passwords[i].service, service) == 0) {
                for (int j = i; j < u->numOfPasswords - 1; j++) {
                    u->passwords[j] = u->passwords[j + 1];
                }
                u->numOfPasswords--;
                deleted = 1;
            }
        }
        if (deleted == 0) {
            printf("No such password saved.\n");
        }
    }
}   

int fileExists(char filename[]) {
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        return 0;
    } else {
        fclose(fp);
        return 1;
    }
}

// exports the user's passwords to a file
void exportPasswords(struct user *u) {
    if (u->numOfPasswords == 0) {
        printf("No passwords added yet.\n");
    } else {
        FILE *fp;
        // creating a text file with the user's name as the file name
        char filename[35]; strcpy(filename, u->name);
        strcat(filename, ".txt");
        fp = fopen(filename, "w"); // open file in write mode
        if (fp == NULL) {
            // if the file cannot be opened
            printf("Error opening file.\n");
        } else {
            fprintf(fp, "%s ", u->name);
            fprintf(fp, "%s ", u->email);
            fprintf(fp, "%s\n", u->masterPassword);
            for (int i = 0; i < u->numOfPasswords; i++) {
                fprintf(fp, "%s ", u->passwords[i].service);
                fprintf(fp, "%s ", u->passwords[i].username);
                fprintf(fp, "%s\n", u->passwords[i].password);
            }
            fclose(fp); // close the text file
            printf("Passwords exported successfully.\n");
        }
    }
}

void logout() {
    printf("Logging out...\n");
}

// menu for once the user is logged in
void showMenu(struct user *u) {
    int loggedInUserChoice = 0;
    printf("Welcome %s!\n", u->name);
    do {
        printf("1. View all passwords\n");
        printf("2. Add a new password\n");
        printf("3. View a specific password\n");
        printf("4. Delete a password\n");
        printf("5. Logout\n");
        printf("6. Export all passwords\n");
        printf("Enter your choice (1-5): "); 
        int option = 0; scanf("%d", &option);
        if (option >= 1 && option <= 5) {
            system("cls");
        }
        switch(option) {
            case 1: showOverview(u);
                    break;
            case 2: addPassword(u);
                    break;
            case 3: searchPassword(u);
                    break;
            case 4: deletePassword(u);
                    break;
            case 5: logout();
                    break;
            case 6: exportPasswords(u);
                    break;
            default: printf("Invalid option. Please try again.\n");
                     break;
        }
        printf("Press 1 to go to dashboard, any key to go main menu: "); getchar();
        scanf("%d", &loggedInUserChoice);
    } while (loggedInUserChoice == 1);
}

// adds a user to the system
void createUser() {
    if (numOfUsers < USER_LIMIT) {
        printf("Enter name: "); scanf("%s", user[numOfUsers].name);
        printf("Enter email: "); scanf("%s", user[numOfUsers].email);
        printf("Enter master password: "); scanf("%s", user[numOfUsers].masterPassword);
        user[numOfUsers].numOfPasswords = 0; // setting initial no. of passwords to 0
        numOfUsers++; // incrementing the number of users
    } else {
        printf("You have reached the maximum number of users.\n");
    }
}

// logging the user in
void login() {
    char username[20]; char password[20];
    printf("Enter email: "); scanf("%s", username);
    printf("Enter password: "); scanf("%s", password);
    for (int i = 0; i < numOfUsers; i++) {
        // checks if password and username match, username is the email entered basically
        if (strcmp(username, user[i].email) == 0 && strcmp(password, user[i].masterPassword) == 0) {
            showMenu(&user[i]);
            return;
        }
    }
    printf("Invalid username or password.\n");
}

int main() {
    int ch = 0;
    do {
        // main menu for the system
        printf("1. Create a new user account\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice (1-3): ");
        int option = 0; scanf("%d", &option);
        
        switch(option) {
            case 1: createUser();
                    break;
            case 2: login();
                    break;
            case 3: printf("Exiting...\n"); exit(0);
                    break;
            default: printf("Invalid option. Please try again.\n");
                     break;
        }
        printf("Go to Main Menu? (1 for Yes, any key to exit): "); 
        scanf("%d", &ch); getchar();
    } while (ch == 1);
    return 0;
}