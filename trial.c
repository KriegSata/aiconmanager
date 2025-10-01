#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h> 
#include <conio.h> 

#define MAX 100
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"

void customerMenu();
void adminMenu();
void registerAdmin();
void registerCustomer();
void registerTechnician();
void viewServiceRequests();
void approveOrDeclineRequest();
void checkRequestStatus(const char* customerCode);
void clearScreen();
void loadSampleData();
void loadAdminData();
void loadUserData();
void loadAirconData();
void loadTechnicianData();
void saveAdminData();
void saveUserData();
void saveAirconData();
void saveTechnicianData();
void takePassword(char password[], int maxLength);
int isValidInteger(const char *input);
int isStrongPassword(const char *password);
void displayMainMenuWithDelay();
void displayFrontPage();
void displayMainMenu();
void logAction(const char* userType, const char* userCode, const char* action);
void viewLog();
void addAirconUnit();
void editAirconUnit();
void approveOrDeclineAccounts();
void technicianMenu();
void viewAllAircons();
void loadServiceRequestData();
void saveServiceRequestData();
void updateServiceRequestStatus(int index, const char* status);
void cancelServiceBooking(const char* customerCode);
void checkAccountStatus();  
void displayTimeBar();  
void assignAirconsToCustomer();  
void loadAirconAssignments();  
void saveAirconAssignments();  
void bookService(const char* customerCode);  
void updateAirconMaintenanceStatus(const char* technicianCode);  
void submitHelpTicket(const char* customerCode);  
void viewHelpTickets();  
int validateEmail(const char* email);  
void saveUserCredentials();  
void generateRandomCode(char* code, int length);  
void saveRandomGeneratedCode(const char* customerCode);  
void saveRandomGeneratedCodeToRGCStat(const char* customerCode, const char* trackingCode);  
void saveCollegeData(const char* customerCode, const char* college, const char* building, const char* floor, const char* room);  
void viewNotifications(const char* customerCode);  
void deassignAirconUnits();  
void generateUniqueTrackingCode(char* code, int length);  
void viewAssignedServiceRequests(const char* technicianCode);  
void viewAllApprovedServiceRequests(const char* technicianCode);  
void collaborateWithOtherTechnicians(const char* technicianCode);  

typedef struct {
    char unitNo[20];
    char brand[50];
    char model[50];
    char type[30];
    char building[50];  
    char floor[10];     
    char room[10];      // Add this line
    char lastMaintenance[20];
} AirconUnit;

typedef struct {
    char customerCode[50];  // Add this line
    char serviceType[20];
    char unitNo[20];
    char status[20];
    char reason[100];
    char eta[30]; 
    char lastUpdated[30];
    char cancelledAt[30];  
    char technicianCode[30];  
} ServiceRequest;

typedef struct {
    char username[30];
    char password[30];
    int isApproved; 
} AdminAccount;

typedef struct {
    char username[30];
    char password[30];
    char customerCode[50];  
    int isApproved; 
    char firstName[50];  
    char middleName[50];  
    char surname[50];  
    char email[100];  
    char mobileNumber[20];  
    char employeeId[20];  
    char registrationTime[30];  
    char college[50];  
    char building[50];  
    char floor[10];  
    char room[10];  
} CustomerAccount;

typedef struct {
    char technicianCode[30];
    char password[30];
    int isApproved;
} TechnicianAccount;

typedef struct {
    char customerCode[50];
    char unitNo[20];
} AirconAssignment;

AirconUnit airconUnits[MAX];
ServiceRequest serviceRequests[MAX];
AdminAccount adminAccounts[MAX];
CustomerAccount customerAccounts[MAX];
TechnicianAccount technicianAccounts[MAX];
AirconAssignment airconAssignments[MAX];
int totalUnits = 0;
int totalRequests = 0;
int totalAdmins = 0;
int totalCustomers = 0;
int totalTechnicians = 0;
int totalAssignments = 0;

void delay(int seconds) {
    int milli_seconds = seconds * 1000;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void takePassword(char password[], int maxLength) {
    int i = 0;
    char ch;
    while (1) {
        ch = getch();
        if (ch == '\r') { 
            password[i] = '\0';
            break;
        } else if (ch == '\b') { 
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else if (i < maxLength - 1) {
            password[i++] = ch;
            printf("*");
        }
    }
    printf("\n");
}

void loadAdminData() {
    FILE *file = fopen("admins.txt", "r");
    if (file == NULL) {
        printf(RED "Error: Could not open admins.txt file.\n" RESET);
        return;
    }

    while (fscanf(file, "%[^,],%[^,],%d\n", adminAccounts[totalAdmins].username, 
                   adminAccounts[totalAdmins].password, 
                   &adminAccounts[totalAdmins].isApproved) == 3) {
        totalAdmins++;
    }

    fclose(file);
}

void loadUserData() {
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        printf(RED "Error: Could not open users.txt file.\n" RESET);
        return;
    }

    while (fscanf(file, "%[^,],%d\n", customerAccounts[totalCustomers].customerCode, 
                   &customerAccounts[totalCustomers].isApproved) == 2) {
        totalCustomers++;
    }

    fclose(file);

    // Load additional customer details from usercred.txt
    file = fopen("usercred.txt", "r");
    if (file == NULL) {
        printf(RED "Error: Could not open usercred.txt file.\n" RESET);
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        char type[20];
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]\n",
               type,
               customerAccounts[totalCustomers].customerCode,
               customerAccounts[totalCustomers].password,
               customerAccounts[totalCustomers].firstName,
               customerAccounts[totalCustomers].middleName,
               customerAccounts[totalCustomers].surname,
               customerAccounts[totalCustomers].email,
               customerAccounts[totalCustomers].mobileNumber,
               customerAccounts[totalCustomers].employeeId,
               customerAccounts[totalCustomers].registrationTime,
               customerAccounts[totalCustomers].college,
               customerAccounts[totalCustomers].building,
               customerAccounts[totalCustomers].floor,
               customerAccounts[totalCustomers].room,
               &customerAccounts[totalCustomers].isApproved);

        if (strcmp(type, "Customer") == 0) {
            totalCustomers++;
        }
    }

    fclose(file);
}

void saveUserData() {
    FILE *file = fopen("users.txt", "w");
    if (file == NULL) {
        printf(RED "Error: Could not open users.txt file for writing.\n" RESET);
        return;
    }

    for (int i = 0; i < totalCustomers; i++) {
        fprintf(file, "%s,%d\n", customerAccounts[i].customerCode, customerAccounts[i].isApproved);
    }

    fclose(file);
}

void loadAirconData() {
    FILE *file = fopen("aircon.txt", "r");
    if (file == NULL) {
        printf(RED "Error: Could not open aircon.txt file.\n" RESET);
        return;
    }

    while (fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s\n", airconUnits[totalUnits].unitNo, 
                   airconUnits[totalUnits].brand, airconUnits[totalUnits].model, 
                   airconUnits[totalUnits].type, airconUnits[totalUnits].building, 
                   airconUnits[totalUnits].floor, airconUnits[totalUnits].lastMaintenance) == 7) {
        totalUnits++;
    }

    fclose(file);
}

void loadTechnicianData() {
    FILE *file = fopen("technicianpass.txt", "r");
    if (file == NULL) {
        printf(RED "Error: Could not open technicianpass.txt file.\n" RESET);
        return;
    }

    while (fscanf(file, "%[^,],%[^,],%d\n", technicianAccounts[totalTechnicians].technicianCode, 
                   technicianAccounts[totalTechnicians].password, 
                   &technicianAccounts[totalTechnicians].isApproved) == 3) {
        totalTechnicians++;
    }

    fclose(file);
}

void saveAdminData() {
    FILE *file = fopen("admins.txt", "w");
    if (file == NULL) {
        printf(RED "Error: Could not open admins.txt file for writing.\n" RESET);
        return;
    }

    for (int i = 0; i < totalAdmins; i++) {
        fprintf(file, "%s,%s,%d\n", adminAccounts[i].username, adminAccounts[i].password, adminAccounts[i].isApproved);
    }

    fclose(file);
}

void saveAirconData() {
    FILE *file = fopen("aircon.txt", "w");
    if (file == NULL) {
        printf(RED "Error: Could not open aircon.txt file for writing.\n" RESET);
        return;
    }

    for (int i = 0; i < totalUnits; i++) {
        fprintf(file, "%s,%s,%s,%s,%s,%s,%s\n", airconUnits[i].unitNo, 
                airconUnits[i].brand, airconUnits[i].model, airconUnits[i].type, 
                airconUnits[i].building, airconUnits[i].floor, airconUnits[i].lastMaintenance);
    }

    fclose(file);
}

void saveTechnicianData() {
    FILE *file = fopen("technicianpass.txt", "w");
    if (file == NULL) {
        printf(RED "Error: Could not open technicianpass.txt file for writing.\n" RESET);
        return;
    }

    for (int i = 0; i < totalTechnicians; i++) {
        fprintf(file, "%s,%s,%d\n", technicianAccounts[i].technicianCode, technicianAccounts[i].password, technicianAccounts[i].isApproved);
    }

    fclose(file);
}

void loadServiceRequestData() {
    FILE *file = fopen("servicerequest.txt", "r");
    if (file == NULL) {
        printf(RED "Error: Could not open servicerequest.txt file.\n" RESET);
        return;
    }

    totalRequests = 0;  // Reset counter
    char line[512];
    
    while (fgets(line, sizeof(line), file)) {
        // Remove newline if present
        line[strcspn(line, "\n")] = 0;
        
        // Parse the line
        char *token;
        token = strtok(line, ",");
        if (token) strcpy(serviceRequests[totalRequests].customerCode, token);
        
        token = strtok(NULL, ",");
        if (token) strcpy(serviceRequests[totalRequests].serviceType, token);
        
        token = strtok(NULL, ",");
        if (token) strcpy(serviceRequests[totalRequests].unitNo, token);
        
        token = strtok(NULL, ",");
        if (token) strcpy(serviceRequests[totalRequests].status, token);
        
        token = strtok(NULL, ",");
        if (token) strcpy(serviceRequests[totalRequests].reason, token);
        
        token = strtok(NULL, ",");
        if (token) strcpy(serviceRequests[totalRequests].eta, token);
        
        token = strtok(NULL, ",");
        if (token && strlen(token) > 0) {
            strcpy(serviceRequests[totalRequests].lastUpdated, token);
        } else {
            // Set current time if no last updated time
            time_t now;
            time(&now);
            strftime(serviceRequests[totalRequests].lastUpdated, 
                    sizeof(serviceRequests[totalRequests].lastUpdated), 
                    "%Y-%m-%d %H:%M:%S", 
                    localtime(&now));
        }
        
        token = strtok(NULL, ",");
        if (token) strcpy(serviceRequests[totalRequests].cancelledAt, token);
        
        token = strtok(NULL, ",");
        if (token) strcpy(serviceRequests[totalRequests].technicianCode, token);

        totalRequests++;
    }

    fclose(file);
    printf(GREEN "Loaded %d service requests\n" RESET, totalRequests);
}

void saveServiceRequestData() {
    FILE *file = fopen("servicerequest.txt", "w");
    if (file == NULL) {
        printf(RED "Error: Could not open servicerequest.txt file for writing.\n" RESET);
        return;
    }

    for (int i = 0; i < totalRequests; i++) {
        fprintf(file, "%s,%s,%s,%s,%s,%s,%s,%s,%s\n", 
                serviceRequests[i].customerCode, 
                serviceRequests[i].serviceType, 
                serviceRequests[i].unitNo, 
                serviceRequests[i].status, 
                serviceRequests[i].reason, 
                serviceRequests[i].eta, 
                serviceRequests[i].lastUpdated,
                serviceRequests[i].cancelledAt,
                serviceRequests[i].technicianCode);
    }

    fclose(file);
}

void updateServiceRequestStatus(int index, const char* status) {
    strcpy(serviceRequests[index].status, status);
    time_t now;
    time(&now);
    strftime(serviceRequests[index].lastUpdated, sizeof(serviceRequests[index].lastUpdated), "%Y-%m-%d %H:%M:%S", localtime(&now));
    saveServiceRequestData();
}

void registerAdmin() {
    char username[30], password[30];

    while(1) {
        clearScreen();
        printf(CYAN "=========================\n" RESET);
        printf(" Admin Registration\n");
        printf(CYAN "=========================\n" RESET);
        printf(YELLOW "[Enter 'back' to return to previous menu]\n\n" RESET);
        
        printf(CYAN "Enter desired username: " RESET);
        scanf("%s", username);
        
        if(strcasecmp(username, "back") == 0) {
            return;
        }

        printf(CYAN "Enter password: " RESET);
        takePassword(password, 30);

        strcpy(adminAccounts[totalAdmins].username, username);
        strcpy(adminAccounts[totalAdmins].password, password);
        adminAccounts[totalAdmins].isApproved = 0;
        totalAdmins++;

        saveAdminData();

        printf(GREEN "Registration successful. You will need to be approved by an admin.\n" RESET);
        getchar();
        getchar();
        break;
    }
}

void saveCollegeData(const char* customerCode, const char* college, const char* building, const char* floor, const char* room) {
    FILE *file = fopen("college.txt", "r+");
    if (file == NULL) {
        printf(RED "Error: Could not open college.txt file.\n" RESET);
        return;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf(RED "Error: Could not open temp.txt file.\n" RESET);
        fclose(file);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char existingCustomerCode[50], existingCollege[50], existingBuilding[50], existingFloor[10], existingRoom[10];
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^\n]", existingCustomerCode, existingCollege, existingBuilding, existingFloor, existingRoom);

        if (strcmp(existingCustomerCode, customerCode) == 0) {
            fprintf(tempFile, "%s,%s,%s,%s,%s\n", customerCode, college, building, floor, room);
        } else {
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("college.txt");
    rename("temp.txt", "college.txt");
}

void generateRandomCode(char* buffer, int length) {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int unique = 0;

    while (!unique) {
        strcpy(buffer, "CUST");
        for (int i = 4; i < length + 4; i++) {
            buffer[i] = charset[rand() % (sizeof(charset) - 1)];
        }
        buffer[length + 4] = '\0';

        unique = 1;
        for (int i = 0; i < totalCustomers; i++) {
            if (strcmp(customerAccounts[i].customerCode, buffer) == 0) {
                unique = 0;
                break;
            }
        }
    }
}

// Add the missing function definition
void generateUniqueTrackingCode(char* code, int length) {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < length; i++) {
        code[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    code[length] = '\0';
}

void registerCustomer() {
    char customerCode[50], password[30];
    char firstName[50], middleName[50], surname[50], email[100], mobileNumber[20], employeeId[20];
    char college[50], building[50], floor[10], room[10];
    time_t now;
    char registrationTime[30];
    char confirmation;

    while (1) {
        clearScreen();
        printf(CYAN "=========================\n" RESET);
        printf(" Customer Registration\n");
        printf(CYAN "=========================\n" RESET);
        printf(YELLOW "[Enter 'back' to return to previous menu]\n\n" RESET);
        
        printf(CYAN "Enter first name: " RESET);
        scanf("%s", firstName);
        
        if(strcasecmp(firstName, "back") == 0) {
            return;
        }

        printf(CYAN "Enter middle name: " RESET);
        scanf("%s", middleName);
        
        if(strcasecmp(middleName, "back") == 0) {
            return;
        }

        printf(CYAN "Enter surname: " RESET);
        scanf("%s", surname);
        
        if(strcasecmp(surname, "back") == 0) {
            return;
        }

        while (1) {
            printf(CYAN "Enter email address: " RESET);
            scanf("%s", email);
            
            if(strcasecmp(email, "back") == 0) {
                return;
            }
            
            if (validateEmail(email)) {
                break;
            } else {
                printf(RED "Invalid email address. Please use an email ending with @ctu.edu.ph\n" RESET);
            }
        }

        printf(CYAN "Enter mobile number: " RESET);
        scanf("%s", mobileNumber);
        
        if(strcasecmp(mobileNumber, "back") == 0) {
            return;
        }

        printf(CYAN "Enter employee ID number: " RESET);
        scanf("%s", employeeId);
        
        if(strcasecmp(employeeId, "back") == 0) {
            return;
        }

        printf(GREEN "\nPlease confirm your details:\n" RESET);
        printf(GREEN "First Name: %s\n" RESET, firstName);
        printf(GREEN "Middle Name: %s\n" RESET, middleName);
        printf(GREEN "Surname: %s\n" RESET, surname);
        printf(GREEN "Email: %s\n" RESET, email);
        printf(GREEN "Mobile Number: %s\n" RESET, mobileNumber);
        printf(GREEN "Employee ID: %s\n" RESET, employeeId);
        printf(YELLOW "\nAre these details correct? (y/n): " RESET);
        scanf(" %c", &confirmation);

        if (confirmation == 'y' || confirmation == 'Y') {
            break;
        } else {
            printf(RED "Let's try again.\n" RESET);
        }
    }

    while (1) {
        clearScreen();
        printf(CYAN "Select college:\n" RESET);
        printf(GREEN "[1] Admin\n" RESET);
        printf(GREEN "[2] CCICT\n" RESET);
        printf(YELLOW "Enter your choice: " RESET);
        int collegeChoice;
        scanf("%d", &collegeChoice);

        switch (collegeChoice) {
            case 1:
                strcpy(college, "Admin");
                break;
            case 2:
                strcpy(college, "CCICT");
                break;
            default:
                printf(RED "Invalid choice. Please enter 1 or 2.\n" RESET);
                continue;
        }
        break;
    }

    while (1) {
        printf(CYAN "Select building:\n" RESET);
        printf(GREEN "[1] Admin\n" RESET);
        printf(GREEN "[2] CCICT\n" RESET);
        printf(YELLOW "Enter your choice: " RESET);
        int buildingChoice;
        scanf("%d", &buildingChoice);

        switch (buildingChoice) {
            case 1:
                strcpy(building, "Admin");
                break;
            case 2:
                strcpy(building, "CCICT");
                break;
            default:
                printf(RED "Invalid choice. Please enter 1 or 2.\n" RESET);
                continue;
        }
        break;
    }

    while (1) {
        if (strcmp(building, "Admin") == 0) {
            printf(CYAN "Select floor:\n" RESET);
            printf(GREEN "[1] 8th\n" RESET);
            printf(YELLOW "Enter your choice: " RESET);
            int floorChoice;
            scanf("%d", &floorChoice);

            if (floorChoice == 1) {
                strcpy(floor, "8th");
                break;
            } else {
                printf(RED "Invalid choice. Please enter 1.\n" RESET);
            }
        } else if (strcmp(building, "CCICT") == 0) {
            printf(CYAN "Select floor:\n" RESET);
            printf(GREEN "[1] 4th\n" RESET);
            printf(YELLOW "Enter your choice: " RESET);
            int floorChoice;
            scanf("%d", &floorChoice);

            if (floorChoice == 1) {
                strcpy(floor, "4th");
                break;
            } else {
                printf(RED "Invalid choice. Please enter 1.\n" RESET);
            }
        }
    }

    printf(CYAN "Enter room: " RESET);
    scanf("%s", room);

    generateRandomCode(customerCode, 6);

    time(&now);
    strftime(registrationTime, sizeof(registrationTime), "%Y-%m-%d %H:%M:%S", localtime(&now));

    strcpy(customerAccounts[totalCustomers].customerCode, customerCode);
    strcpy(customerAccounts[totalCustomers].firstName, firstName);
    strcpy(customerAccounts[totalCustomers].middleName, middleName);
    strcpy(customerAccounts[totalCustomers].surname, surname);
    strcpy(customerAccounts[totalCustomers].email, email);
    strcpy(customerAccounts[totalCustomers].mobileNumber, mobileNumber);
    strcpy(customerAccounts[totalCustomers].employeeId, employeeId);
    strcpy(customerAccounts[totalCustomers].registrationTime, registrationTime);
    strcpy(customerAccounts[totalCustomers].college, college);
    strcpy(customerAccounts[totalCustomers].building, building);
    strcpy(customerAccounts[totalCustomers].floor, floor);
    strcpy(customerAccounts[totalCustomers].room, room);
    customerAccounts[totalCustomers].isApproved = 0;
    totalCustomers++;

    saveUserData();
    saveUserCredentials();
    saveRandomGeneratedCode(customerCode);
    saveCollegeData(customerCode, college, building, floor, room);

    // Modified automatic aircon assignment section
    for (int i = 0; i < totalUnits; i++) {
        // Check if aircon location matches customer location
        if (strcmp(airconUnits[i].building, building) == 0 && 
            strcmp(airconUnits[i].floor, floor) == 0 && 
            strcmp(airconUnits[i].room, room) == 0) {
            
            // Create new assignment
            strcpy(airconAssignments[totalAssignments].customerCode, customerCode);
            strcpy(airconAssignments[totalAssignments].unitNo, airconUnits[i].unitNo);
            totalAssignments++;
            printf(GREEN "Automatically assigned aircon unit %s to your account.\n" RESET, airconUnits[i].unitNo);
        }
    }
    saveAirconAssignments();

    logAction("Customer", customerCode, "Registered");

    printf(GREEN "\nRegistration Details:\n" RESET);
    printf(GREEN "Customer Code: %s\n" RESET, customerCode);
    printf(GREEN "First Name: %s\n" RESET, firstName);
    printf(GREEN "Middle Name: %s\n" RESET, middleName);
    printf(GREEN "Surname: %s\n" RESET, surname);
    printf(GREEN "Email: %s\n" RESET, email);
    printf(GREEN "Mobile Number: %s\n" RESET, mobileNumber);
    printf(GREEN "Employee ID: %s\n" RESET, employeeId);
    printf(GREEN "College: %s\n" RESET, college);
    printf(GREEN "Building: %s\n" RESET, building);
    printf(GREEN "Floor: %s\n" RESET, floor);
    printf(GREEN "Room: %s\n" RESET, room);
    printf(GREEN "Registration Time: %s\n" RESET, registrationTime);
    printf(GREEN "Registration Successful!\n" RESET);

    getchar();
    getchar();
}

void registerTechnician() {
    char password[30], employeeId[20], surname[50];
    int technicianNumber = 1;
    char technicianCode[30];

    while(1) {
        clearScreen();
        printf(CYAN "=========================\n" RESET);
        printf(" Technician Registration\n");
        printf(CYAN "=========================\n" RESET);
        printf(YELLOW "[Enter 'back' to return to previous menu]\n\n" RESET);
        
        printf(CYAN "Enter your employee ID: " RESET);
        scanf("%s", employeeId);
        
        if(strcasecmp(employeeId, "back") == 0) {
            return;
        }

        printf(CYAN "Enter your surname: " RESET);
        scanf("%s", surname);
        
        if(strcasecmp(surname, "back") == 0) {
            return;
        }

        // Generate the next available technician code
        for (int i = 0; i < totalTechnicians; i++) {
            int currentNumber;
            if (sscanf(technicianAccounts[i].technicianCode, "tech%d", &currentNumber) == 1) {
                if (currentNumber >= technicianNumber) {
                    technicianNumber = currentNumber + 1;
                }
            }
        }
        sprintf(technicianCode, "tech%d", technicianNumber);

        printf(CYAN "Enter password: " RESET);
        takePassword(password, 30);

        strcpy(technicianAccounts[totalTechnicians].technicianCode, technicianCode);
        strcpy(technicianAccounts[totalTechnicians].password, password);
        technicianAccounts[totalTechnicians].isApproved = 0;
        totalTechnicians++;

        saveTechnicianData();

        printf(GREEN "Registration successful. Your technician code is %s. You will need to be approved by an admin.\n" RESET, technicianCode);
        getchar();
        getchar();
        break;
    }
}

void displayMainMenuWithDelay() {
    printf(CYAN "\n=========================" RESET "\n");
    printf(" Aircon Service System\n");
    printf(CYAN "=========================\n" RESET);
    printf("\n");  
    printf(GREEN "[1] Customer\n" RESET);
    printf(BLUE "[2] Admin\n" RESET);
    printf(YELLOW "[3] Technician\n" RESET);
    printf(CYAN "[4] Register an Account\n" RESET);
    printf(YELLOW "[5] Check Account Status\n" RESET);
    printf(RED "[6] Exit\n" RESET);
    printf("\n");  
    printf(YELLOW "Enter your choice: " RESET);
}

void displayFrontPage() {
    printf(CYAN "\n   ___   _______  _________  _  __  ___________ _   _______________  ______  __________________  ___  \n" RESET);
    printf(RED "  / _ | /  _/ _ \\/ ___/ __ \\/ |/ / / __/ __/ _ \\ | / /  _/ ___/ __/ / __/\\ \\/ / __/_  __/ __/  |/  /  \n" RESET);
    printf(GREEN " / __ |_/ // , _/ /__/ /_/ /    / _\\ \\/ _// , _/ |/ // // /__/ _/  _\\ \\   \\  /\\ \\  / / / _// /|_/ /   \n" RESET);
    printf(YELLOW "/_/ |_/___/_/|_|\\___/\\____/_/|_/ /___/___/_/|_||___/___/\\___/___/ /___/   /_/___/ /_/ /___/_/  /_/    \n" RESET);
    printf(BLUE "                                                                                                      \n" RESET);
}

void displayMainMenu() {
    int choice;

    while (1) {
        clearScreen();
        displayFrontPage();
        delay(2);  
        clearScreen();  
        displayTimeBar();  
        displayMainMenuWithDelay();

        scanf("%d", &choice);

        switch (choice) {
            case 1:
                customerMenu();
                break;
            case 2:
                adminMenu();
                break;
            case 3:
                technicianMenu();
                break;
            case 4:
                clearScreen();
                printf(CYAN "\n=========================" RESET "\n");
                printf(" Register an Account\n");
                printf(CYAN "=========================\n" RESET);
                printf("\n");
                printf(GREEN "[1] Customer Account\n" RESET);
                printf(BLUE "[2] Admin Account\n" RESET);
                printf(YELLOW "[3] Technician Account\n" RESET);
                printf("\n");
                printf(YELLOW "Enter your choice: " RESET);
                int subChoice;
                scanf("%d", &subChoice);
                if (subChoice == 1) {
                    registerCustomer();
                } else if (subChoice == 2) {
                    registerAdmin();
                } else if (subChoice == 3) {
                    registerTechnician();
                } else {
                    printf(RED "Invalid choice. Please try again.\n" RESET);
                    delay(2);
                }
                break;
            case 5:
                checkAccountStatus();
                break;
            case 6:
                printf(GREEN "Thank you for using the Aircon Service System. Goodbye!\n" RESET);
                exit(0);
            default:
                printf(RED "Invalid choice. Please try again.\n" RESET);
                delay(2);
        }
    }
}

void viewAndEditCredentials(const char* customerCode) {
    int choice, subChoice;
    int found = 0;
    for (int i = 0; i < totalCustomers; i++) {
        if (strcmp(customerAccounts[i].customerCode, customerCode) == 0) {
            found = 1;
            while (1) {
                clearScreen();
                printf(CYAN "\n=========================\n" RESET);
                printf(" View and Edit Credentials\n");
                printf(CYAN "=========================\n" RESET);
                printf(GREEN "[1] View Credentials\n" RESET);
                printf(YELLOW "[2] Edit Credentials\n" RESET);
                printf(BLUE "[3] Edit Location\n" RESET);
                printf(RED "[4] Back to Customer Menu\n" RESET);
                printf(YELLOW "Enter your choice: " RESET);
                scanf("%d", &choice);

                if (choice == 1) {
                    clearScreen();
                    printf(GREEN "Customer Code: %s\n" RESET, customerAccounts[i].customerCode);
                    printf(GREEN "First Name: %s\n" RESET, customerAccounts[i].firstName);
                    printf(GREEN "Middle Name: %s\n" RESET, customerAccounts[i].middleName);
                    printf(GREEN "Surname: %s\n" RESET, customerAccounts[i].surname);
                    printf(GREEN "Email: %s\n" RESET, customerAccounts[i].email);
                    printf(GREEN "Mobile Number: %s\n" RESET, customerAccounts[i].mobileNumber);
                    printf(GREEN "Employee ID: %s\n" RESET, customerAccounts[i].employeeId);
                    printf(GREEN "College: %s\n" RESET, customerAccounts[i].college);
                    printf(GREEN "Building: %s\n" RESET, customerAccounts[i].building);
                    printf(GREEN "Floor: %s\n" RESET, customerAccounts[i].floor);
                    printf(GREEN "Room: %s\n" RESET, customerAccounts[i].room);
                    printf(GREEN "Registration Time: %s\n" RESET, customerAccounts[i].registrationTime);
                    printf(CYAN "\nPress Enter to continue..." RESET);
                    getchar();
                    getchar();
                } else if (choice == 2) {
                    while (1) {
                        clearScreen();
                        printf(CYAN "\n=========================\n" RESET);
                        printf(" Edit Credentials\n");
                        printf(CYAN "=========================\n" RESET);
                        printf(GREEN "[1] First Name\n" RESET);
                        printf(YELLOW "[2] Middle Name\n" RESET);
                        printf(BLUE "[3] Surname\n" RESET);
                        printf(GREEN "[4] Email\n" RESET);
                        printf(YELLOW "[5] Mobile Number\n" RESET);
                        printf(BLUE "[6] Employee ID\n" RESET);
                        printf(RED "[7] Back\n" RESET);
                        printf(YELLOW "Enter your choice: " RESET);
                        scanf("%d", &subChoice);

                        if (subChoice == 1) {
                            printf(CYAN "Enter new first name: " RESET);
                            scanf("%s", customerAccounts[i].firstName);
                        } else if (subChoice == 2) {
                            printf(CYAN "Enter new middle name: " RESET);
                            scanf("%s", customerAccounts[i].middleName);
                        } else if (subChoice == 3) {
                            printf(CYAN "Enter new surname: " RESET);
                            scanf("%s", customerAccounts[i].surname);
                        } else if (subChoice == 4) {
                            while (1) {
                                printf(CYAN "Enter new email address: " RESET);
                                scanf("%s", customerAccounts[i].email);
                                if (validateEmail(customerAccounts[i].email)) {
                                    break;
                                } else {
                                    printf(RED "Invalid email address. Please use an email ending with @ctu.edu.ph\n" RESET);
                                }
                            }
                        } else if (subChoice == 5) {
                            printf(CYAN "Enter new mobile number: " RESET);
                            scanf("%s", customerAccounts[i].mobileNumber);
                        } else if (subChoice == 6) {
                            printf(CYAN "Enter new employee ID number: " RESET);
                            scanf("%s", customerAccounts[i].employeeId);
                        } else if (subChoice == 7) {
                            break;
                        } else {
                            printf(RED "Invalid choice. Try again.\n" RESET);
                            getchar();
                            getchar();
                        }
                        saveUserData();
                        saveUserCredentials();
                        printf(GREEN "Credentials updated successfully!\n" RESET);
                        printf(CYAN "\nPress Enter to continue..." RESET);
                        getchar();
                        getchar();
                    }
                } else if (choice == 3) {
                    while (1) {
                        clearScreen();
                        printf(CYAN "\n=========================\n" RESET);
                        printf(" Edit Location\n");
                        printf(CYAN "=========================\n" RESET);
                        printf(GREEN "[1] College\n" RESET);
                        printf(YELLOW "[2] Building\n" RESET);
                        printf(BLUE "[3] Floor\n" RESET);
                        printf(GREEN "[4] Room\n" RESET);
                        printf(RED "[5] Back\n" RESET);
                        printf(YELLOW "Enter your choice: " RESET);
                        scanf("%d", &subChoice);

                        if (subChoice == 1) {
                            printf(CYAN "Enter new college: " RESET);
                            scanf("%s", customerAccounts[i].college);
                        } else if (subChoice == 2) {
                            printf(CYAN "Enter new building: " RESET);
                            scanf("%s", customerAccounts[i].building);
                        } else if (subChoice == 3) {
                            printf(CYAN "Enter new floor: " RESET);
                            scanf("%s", customerAccounts[i].floor);
                        } else if (subChoice == 4) {
                            printf(CYAN "Enter new room: " RESET);
                            scanf("%s", customerAccounts[i].room);
                        } else if (subChoice == 5) {
                            break;
                        } else {
                            printf(RED "Invalid choice. Try again.\n" RESET);
                            getchar();
                            getchar();
                        }
                        saveUserData();
                        saveUserCredentials();
                        saveCollegeData(customerAccounts[i].customerCode, customerAccounts[i].college, customerAccounts[i].building, customerAccounts[i].floor, customerAccounts[i].room);
                        printf(GREEN "Location updated successfully!\n" RESET);
                        printf(CYAN "\nPress Enter to continue..." RESET);
                        getchar();
                        getchar();
                    }
                } else if (choice == 4) {
                    return;
                } else {
                    printf(RED "Invalid choice. Try again.\n" RESET);
                    getchar();
                    getchar();
                }
            }
        }
    }
    if (!found) {
        printf(RED "Customer not found.\n" RESET);
        getchar();
        getchar();
    }
}

void customerMenu() {
    int choice;
    char customerCode[50];

    clearScreen();
    printf(CYAN "Enter your customer code: " RESET);
    scanf("%s", customerCode);

    int authorized = 0;
    for (int i = 0; i < totalCustomers; i++) {
        if (strcmp(customerAccounts[i].customerCode, customerCode) == 0 && customerAccounts[i].isApproved == 1) {
            authorized = 1;
            break;
        }
    }

    if (!authorized) {
        printf(RED "Unauthorized customer code.\n" RESET);
        getchar();
        getchar();
        return;
    }

    logAction("Customer", customerCode, "Logged In");

    while (1) {
        clearScreen();
        displayTimeBar();  
        printf(CYAN "\n==========================" RESET);
        printf(CYAN "\n Available Services" RESET);
        printf(CYAN "\n==========================\n" RESET);
        printf(YELLOW "[1] Cleaning Service\n" RESET);
        printf("   - Basic cleaning and sanitizing\n");
        printf("   - Filter cleaning and maintenance\n");
        printf("   - Coil cleaning\n");
        printf(YELLOW "\n[2] Maintenance Service\n" RESET);
        printf("   - System inspection and diagnosis\n");
        printf("   - Parts replacement if needed\n");
        printf("   - Performance optimization\n");
        printf(YELLOW "\n[3] Repair Service\n" RESET);
        printf("   - Troubleshooting and diagnostics\n");
        printf("   - Component repair/replacement\n");
        printf("   - Emergency repair service\n");
        printf("   - Post-repair testing\n");
        printf(CYAN "\n==========================\n\n" RESET);

        printf(CYAN "=========================\n" RESET);
        printf(" Customer Menu\n");
        printf(CYAN "=========================\n" RESET);
        printf("\n");
        printf(GREEN "[1] View Aircon Units\n" RESET);
        printf(YELLOW "[2] Book Service\n" RESET);
        printf(BLUE "[3] Check Request Status\n" RESET);
        printf(RED "[4] Cancel Service Booking\n" RESET);  
        printf(BLUE "[5] Submit a Help Ticket to Admin\n" RESET);  
        printf(GREEN "[6] View and Edit Credentials\n" RESET);  
        printf(YELLOW "[7] View Notifications\n" RESET);  
        printf(RED "[8] Back to Main Menu\n" RESET);
        printf("\n");
        printf(YELLOW "Enter your choice: " RESET);
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                clearScreen();
                displayTimeBar();
                printf(CYAN "\n                               YOUR AIRCON UNITS                               \n" RESET);
                
                printf(CYAN "+--------+------------+------------+------------+------------+--------+----------------+\n" RESET);
                printf(CYAN "| Unit   | Brand      | Model      | Type       | Building   | Floor  | Maintenance    |\n" RESET);
                printf(CYAN "+--------+------------+------------+------------+------------+--------+----------------+\n" RESET);

                for (int i = 0; i < totalAssignments; i++) {
                    if (strcmp(airconAssignments[i].customerCode, customerCode) == 0) {
                        for (int j = 0; j < totalUnits; j++) {
                            if (strcmp(airconUnits[j].unitNo, airconAssignments[i].unitNo) == 0) {
                                if (i % 2 == 0) {
                                    printf(GREEN "| %-6s | %-10s | %-10s | %-10s | %-10s | %-6s | %-14s |\n" RESET,
                                           airconUnits[j].unitNo,
                                           airconUnits[j].brand,
                                           airconUnits[j].model,
                                           airconUnits[j].type,
                                           airconUnits[j].building,
                                           airconUnits[j].floor,
                                           airconUnits[j].lastMaintenance);
                                } else {
                                    printf(YELLOW "| %-6s | %-10s | %-10s | %-10s | %-10s | %-6s | %-14s |\n" RESET,
                                           airconUnits[j].unitNo,
                                           airconUnits[j].brand,
                                           airconUnits[j].model,
                                           airconUnits[j].type,
                                           airconUnits[j].building,
                                           airconUnits[j].floor,
                                           airconUnits[j].lastMaintenance);
                                }
                                break;
                            }
                        }
                    }
                }

                printf(CYAN "+--------+------------+------------+------------+------------+--------+----------------+\n" RESET);
                printf(CYAN "\nPress Enter to continue..." RESET);
                getchar();
                getchar();
                break;
            case 2:
                bookService(customerCode);
                break;
            case 3:
                checkRequestStatus(customerCode);
                getchar();
                getchar();
                break;
            case 4:
                cancelServiceBooking(customerCode);
                break;
            case 5:
                submitHelpTicket(customerCode);
                break;
            case 6:
                viewAndEditCredentials(customerCode);
                break;
            case 7:
                viewNotifications(customerCode);
                break;
            case 8:
                return;
            default:
                printf(RED "Invalid choice. Try again.\n" RESET);
                getchar();
                getchar();
        }
    }
}

void adminMenu() {
    char username[30], password[30];
    int choice;

    clearScreen();
    printf(CYAN "\nAdmin Login\n" RESET);
    printf(BLUE "Username: " RESET);
    scanf("%s", username);
    printf(BLUE "Enter password: " RESET);  
    takePassword(password, 30);

    int authorized = 0;
    for (int i = 0; i < totalAdmins; i++) {
        if (strcmp(adminAccounts[i].username, username) == 0 && strcmp(adminAccounts[i].password, password) == 0) {
            if (adminAccounts[i].isApproved == 0) {
                printf(RED "Your account is not approved.\n" RESET);
                return;
            }
            authorized = 1;
            break;
        }
    }

    if (!authorized) {
        printf(RED "Admin login failed. Incorrect username or password.\n" RESET);
        getchar();
        getchar();
        return;
    }

    logAction("Admin", username, "Logged In");

    while (1) {
        clearScreen();
        displayTimeBar();  
        printf(CYAN "\n=========================" RESET "\n");
        printf(" Admin Menu\n");
        printf(CYAN "=========================\n" RESET);
        printf("\n");
        printf(GREEN "[1] View Service Requests\n" RESET);
        printf(BLUE "[2] Approve/Decline Requests\n" RESET);
        printf(YELLOW "[3] View Log History\n" RESET);
        printf(GREEN "[4] Add Aircon Unit\n" RESET);
        printf(BLUE "[5] Edit Aircon Unit\n" RESET);
        printf(YELLOW "[6] Approve/Decline Accounts\n" RESET);
        printf(CYAN "[7] View All Aircons\n" RESET);
        printf(GREEN "[8] Assign Aircons to Customer\n" RESET);  
        printf(YELLOW "[9] View Help Tickets\n" RESET);  
        printf(BLUE "[10] De-assign Aircon Units\n" RESET);  
        printf(RED "[11] Back to Main Menu\n" RESET);
        printf("\n");
        printf(YELLOW "Enter your choice: " RESET);
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewServiceRequests();
                break;
            case 2:
                approveOrDeclineRequest();
                break;
            case 3:
                viewLog();
                break;
            case 4:
                addAirconUnit();
                break;
            case 5:
                editAirconUnit();
                break;
            case 6:
                approveOrDeclineAccounts();
                break;
            case 7:
                viewAllAircons();
                break;
            case 8:
                assignAirconsToCustomer();
                break;
            case 9:
                viewHelpTickets();
                break;
            case 10:
                deassignAirconUnits();
                break;
            case 11:
                return;
            default:
                printf(RED "Invalid choice. Try again.\n" RESET);
        }
    }
}

void viewServiceRequests() {
    clearScreen();
    displayTimeBar();
    
    printf(CYAN "\n                               SERVICE REQUESTS                               \n" RESET);
    
    printf(CYAN "+-------------+--------+------------+-----------+------------------+------------------+------------------+------------------+\n" RESET);
    printf(CYAN "| Customer    | Unit   | Service    | Status    | Last Updated    | Notes           | Technician       | ETA              |\n" RESET);
    printf(CYAN "+-------------+--------+------------+-----------+------------------+------------------+------------------+------------------+\n" RESET);

    for (int i = 0; i < totalRequests; i++) {
        if (i % 2 == 0) {
            printf(GREEN "| %-11s | %-6s | %-10s | %-9s | %-16s | %-14s | %-16s | %-16s |\n" RESET,
                   serviceRequests[i].customerCode,
                   serviceRequests[i].unitNo,
                   serviceRequests[i].serviceType,
                   serviceRequests[i].status,
                   serviceRequests[i].lastUpdated,
                   strlen(serviceRequests[i].reason) > 0 ? serviceRequests[i].reason : 
                   (strcmp(serviceRequests[i].status, "Approved") == 0 ? serviceRequests[i].eta : "N/A"),
                   serviceRequests[i].technicianCode,
                   serviceRequests[i].eta);
        } else {
            printf(YELLOW "| %-11s | %-6s | %-10s | %-9s | %-16s | %-14s | %-16s | %-16s |\n" RESET,
                   serviceRequests[i].customerCode,
                   serviceRequests[i].unitNo,
                   serviceRequests[i].serviceType,
                   serviceRequests[i].status,
                   serviceRequests[i].lastUpdated,
                   strlen(serviceRequests[i].reason) > 0 ? serviceRequests[i].reason : 
                   (strcmp(serviceRequests[i].status, "Approved") == 0 ? serviceRequests[i].eta : "N/A"),
                   serviceRequests[i].technicianCode,
                   serviceRequests[i].eta);
        }
        if (i < totalRequests - 1) {
            printf(CYAN "+-------------+--------+------------+-----------+------------------+------------------+------------------+------------------+\n" RESET);
        }
    }

    printf(CYAN "+-------------+--------+------------+-----------+------------------+------------------+------------------+------------------+\n" RESET);
    printf(YELLOW "\nTotal Requests: %d\n" RESET, totalRequests);
    printf(CYAN "\nPress Enter to continue..." RESET);
    getchar();
    getchar();
}

void approveOrDeclineRequest() {
    char customerCode[50], unitNo[20], technicianCode[30];
    int found = 0;

    while (1) {
        clearScreen();
        // Display all pending service requests first
        printf(CYAN "\n                               PENDING SERVICE REQUESTS                               \n" RESET);
        printf(CYAN "+-------------+--------+------------+------------------+------------------+----------------+\n" RESET);
        printf(CYAN "| Customer    | Unit   | Service    | Requested Date   | Requested Time   | Building       |\n" RESET);
        printf(CYAN "+-------------+--------+------------+------------------+------------------+----------------+\n" RESET);

        for (int i = 0; i < totalRequests; i++) {
            if (strcmp(serviceRequests[i].status, "Pending") == 0) {
                char dateCopy[30], timeCopy[30];
                strcpy(dateCopy, serviceRequests[i].eta);
                char *date = strtok(dateCopy, " ");
                char *time = strtok(NULL, " ");
                
                // Find building for this unit
                char building[50] = "N/A";
                for (int j = 0; j < totalUnits; j++) {
                    if (strcmp(airconUnits[j].unitNo, serviceRequests[i].unitNo) == 0) {
                        strcpy(building, airconUnits[j].building);
                        break;
                    }
                }
                
                printf(YELLOW "| %-11s | %-6s | %-10s | %-16s | %-16s | %-14s |\n" RESET,
                       serviceRequests[i].customerCode,
                       serviceRequests[i].unitNo,
                       serviceRequests[i].serviceType,
                       date ? date : "N/A",
                       time ? time : "N/A",
                       building);
            }
        }
        printf(CYAN "+-------------+--------+------------+------------------+------------------+----------------+\n" RESET);

        printf(CYAN "\nEnter Customer Code to approve/decline service request (or type 'back' to return): " RESET);
        scanf("%s", customerCode);

        if (strcasecmp(customerCode, "back") == 0) {
            return;
        }

        printf(CYAN "Enter Unit No: " RESET);
        scanf("%s", unitNo);

        found = 0;
        for (int i = 0; i < totalRequests; i++) {
            if (strcmp(serviceRequests[i].customerCode, customerCode) == 0 && 
                strcmp(serviceRequests[i].unitNo, unitNo) == 0 && 
                strcmp(serviceRequests[i].status, "Pending") == 0) {
                found = 1;
                printf(CYAN "Approve/Decline this request (a/d): " RESET);
                char choice = getch();
                if (choice == 'a' || choice == 'A') {
                    printf(CYAN "Enter Technician Code to assign: " RESET);
                    scanf("%s", technicianCode);
                    strcpy(serviceRequests[i].status, "Approved");
                    strcpy(serviceRequests[i].technicianCode, technicianCode);
                    time_t now;
                    time(&now);
                    strftime(serviceRequests[i].lastUpdated, sizeof(serviceRequests[i].lastUpdated), "%Y-%m-%d %H:%M:%S", localtime(&now));
                    saveServiceRequestData();
                    printf(GREEN "Service request approved and technician assigned.\n" RESET);
                    logAction("Admin", customerCode, "Approved Service and Assigned Technician");
                } else if (choice == 'd' || choice == 'D') {
                    strcpy(serviceRequests[i].status, "Declined");
                    time_t now;
                    time(&now);
                    strftime(serviceRequests[i].lastUpdated, sizeof(serviceRequests[i].lastUpdated), "%Y-%m-%d %H:%M:%S", localtime(&now));
                    saveServiceRequestData();
                    printf(RED "Service request declined.\n" RESET);
                    logAction("Admin", customerCode, "Declined Service Request");
                } else {
                    printf(RED "Invalid input. Please try again.\n" RESET);
                }
                break;
            }
        }

        if (!found) {
            printf(RED "Service request not found. Please try again.\n" RESET);
        }

        printf(CYAN "\nPress Enter to continue..." RESET);
        getchar();
        getchar();
    }
}

void checkRequestStatus(const char* customerCode) {
    int found = 0;
    clearScreen();
    displayTimeBar();
    
    printf(CYAN "\n                               SERVICE REQUEST STATUS                               \n" RESET);
    
    printf(CYAN "+--------+------------+------------+-----------+------------------+------------------+------------------+------------------+\n" RESET);
    printf(CYAN "| Unit   | Service    | Status     | ETA       | Last Updated     | Technician       | Date             | Location         |\n" RESET);
    printf(CYAN "+--------+------------+------------+-----------+------------------+------------------+------------------+------------------+\n" RESET);

    for (int i = 0; i < totalRequests; i++) {
        if (strcmp(serviceRequests[i].customerCode, customerCode) == 0) {
            for (int j = 0; j < totalUnits; j++) {
                if (strcmp(airconUnits[j].unitNo, serviceRequests[i].unitNo) == 0) {
                    if (i % 2 == 0) {
                        printf(GREEN "| %-6s | %-10s | %-10s | %-9s | %-16s | %-16s | %-16s | %-16s |\n" RESET,
                               serviceRequests[i].unitNo,
                               serviceRequests[i].serviceType,
                               serviceRequests[i].status,
                               serviceRequests[i].eta,
                               serviceRequests[i].lastUpdated,
                               serviceRequests[i].technicianCode,
                               serviceRequests[i].eta,
                               airconUnits[j].building);
                    } else {
                        printf(YELLOW "| %-6s | %-10s | %-10s | %-9s | %-16s | %-16s | %-16s | %-16s |\n" RESET,
                               serviceRequests[i].unitNo,
                               serviceRequests[i].serviceType,
                               serviceRequests[i].status,
                               serviceRequests[i].eta,
                               serviceRequests[i].lastUpdated,
                               serviceRequests[i].technicianCode,
                               serviceRequests[i].eta,
                               airconUnits[j].building);
                    }
                    break;
                }
            }
            found = 1;
        }
    }

    printf(CYAN "+--------+------------+------------+-----------+------------------+------------------+------------------+------------------+\n" RESET);
    
    if (!found) {
        printf(RED "No service requests found for your customer code.\n" RESET);
    }

    printf(CYAN "\nPress Enter to continue..." RESET);
    getchar();
    getchar();
}

void logAction(const char* userType, const char* userCode, const char* action) {
    FILE *file = fopen("history.txt", "a");
    if (file == NULL) {
        printf(RED "Error: Could not open history.txt file for writing.\n" RESET);
        return;
    }

    time_t now;
    time(&now);
    fprintf(file, "%s %s,%s,%s", userType, userCode, action, ctime(&now));
    fclose(file);
}

void viewLog() {
    clearScreen();
    displayTimeBar();  
    FILE *file = fopen("history.txt", "r");
    if (file == NULL) {
        printf(RED "Error: Could not open history.txt file.\n" RESET);
        return;
    }

    char line[256];
    printf(CYAN "Log History:\n" RESET);
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
    getchar();
    getchar();
}

void bookService(const char* customerCode) {
    char unitNo[20], serviceType[20], serviceTime[20], serviceDate[20];
    int validTime = 0;
    struct tm preferredDateTime = {0};
    time_t now, preferredTime;

    clearScreen();
    displayTimeBar();
    printf(CYAN "\n                               YOUR AIRCON UNITS                               \n" RESET);
    printf(CYAN "+--------+------------+------------+------------+------------+--------+----------------+\n" RESET);
    printf(CYAN "| Unit   | Brand      | Model      | Type       | Building   | Floor  | Maintenance    |\n" RESET);
    printf(CYAN "+--------+------------+------------+------------+------------+--------+----------------+\n" RESET);

    for (int i = 0; i < totalAssignments; i++) {
        if (strcmp(airconAssignments[i].customerCode, customerCode) == 0) {
            for (int j = 0; j < totalUnits; j++) {
                if (strcmp(airconUnits[j].unitNo, airconAssignments[i].unitNo) == 0) {
                    printf(GREEN "| %-6s | %-10s | %-10s | %-10s | %-10s | %-6s | %-14s |\n" RESET,
                           airconUnits[j].unitNo,
                           airconUnits[j].brand,
                           airconUnits[j].model,
                           airconUnits[j].type,
                           airconUnits[j].building,
                           airconUnits[j].floor,
                           airconUnits[j].lastMaintenance);
                    break;
                }
            }
        }
    }

    printf(CYAN "+--------+------------+------------+------------+------------+--------+----------------+\n" RESET);

    printf(CYAN "\nEnter the Unit No of the aircon: " RESET);
    scanf("%s", unitNo);

    int unitAssigned = 0;
    for (int i = 0; i < totalAssignments; i++) {
        if (strcmp(airconAssignments[i].customerCode, customerCode) == 0 && 
            strcmp(airconAssignments[i].unitNo, unitNo) == 0) {
            unitAssigned = 1;
            break;
        }
    }

    if (!unitAssigned) {
        printf(RED "You can only book services for aircon units assigned to you.\n" RESET);
        getchar();
        getchar();
        return;
    }

    while (1) {
        printf(CYAN "Enter preferred service date (YYYY-MM-DD): " RESET);
        scanf("%s", serviceDate);
        printf(CYAN "Enter preferred service time (HH:MM): " RESET);
        scanf("%s", serviceTime);

        sscanf(serviceDate, "%d-%d-%d", &preferredDateTime.tm_year, &preferredDateTime.tm_mon, &preferredDateTime.tm_mday);
        sscanf(serviceTime, "%d:%d", &preferredDateTime.tm_hour, &preferredDateTime.tm_min);
        preferredDateTime.tm_year -= 1900;
        preferredDateTime.tm_mon -= 1;
        preferredDateTime.tm_sec = 0;

        time(&now);
        preferredTime = mktime(&preferredDateTime);

        if (difftime(preferredTime, now) > 0) {
            validTime = 1;
            break;
        } else {
            printf(RED "Preferred time and date must be ahead of the current time and date. Please try again.\n" RESET);
        }
    }

    while (1) {
        printf(CYAN "Select Service Type (1. Clean, 2. Maintenance, 3. Repair): " RESET);
        int serviceChoice;
        scanf("%d", &serviceChoice);

        switch (serviceChoice) {
            case 1:
                strcpy(serviceType, "Clean");
                break;
            case 2:
                strcpy(serviceType, "Maintenance");
                break;
            case 3:
                strcpy(serviceType, "Repair");
                break;
            default:
                printf(RED "Invalid choice. Please enter 1, 2, or 3.\n" RESET);
                continue;
        }

        for (int i = 0; i < totalRequests; i++) {
            if (strcmp(serviceRequests[i].customerCode, customerCode) == 0 && 
                strcmp(serviceRequests[i].unitNo, unitNo) == 0 &&
                strcasecmp(serviceRequests[i].serviceType, serviceType) == 0 && 
                strcasecmp(serviceRequests[i].status, "Pending") == 0) {
                printf(RED "You already have a pending %s request for this unit. Please wait until it is completed.\n" RESET, serviceType);
                getchar();
                getchar();
                return;
            }
        }

        strcpy(serviceRequests[totalRequests].customerCode, customerCode);
        strcpy(serviceRequests[totalRequests].unitNo, unitNo);
        strcpy(serviceRequests[totalRequests].serviceType, serviceType);
        strcpy(serviceRequests[totalRequests].status, "Pending");
        sprintf(serviceRequests[totalRequests].eta, "%s %s", serviceDate, serviceTime);
        
        // Initialize other fields
        serviceRequests[totalRequests].reason[0] = '\0';
        serviceRequests[totalRequests].cancelledAt[0] = '\0';
        serviceRequests[totalRequests].technicianCode[0] = '\0';
        
        // Set last updated time
        time(&now);
        strftime(serviceRequests[totalRequests].lastUpdated, 
                sizeof(serviceRequests[totalRequests].lastUpdated), 
                "%Y-%m-%d %H:%M:%S", 
                localtime(&now));
        
        totalRequests++;
        saveServiceRequestData();

        printf(GREEN "Service request for %s submitted successfully!\n" RESET, serviceType);

        printf(CYAN "Do you want to book another service for this unit? (y/n): " RESET);
        char anotherService;
        scanf(" %c", &anotherService);
        if (anotherService == 'n' || anotherService == 'N') {
            break;
        }
    }

    logAction("Customer", customerCode, "Booked Service");
    getchar();
    getchar();
}

void addAirconUnit() {
    AirconUnit newUnit;
    int lastNumber = 0;

    // Find the last generated aircon code number
    for (int i = 0; i < totalUnits; i++) {
        int currentNumber;
        if (sscanf(airconUnits[i].unitNo, "A%d", &currentNumber) == 1) {
            if (currentNumber > lastNumber) {
                lastNumber = currentNumber;
            }
        }
    }

    // Generate the new aircon code
    sprintf(newUnit.unitNo, "A%03d", lastNumber + 1);

    clearScreen();
    printf(CYAN "Generated Unit No: %s\n" RESET, newUnit.unitNo);
    printf(CYAN "Enter Brand: " RESET);
    scanf("%s", newUnit.brand);
    printf(CYAN "Enter Model: " RESET);
    scanf("%s", newUnit.model);
    printf(CYAN "Enter Type: " RESET);
    scanf("%s", newUnit.type);

    while (1) {
        printf(CYAN "Select building:\n" RESET);
        printf(GREEN "[1] Admin\n" RESET);
        printf(GREEN "[2] CCICT\n" RESET);
        printf(YELLOW "Enter your choice: " RESET);
        int buildingChoice;
        scanf("%d", &buildingChoice);

        switch (buildingChoice) {
            case 1:
                strcpy(newUnit.building, "Admin");
                break;
            case 2:
                strcpy(newUnit.building, "CCICT");
                break;
            default:
                printf(RED "Invalid choice. Please enter 1 or 2.\n" RESET);
                continue;
        }
        break;
    }

    while (1) {
        if (strcmp(newUnit.building, "Admin") == 0) {
            printf(CYAN "Select floor:\n" RESET);
            printf(GREEN "[1] 8th\n" RESET);
            printf(YELLOW "Enter your choice: " RESET);
            int floorChoice;
            scanf("%d", &floorChoice);

            if (floorChoice == 1) {
                strcpy(newUnit.floor, "8th");
                break;
            } else {
                printf(RED "Invalid choice. Please enter 1.\n" RESET);
            }
        } else if (strcmp(newUnit.building, "CCICT") == 0) {
            printf(CYAN "Select floor:\n" RESET);
            printf(GREEN "[1] 4th\n" RESET);
            printf(YELLOW "Enter your choice: " RESET);
            int floorChoice;
            scanf("%d", &floorChoice);

            if (floorChoice == 1) {
                strcpy(newUnit.floor, "4th");
                break;
            } else {
                printf(RED "Invalid choice. Please enter 1.\n" RESET);
            }
        }
    }

    printf(CYAN "Enter room: " RESET);
    scanf("%s", newUnit.room);

    printf(CYAN "Enter Last Maintenance Date (YYYY-MM-DD): " RESET);
    scanf("%s", newUnit.lastMaintenance);

    airconUnits[totalUnits++] = newUnit;
    saveAirconData();

    // Modified automatic assignment section - assign to all matching customers
    for (int i = 0; i < totalCustomers; i++) {
        if (strcmp(customerAccounts[i].building, newUnit.building) == 0 &&
            strcmp(customerAccounts[i].floor, newUnit.floor) == 0 &&
            strcmp(customerAccounts[i].room, newUnit.room) == 0) {
            strcpy(airconAssignments[totalAssignments].customerCode, customerAccounts[i].customerCode);
            strcpy(airconAssignments[totalAssignments].unitNo, newUnit.unitNo);
            totalAssignments++;
            printf(GREEN "Automatically assigned to customer %s based on location match.\n" RESET, 
                   customerAccounts[i].customerCode);
        }
    }
    saveAirconAssignments();

    printf(GREEN "Aircon unit added and assigned successfully!\n" RESET);
    logAction("Admin", "N/A", "Added Aircon Unit");
    getchar();
    getchar();
}

void editAirconUnit() {
    char unitNo[20];
    int found = 0;
    int choice;

    clearScreen();
    printf(CYAN "Enter Unit No of the aircon to edit: " RESET);
    scanf("%s", unitNo);

    for (int i = 0; i < totalUnits; i++) {
        if (strcmp(airconUnits[i].unitNo, unitNo) == 0) {
            found = 1;
            while (1) {
                clearScreen();
                printf(CYAN "Editing Aircon Unit: %s\n" RESET, unitNo);
                printf(GREEN "[1] Brand\n" RESET);
                printf(YELLOW "[2] Model\n" RESET);
                printf(BLUE "[3] Type\n" RESET);
                printf(GREEN "[4] Building\n" RESET);
                printf(YELLOW "[5] Floor\n" RESET);
                printf(BLUE "[6] Last Maintenance Date\n" RESET);
                printf(RED "[7] Back\n" RESET);
                printf(YELLOW "Enter your choice: " RESET);
                scanf("%d", &choice);

                if (choice == 1) {
                    printf(CYAN "Enter new Brand: " RESET);
                    scanf("%s", airconUnits[i].brand);
                } else if (choice == 2) {
                    printf(CYAN "Enter new Model: " RESET);
                    scanf("%s", airconUnits[i].model);
                } else if (choice == 3) {
                    printf(CYAN "Enter new Type: " RESET);
                    scanf("%s", airconUnits[i].type);
                } else if (choice == 4) {
                    printf(CYAN "Enter new Building: " RESET);
                    scanf("%s", airconUnits[i].building);
                } else if (choice == 5) {
                    printf(CYAN "Enter new Floor: " RESET);
                    scanf("%s", airconUnits[i].floor);
                } else if (choice == 6) {
                    printf(CYAN "Enter new Last Maintenance Date (YYYY-MM-DD): " RESET);
                    scanf("%s", airconUnits[i].lastMaintenance);
                } else if (choice == 7) {
                    break;
                } else {
                    printf(RED "Invalid choice. Try again.\n" RESET);
                    getchar();
                    getchar();
                }
                saveAirconData();
                printf(GREEN "Aircon unit updated successfully!\n" RESET);
                printf(CYAN "\nPress Enter to continue..." RESET);
                getchar();
                getchar();
            }
            break;
        }
    }

    if (!found) {
        printf(RED "No such aircon unit found.\n" RESET);
        getchar();
        getchar();
    }
}

void approveOrDeclineAccounts() {
    int choice;
    int pendingAccounts = 0;
    clearScreen();
    printf(CYAN "\n=========================" RESET "\n");
    printf(" Approve/Decline Accounts\n");
    printf(CYAN "=========================\n" RESET);
    printf("\n");
    printf(GREEN "[1] Customer Accounts\n" RESET);
    printf(BLUE "[2] Admin Accounts\n" RESET);
    printf(YELLOW "[3] Technician Accounts\n" RESET);
    printf("\n");
    printf(YELLOW "Enter your choice: " RESET);
    scanf("%d", &choice);

    if (choice == 1) {
        printf(CYAN "\nPending Customer Accounts:\n" RESET);
        printf(CYAN "+----------------+----------------+----------------+----------------+----------------+\n" RESET);
        printf(CYAN "| Tracking Code  | Surname        | First Name     | Employee Code  | Registration   |\n" RESET);
        printf(CYAN "+----------------+----------------+----------------+----------------+----------------+\n" RESET);
        for (int i = 0; i < totalCustomers; i++) {
            if (customerAccounts[i].isApproved == 0) {
                pendingAccounts = 1;
                printf(GREEN "| %-14s | %-14s | %-14s | %-14s | %-14s |\n" RESET,
                       customerAccounts[i].customerCode,
                       customerAccounts[i].surname,
                       customerAccounts[i].firstName,
                       customerAccounts[i].employeeId,
                       customerAccounts[i].registrationTime);
            }
        }
        printf(CYAN "+----------------+----------------+----------------+----------------+----------------+\n" RESET);
        if (!pendingAccounts) {
            printf(RED "No pending customer accounts for approval.\n" RESET);
            getchar();
            getchar();
            return;
        }
        printf(YELLOW "Enter Customer Code to approve/decline: " RESET);
        char customerCode[50];
        scanf("%s", customerCode);
        for (int i = 0; i < totalCustomers; i++) {
            if (strcmp(customerAccounts[i].customerCode, customerCode) == 0 && customerAccounts[i].isApproved == 0) {
                printf(CYAN "Approve/Decline this account (a/d): " RESET);
                char choice = getch();
                if (choice == 'a' || choice == 'A') {
                    customerAccounts[i].isApproved = 1;
                    printf(GREEN "Account Approved\n" RESET);
                    logAction("Admin", "N/A", "Approved Customer Account");
                } else if (choice == 'd' || choice == 'D') {
                    printf(RED "Account Declined\n" RESET);
                    logAction("Admin", "N/A", "Declined Customer Account");
                } else {
                    printf(RED "Invalid input\n" RESET);
                    getchar();
                    getchar();
                    return;
                }
                saveUserData();  
                break;
            }
        }
    } else if (choice == 2) {
        printf(CYAN "\nPending Admin Accounts:\n" RESET);
        printf(CYAN "+----------------+----------------+----------------+----------------+----------------+\n" RESET);
        printf(CYAN "| Tracking Code  | Surname        | First Name     | Employee Code  | Registration   |\n" RESET);
        printf(CYAN "+----------------+----------------+----------------+----------------+----------------+\n" RESET);
        for (int i = 0; i < totalAdmins; i++) {
            if (adminAccounts[i].isApproved == 0) {
                pendingAccounts = 1;
                printf(GREEN "| %-14s | %-14s | %-14s | %-14s | %-14s |\n" RESET,
                       adminAccounts[i].username,
                       "N/A",  
                       "N/A",  
                       "N/A",  
                       "N/A"); 
            }
        }
        printf(CYAN "+----------------+----------------+----------------+----------------+----------------+\n" RESET);
        if (!pendingAccounts) {
            printf(RED "No pending admin accounts for approval.\n" RESET);
            getchar();
            getchar();
            return;
        }
        printf(YELLOW "Enter Admin Username to approve/decline: " RESET);
        char adminUsername[50];
        scanf("%s", adminUsername);
        for (int i = 0; i < totalAdmins; i++) {
            if (strcmp(adminAccounts[i].username, adminUsername) == 0 && adminAccounts[i].isApproved == 0) {
                printf(CYAN "Approve/Decline this account (a/d): " RESET);
                char choice = getch();
                if (choice == 'a' || choice == 'A') {
                    adminAccounts[i].isApproved = 1;
                    printf(GREEN "Account Approved\n" RESET);
                    logAction("Admin", "N/A", "Approved Admin Account");
                } else if (choice == 'd' || choice == 'D') {
                    printf(RED "Account Declined\n" RESET);
                    logAction("Admin", "N/A", "Declined Admin Account");
                } else {
                    printf(RED "Invalid input\n" RESET);
                    getchar();
                    getchar();
                    return;
                }
                saveAdminData();  
                break;
            }
        }
    } else if (choice == 3) {
        printf(CYAN "\nPending Technician Accounts:\n" RESET);
        printf(CYAN "+----------------+----------------+----------------+----------------+----------------+\n" RESET);
        printf(CYAN "| Tracking Code  | Surname        | First Name     | Employee Code  | Registration   |\n" RESET);
        printf(CYAN "+----------------+----------------+----------------+----------------+----------------+\n" RESET);
        for (int i = 0; i < totalTechnicians; i++) {
            if (technicianAccounts[i].isApproved == 0) {
                pendingAccounts = 1;
                printf(GREEN "| %-14s | %-14s | %-14s | %-14s | %-14s |\n" RESET,
                       technicianAccounts[i].technicianCode,
                       "N/A",  
                       "N/A",  
                       "N/A",  
                       "N/A"); 
            }
        }
        printf(CYAN "+----------------+----------------+----------------+----------------+----------------+\n" RESET);
        if (!pendingAccounts) {
            printf(RED "No pending technician accounts for approval.\n" RESET);
            getchar();
            getchar();
            return;
        }
        printf(YELLOW "Enter Technician Code to approve/decline: " RESET);
        char technicianCode[50];
        scanf("%s", technicianCode);
        for (int i = 0; i < totalTechnicians; i++) {
            if (strcmp(technicianAccounts[i].technicianCode, technicianCode) == 0 && technicianAccounts[i].isApproved == 0) {
                printf(CYAN "Approve/Decline this account (a/d): " RESET);
                char choice = getch();
                if (choice == 'a' || choice == 'A') {
                    technicianAccounts[i].isApproved = 1;
                    printf(GREEN "Account Approved\n" RESET);
                    logAction("Admin", "N/A", "Approved Technician Account");
                } else if (choice == 'd' || choice == 'D') {
                    printf(RED "Account Declined\n" RESET);
                    logAction("Admin", "N/A", "Declined Technician Account");
                } else {
                    printf(RED "Invalid input\n" RESET);
                    getchar();
                    getchar();
                    return;
                }
                saveTechnicianData();  
                break;
            }
        }
    } else {
        printf(RED "Invalid choice. Please try again.\n" RESET);
        getchar();
        getchar();
        return;
    }

    getchar();
    getchar();
}

void viewAllAircons() {
    clearScreen();
    displayTimeBar();

    printf(CYAN "\n                               AIRCON UNITS INVENTORY                               \n" RESET);
    
    printf(CYAN "+--------+------------+------------+------------+------------+--------+----------------+\n" RESET);
    printf(CYAN "| Unit   | Brand      | Model      | Type       | Building   | Floor  | Maintenance    |\n" RESET);
    printf(CYAN "+--------+------------+------------+------------+------------+--------+----------------+\n" RESET);

    for (int i = 0; i < totalUnits; i++) {
        if (i % 2 == 0) {
            printf(GREEN "| %-6s | %-10s | %-10s | %-10s | %-10s | %-6s | %-14s |\n" RESET,
                   airconUnits[i].unitNo,
                   airconUnits[i].brand,
                   airconUnits[i].model,
                   airconUnits[i].type,
                   airconUnits[i].building,
                   airconUnits[i].floor,
                   airconUnits[i].lastMaintenance);
        } else {
            printf(YELLOW "| %-6s | %-10s | %-10s | %-10s | %-10s | %-6s | %-14s |\n" RESET,
                   airconUnits[i].unitNo,
                   airconUnits[i].brand,
                   airconUnits[i].model,
                   airconUnits[i].type,
                   airconUnits[i].building,
                   airconUnits[i].floor,
                   airconUnits[i].lastMaintenance);
        }
        if (i < totalUnits - 1) {
            printf(CYAN "+--------+------------+------------+------------+------------+--------+----------------+\n" RESET);
        }
    }

    printf(CYAN "+--------+------------+------------+------------+------------+--------+----------------+\n" RESET);
    
    printf(YELLOW "\nTotal Units: %d\n" RESET, totalUnits);
    printf(GREEN "Hint: Use Unit No when booking or requesting service\n" RESET);
    printf(CYAN "\nPress Enter to continue..." RESET);
    getchar();
    getchar();
}

void technicianMenu() {
    char technicianCode[30], password[30];
    int choice;

    clearScreen();
    printf(CYAN "\nTechnician Login\n" RESET);
    printf(BLUE "Technician Code: " RESET);
    scanf("%s", technicianCode);
    printf(BLUE "Enter password: " RESET);
    takePassword(password, 30);

    int authorized = 0;
    for (int i = 0; i < totalTechnicians; i++) {
        if (strcmp(technicianAccounts[i].technicianCode, technicianCode) == 0 && strcmp(technicianAccounts[i].password, password) == 0) {
            if (technicianAccounts[i].isApproved == 0) {
                printf(RED "Your account is not approved.\n" RESET);
                return;
            }
            authorized = 1;
            break;
        }
    }

    if (!authorized) {
        printf(RED "Technician login failed. Incorrect technician code or password.\n" RESET);
        getchar();
        getchar();
        return;
    }

    logAction("Technician", technicianCode, "Logged In");

    while (1) {
        clearScreen();
        displayTimeBar();  
        printf(CYAN "\n=========================\n" RESET);
        printf(" Technician Menu\n");
        printf(CYAN "=========================\n" RESET);
        printf("\n");
        printf(GREEN "[1] View All Approved Service Requests\n" RESET);
        printf(BLUE "[2] Collaborate with Other Technicians\n" RESET);
        printf(RED "[3] Back to Main Menu\n" RESET);
        printf("\n");
        printf(YELLOW "Enter your choice: " RESET);
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewAllApprovedServiceRequests(technicianCode);
                break;
            case 2:
                collaborateWithOtherTechnicians(technicianCode);
                break;
            case 3:
                return;
            default:
                printf(RED "Invalid choice. Try again.\n" RESET);
                getchar();
                getchar();
        }
    }
}

void viewAssignedServiceRequests(const char* technicianCode) {
    clearScreen();
    displayTimeBar();
    
    printf(CYAN "\n                               ASSIGNED SERVICE REQUESTS                               \n" RESET);
    
    printf(CYAN "+-------------+--------+------------+-----------+------------------+------------------+------------------+\n" RESET);
    printf(CYAN "| Customer    | Unit   | Service    | Status    | Last Updated    | Notes           |\n" RESET);
    printf(CYAN "+-------------+--------+------------+-----------+------------------+------------------+\n" RESET);

    for (int i = 0; i < totalRequests; i++) {
        if (strcmp(serviceRequests[i].technicianCode, technicianCode) == 0) {
            if (i % 2 == 0) {
                printf(GREEN "| %-11s | %-6s | %-10s | %-9s | %-16s | %-14s |\n" RESET,
                       serviceRequests[i].customerCode,
                       serviceRequests[i].unitNo,
                       serviceRequests[i].serviceType,
                       serviceRequests[i].status,
                       serviceRequests[i].lastUpdated,
                       strlen(serviceRequests[i].reason) > 0 ? serviceRequests[i].reason : 
                       (strcmp(serviceRequests[i].status, "Approved") == 0 ? serviceRequests[i].eta : "N/A"));
            } else {
                printf(YELLOW "| %-11s | %-6s | %-10s | %-9s | %-16s | %-14s |\n" RESET,
                       serviceRequests[i].customerCode,
                       serviceRequests[i].unitNo,
                       serviceRequests[i].serviceType,
                       serviceRequests[i].status,
                       serviceRequests[i].lastUpdated,
                       strlen(serviceRequests[i].reason) > 0 ? serviceRequests[i].reason : 
                       (strcmp(serviceRequests[i].status, "Approved") == 0 ? serviceRequests[i].eta : "N/A"));
            }
        }
    }

    printf(CYAN "+-------------+--------+------------+-----------+------------------+------------------+\n" RESET);
    printf(CYAN "\nPress Enter to continue..." RESET);
    getchar();
    getchar();
}

void updateAirconMaintenanceStatus(const char* technicianCode) {  // Add technicianCode parameter
    char unitNo[20], status[50], serviceType[20];
    int found = 0;
    struct tm nextServiceDate = {0};
    time_t now, nextServiceTime;

    clearScreen();
    displayTimeBar();
    
    printf(CYAN "\n                               ASSIGNED AIRCON UNITS                               \n" RESET);
    
    // First display assigned service requests for this technician
    printf(CYAN "\nYour Assigned Service Requests:\n" RESET);
    printf(CYAN "+--------+------------+-----------+------------------+\n" RESET);
    printf(CYAN "| Unit   | Service    | Status    | Customer        |\n" RESET);
    printf(CYAN "+--------+------------+-----------+------------------+\n" RESET);

    for (int i = 0; i < totalRequests; i++) {
        if (strcmp(serviceRequests[i].technicianCode, technicianCode) == 0) {
            printf(GREEN "| %-6s | %-10s | %-9s | %-14s |\n" RESET,
                   serviceRequests[i].unitNo,
                   serviceRequests[i].serviceType,
                   serviceRequests[i].status,
                   serviceRequests[i].customerCode);
        }
    }
    printf(CYAN "+--------+------------+-----------+------------------+\n" RESET);

    while (1) {
        printf(CYAN "Enter Unit No of the aircon to update maintenance status: " RESET);
        scanf("%s", unitNo);

        found = 0;
        for (int i = 0; i < totalRequests; i++) {
            if (strcmp(serviceRequests[i].unitNo, unitNo) == 0 && 
                strcmp(serviceRequests[i].technicianCode, technicianCode) == 0) {
                found = 1;
                printf(CYAN "Enter Service Type (%s): " RESET, serviceRequests[i].serviceType);
                scanf("%s", serviceType);

                if (strcmp(serviceType, serviceRequests[i].serviceType) == 0) {
                    printf(CYAN "Updating Maintenance Status for Aircon Unit: %s\n" RESET, unitNo);
                    printf(CYAN "Enter new Last Maintenance Date (YYYY-MM-DD): " RESET);
                    
                    // Update the correct aircon unit
                    int j; // Declare the variable j here
                    for (j = 0; j < totalUnits; j++) {
                        if (strcmp(airconUnits[j].unitNo, unitNo) == 0) {
                            scanf("%s", airconUnits[j].lastMaintenance);
                            break;
                        }
                    }

                    printf(CYAN "Enter new status (Complete/Delayed/On the way/Ongoing/Critical): " RESET);
                    scanf("%s", status);

                    saveAirconData();
                    updateServiceRequestStatus(i, status);

                    // Set next service date
                    sscanf(airconUnits[j].lastMaintenance, "%d-%d-%d", &nextServiceDate.tm_year, &nextServiceDate.tm_mon, &nextServiceDate.tm_mday);
                    nextServiceDate.tm_year -= 1900;
                    nextServiceDate.tm_mon -= 1;

                    if (strcasecmp(serviceType, "Clean") == 0) {
                        nextServiceDate.tm_mon += 3;  // Add 3 months for cleaning
                    } else if (strcasecmp(serviceType, "Maintenance") == 0) {
                        nextServiceDate.tm_mon += 6;  // Add 6 months for maintenance
                    }

                    nextServiceTime = mktime(&nextServiceDate);
                    strftime(airconUnits[j].lastMaintenance, sizeof(airconUnits[j].lastMaintenance), "%Y-%m-%d", localtime(&nextServiceTime));

                    printf(GREEN "Aircon maintenance status updated successfully!\n" RESET);
                    logAction("Technician", technicianCode, "Updated Aircon Maintenance Status");
                    break;
                }
            }
        }

        if (!found) {
            printf(RED "Unit No not found or not assigned to you. Please try again.\n" RESET);
            continue;
        }

        break;
    }

    printf(CYAN "\nPress Enter to continue..." RESET);
    getchar();
    getchar();
}

void cancelServiceBooking(const char* customerCode) {
    int found = 0;
    char unitNo[20], serviceType[20];
    
    clearScreen();
    printf(CYAN "Your Current Service Requests:\n" RESET);
    for (int i = 0; i < totalRequests; i++) {
        if (strcmp(serviceRequests[i].customerCode, customerCode) == 0 &&
            strcmp(serviceRequests[i].status, "Pending") == 0) {
            printf(GREEN "Unit No: %s, Service Type: %s, Status: %s, Booked At: %s\n" RESET,
                   serviceRequests[i].unitNo, serviceRequests[i].serviceType, serviceRequests[i].status, serviceRequests[i].eta);
            found = 1;
        }
    }

    if (!found) {
        printf(RED "No pending service requests found.\n" RESET);
        getchar();
        getchar();
        return;
    }

    printf(CYAN "\nEnter Unit No of the service to cancel: " RESET);
    scanf("%s", unitNo);
    printf(CYAN "Enter Service Type to cancel (Clean/Maintenance/Repair): " RESET);
    scanf("%s", serviceType);

    found = 0;
    for (int i = 0; i < totalRequests; i++) {
        if (strcmp(serviceRequests[i].customerCode, customerCode) == 0 &&
            strcmp(serviceRequests[i].unitNo, unitNo) == 0 &&
            strcmp(serviceRequests[i].serviceType, serviceType) == 0 &&
            strcmp(serviceRequests[i].status, "Pending") == 0) {
            
            time_t now;
            time(&now);
            char timestamp[30];
            strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
            
            strcpy(serviceRequests[i].status, "Cancelled");
            strcpy(serviceRequests[i].cancelledAt, timestamp);
            strcpy(serviceRequests[i].lastUpdated, timestamp);
            
            saveServiceRequestData();
            
            printf(GREEN "Service request cancelled successfully at %s!\n" RESET, timestamp);
            logAction("Customer", customerCode, "Cancelled Service Request");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf(RED "No pending service request found for the specified unit and service type.\n" RESET);
    }
    
    getchar();
    getchar();
}

void checkAccountStatus() {
    char trackingCode[50];
    int found = 0;

    clearScreen();
    displayTimeBar();  
    printf(CYAN "Enter your tracking code: " RESET);
    scanf("%s", trackingCode);

    for (int i = 0; i < totalCustomers; i++) {
        if (strcmp(customerAccounts[i].customerCode, trackingCode) == 0) {
            printf(GREEN "\nAccount Type: Customer\n" RESET);
            printf(YELLOW "Status: %s\n" RESET, 
                customerAccounts[i].isApproved ? "Approved" : "Pending Approval");
            if (customerAccounts[i].isApproved) {
                printf(GREEN "Your Customer Code: %s\n" RESET, customerAccounts[i].customerCode);
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        for (int i = 0; i < totalAdmins; i++) {
            if (strcmp(adminAccounts[i].username, trackingCode) == 0) {
                printf(BLUE "\nAccount Type: Admin\n" RESET);
                printf(YELLOW "Status: %s\n" RESET, 
                    adminAccounts[i].isApproved ? "Approved" : "Pending Approval");
                found = 1;
                break;
            }
        }
    }

    if (!found) {
        for (int i = 0; i < totalTechnicians; i++) {
            if (strcmp(technicianAccounts[i].technicianCode, trackingCode) == 0) {
                printf(YELLOW "\nAccount Type: Technician\n" RESET);
                printf(YELLOW "Status: %s\n" RESET, 
                    technicianAccounts[i].isApproved ? "Approved" : "Pending Approval");
                found = 1;
                break;
            }
        }
    }

    if (!found) {
        printf(RED "\nNo account found with the provided tracking code.\n" RESET);
    }

    printf(CYAN "\nPress Enter to continue..." RESET);
    getchar();
    getchar();
}

void displayTimeBar() {
    time_t now;
    struct tm *local;
    char timeStr[100];
    
    time(&now);
    local = localtime(&now);
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", local);
    
    printf(CYAN "=========================\n" RESET);
    printf(YELLOW "Current Time: %s\n" RESET, timeStr);
    printf(CYAN "=========================\n" RESET);
}

void assignAirconsToCustomer() {
    char customerCode[50], unitNo[20];
    int found = 0;

    clearScreen();
    printf(CYAN "Customers with Aircon Assignment Help Tickets:\n" RESET);
    printf(CYAN "+-------------+----------------+----------------+\n" RESET);
    printf(CYAN "| Customer    | Employee ID    | Request        |\n" RESET);
    printf(CYAN "+-------------+----------------+----------------+\n" RESET);

    FILE *file = fopen("helptickets.txt", "r");
    if (file == NULL) {
        printf(RED "Error: Could not open helptickets.txt file.\n" RESET);
        return;
    }

    char issueType[20], description[100], timestamp[30], status[20];
    while (fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^\n]\n", customerCode, issueType, description, timestamp, status) == 5) {
        if (strcmp(issueType, "Aircon Assignment") == 0) {
            for (int i = 0; i < totalCustomers; i++) {
                if (strcmp(customerAccounts[i].customerCode, customerCode) == 0) {
                    printf(GREEN "| %-11s | %-14s | %-14s |\n" RESET, customerCode, customerAccounts[i].employeeId, description);
                    break;
                }
            }
        }
    }

    fclose(file);
    printf(CYAN "+-------------+----------------+----------------+\n" RESET);

    printf(CYAN "Enter Customer Code to assign aircon units (or type 'back' to return): " RESET);
    scanf("%s", customerCode);

    if (strcasecmp(customerCode, "back") == 0) {
        return;
    }

    for (int i = 0; i < totalCustomers; i++) {
        if (strcmp(customerAccounts[i].customerCode, customerCode) == 0 && customerAccounts[i].isApproved == 1) {
            found = 1;
            break;
        }
    }

    if (!found) {
        printf(RED "Customer code not found or not approved.\n" RESET);
        getchar();
        getchar();
        return;
    }

    printf(CYAN "\n                               UNASSIGNED AIRCON UNITS                               \n" RESET);
    printf(CYAN "+--------+------------+------------+------------+------------+--------+----------------+\n" RESET);
    printf(CYAN "| Unit   | Brand      | Model      | Type       | Building   | Floor  | Maintenance    |\n" RESET);
    printf(CYAN "+--------+------------+------------+------------+------------+--------+----------------+\n" RESET);

    for (int i = 0; i < totalUnits; i++) {
        int assigned = 0;
        for (int j = 0; j < totalAssignments; j++) {
            if (strcmp(airconUnits[i].unitNo, airconAssignments[j].unitNo) == 0) {
                assigned = 1;
                break;
            }
        }
        if (!assigned) {
            printf(GREEN "| %-6s | %-10s | %-10s | %-10s | %-10s | %-6s | %-14s |\n" RESET,
                   airconUnits[i].unitNo,
                   airconUnits[i].brand,
                   airconUnits[i].model,
                   airconUnits[i].type,
                   airconUnits[i].building,
                   airconUnits[i].floor,
                   airconUnits[i].lastMaintenance);
        }
    }

    printf(CYAN "+--------+------------+------------+------------+------------+--------+----------------+\n" RESET);

    while (1) {
        printf(CYAN "Enter Unit No to assign to customer (or type 'done' to finish): " RESET);
        scanf("%s", unitNo);

        if (strcasecmp(unitNo, "done") == 0) {
            break;
        }

        found = 0;
        for (int i = 0; i < totalUnits; i++) {
            if (strcmp(airconUnits[i].unitNo, unitNo) == 0) {
                int assigned = 0;
                for (int j = 0; j < totalAssignments; j++) {
                    if (strcmp(airconAssignments[j].unitNo, unitNo) == 0) {
                        assigned = 1;
                        break;
                    }
                }
                if (!assigned) {
                    found = 1;
                    break;
                }
            }
        }

        if (!found) {
            printf(RED "Unit No not found or already assigned. Please try again.\n" RESET);
            continue;
        }

        strcpy(airconAssignments[totalAssignments].customerCode, customerCode);
        strcpy(airconAssignments[totalAssignments].unitNo, unitNo);
        totalAssignments++;

        saveAirconAssignments();

        printf(GREEN "Aircon unit %s assigned to customer %s successfully.\n" RESET, unitNo, customerCode);
        logAction("Admin", customerCode, "Assigned Aircon Unit");
    }

    printf(CYAN "\nPress Enter to continue..." RESET);
    getchar();
    getchar();
}

void loadAirconAssignments() {
    FILE *file = fopen("airconassignments.txt", "r");
    if (file == NULL) {
        printf(RED "Error: Could not open airconassignments.txt file.\n" RESET);
        return;
    }

    while (fscanf(file, "%[^,],%s\n", airconAssignments[totalAssignments].customerCode, 
                  airconAssignments[totalAssignments].unitNo) == 2) {
        totalAssignments++;
    }

    fclose(file);
}

void saveAirconAssignments() {
    FILE *file = fopen("airconassignments.txt", "w");
    if (file == NULL) {
        printf(RED "Error: Could not open airconassignments.txt file for writing.\n" RESET);
        return;
    }

    for (int i = 0; i < totalAssignments; i++) {
        fprintf(file, "%s,%s\n", airconAssignments[i].customerCode, airconAssignments[i].unitNo);
    }

    fclose(file);
}

void submitHelpTicket(const char* customerCode) {
    char issueType[20], description[100];
    time_t now;
    char timestamp[30];

    clearScreen();
    printf(CYAN "Submit a Help Ticket\n" RESET);
    printf(CYAN "====================\n" RESET);
    printf(GREEN "[1] Aircon Assignment\n" RESET);
    printf(YELLOW "[2] Other Issue\n" RESET);
    printf(YELLOW "Enter the type of issue: " RESET);
    scanf("%s", issueType);

    printf(CYAN "Describe your issue: " RESET);
    getchar();  
    fgets(description, sizeof(description), stdin);
    description[strcspn(description, "\n")] = 0;  

    time(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

    FILE *file = fopen("helptickets.txt", "a");
    if (file == NULL) {
        printf(RED "Error: Could not open helptickets.txt file for writing.\n" RESET);
        return;
    }

    fprintf(file, "%s,%s,%s,%s,Pending\n", customerCode, issueType, description, timestamp);
    fclose(file);

    printf(GREEN "Help ticket submitted successfully at %s!\n" RESET, timestamp);
    logAction("Customer", customerCode, "Submitted Help Ticket");
    getchar();
    getchar();
}

void viewHelpTickets() {
    clearScreen();
    displayTimeBar();  
    FILE *file = fopen("helptickets.txt", "r");
    if (file == NULL) {
        printf(RED "Error: Could not open helptickets.txt file.\n" RESET);
        return;
    }

    char customerCode[50], issueType[20], description[100], timestamp[30], status[20];
    printf(CYAN "Help Tickets:\n" RESET);
    printf(CYAN "+-------------+-------------------+------------------------------------------+-------------------+-------------------+\n" RESET);
    printf(CYAN "| Customer    | Issue Type        | Description                              | Timestamp         | Status            |\n" RESET);
    printf(CYAN "+-------------+-------------------+------------------------------------------+-------------------+-------------------+\n" RESET);

    while (fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^\n]\n", customerCode, issueType, description, timestamp, status) == 5) {
        printf(GREEN "| %-11s | %-17s | %-40s | %-17s | %-17s |\n" RESET, customerCode, issueType, description, timestamp, status);
    }

    printf(CYAN "+-------------+-------------------+------------------------------------------+-------------------+-------------------+\n" RESET);
    fclose(file);

    printf(CYAN "Enter Customer Code to view or edit the status (or type 'back' to return): " RESET);
    scanf("%s", customerCode);

    if (strcasecmp(customerCode, "back") == 0) {
        return;
    }

    printf(CYAN "Enter new status (On Process/Finished): " RESET);
    scanf("%s", status);

    file = fopen("helptickets.txt", "r+");
    if (file == NULL) {
        printf(RED "Error: Could not open helptickets.txt file.\n" RESET);
        return;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf(RED "Error: Could not open temp.txt file.\n" RESET);
        fclose(file);
        return;
    }

    while (fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^\n]\n", customerCode, issueType, description, timestamp, status) == 5) {
        if (strcmp(customerCode, customerCode) == 0) {
            fprintf(tempFile, "%s,%s,%s,%s,%s\n", customerCode, issueType, description, timestamp, status);
        } else {
            fprintf(tempFile, "%s,%s,%s,%s,%s\n", customerCode, issueType, description, timestamp, status);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("helptickets.txt");
    rename("temp.txt", "helptickets.txt");

    printf(GREEN "Status updated successfully!\n" RESET);
    logAction("Admin", customerCode, "Updated Help Ticket Status");
    printf(CYAN "\nPress Enter to continue..." RESET);
    getchar();
    getchar();
}

int validateEmail(const char* email) {
    const char* domain = "@ctu.edu.ph";
    int emailLen = strlen(email);
    int domainLen = strlen(domain);
    if (emailLen < domainLen) {
        return 0;
    }
    return strcmp(email + emailLen - domainLen, domain) == 0;
}

void saveUserCredentials() {
    FILE *file = fopen("usercred.txt", "w");
    if (file == NULL) {
        printf(RED "Error: Could not open usercred.txt file for writing.\n" RESET);
        return;
    }

    // Save customer credentials
    for (int i = 0; i < totalCustomers; i++) {
        fprintf(file, "Customer,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
                customerAccounts[i].customerCode,
                customerAccounts[i].password,
                customerAccounts[i].firstName,
                customerAccounts[i].middleName,
                customerAccounts[i].surname,
                customerAccounts[i].email,
                customerAccounts[i].mobileNumber,
                customerAccounts[i].employeeId,
                customerAccounts[i].registrationTime,
                customerAccounts[i].college,
                customerAccounts[i].building,
                customerAccounts[i].floor,
                customerAccounts[i].room,
                customerAccounts[i].isApproved ? "Approved" : "Pending Approval");
    }

    // Save admin credentials
    for (int i = 0; i < totalAdmins; i++) {
        fprintf(file, "Admin,%s,%s,%d\n",
                adminAccounts[i].username,
                adminAccounts[i].password,
                adminAccounts[i].isApproved);
    }

    // Save technician credentials
    for (int i = 0; i < totalTechnicians; i++) {
        fprintf(file, "Technician,%s,%s,%d\n",
                technicianAccounts[i].technicianCode,
                technicianAccounts[i].password,
                technicianAccounts[i].isApproved);
    }

    fclose(file);
}

void saveRandomGeneratedCode(const char* customerCode) {
    FILE *file = fopen("trackingcodes.txt", "a");
    if (file == NULL) {
        printf(RED "Error: Could not open trackingcodes.txt file for writing.\n" RESET);
        return;
    }

    fprintf(file, "%s\n", customerCode);
    fclose(file);
}

void saveRandomGeneratedCodeToRGCStat(const char* customerCode, const char* trackingCode) {
    FILE *file = fopen("rgc-stat.txt", "a");
    if (file == NULL) {
        printf(RED "Error: Could not open rgc-stat.txt file for writing.\n" RESET);
        return;
    }

    fprintf(file, "%s,%s\n", customerCode, trackingCode);
    fclose(file);
}

int parseDate(const char* dateStr, struct tm* date) {
    return sscanf(dateStr, "%d-%d-%d", &date->tm_year, &date->tm_mon, &date->tm_mday);
}

void viewNotifications(const char* customerCode) {
    clearScreen();
    displayTimeBar();  
    printf(CYAN "Notifications for Customer Code: %s\n" RESET, customerCode);

    for (int i = 0; i < totalAssignments; i++) {
        if (strcmp(airconAssignments[i].customerCode, customerCode) == 0) {
            for (int j = 0; j < totalUnits; j++) {
                if (strcmp(airconUnits[j].unitNo, airconAssignments[i].unitNo) == 0) {
                    time_t now;
                    time(&now);
                    struct tm lastMaintenance = {0};
                    if (parseDate(airconUnits[j].lastMaintenance, &lastMaintenance) == 3) {
                        lastMaintenance.tm_year -= 1900;
                        lastMaintenance.tm_mon -= 1;
                        double daysSinceLastMaintenance = difftime(now, mktime(&lastMaintenance)) / (60 * 60 * 24);

                        if (daysSinceLastMaintenance >= 70 && daysSinceLastMaintenance < 90) {
                            printf(YELLOW "Reminder: Your aircon unit %s needs cleaning soon. Last cleaned on %s.\n" RESET, airconUnits[j].unitNo, airconUnits[j].lastMaintenance);
                        } else if (daysSinceLastMaintenance >= 150 && daysSinceLastMaintenance < 180) {
                            printf(YELLOW "Reminder: Your aircon unit %s needs maintenance soon. Last maintained on %s.\n" RESET, airconUnits[j].unitNo, airconUnits[j].lastMaintenance);
                        }
                    }
                }
            }
        }
    }

    FILE *file = fopen("helptickets.txt", "r");
    if (file == NULL) {
        printf(RED "Error: Could not open helptickets.txt file.\n" RESET);
        return;
    }

    char issueType[20], description[100], timestamp[30], status[20];
    while (fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^\n]\n", customerCode, issueType, description, timestamp, status) == 5) {
        printf(GREEN "Help Ticket: %s - %s\n" RESET, issueType, status);
    }

    fclose(file);

    printf(CYAN "\nPress Enter to continue..." RESET);
    getchar();
    getchar();
}

void deassignAirconUnits() {
    char customerCode[50], unitNo[20];
    int found = 0;

    clearScreen();
    printf(CYAN "Customers with Assigned Aircon Units:\n" RESET);
    printf(CYAN "+-------------+--------+\n" RESET);
    printf(CYAN "| Customer    | Unit   |\n" RESET);
    printf(CYAN "+-------------+--------+\n" RESET);

    for (int i = 0; i < totalAssignments; i++) {
        printf(GREEN "| %-11s | %-6s |\n" RESET, airconAssignments[i].customerCode, airconAssignments[i].unitNo);
    }

    printf(CYAN "+-------------+--------+\n" RESET);

    printf(CYAN "Enter Customer Code to de-assign aircon units (or type 'back' to return): " RESET);
    scanf("%s", customerCode);

    if (strcasecmp(customerCode, "back") == 0) {
        return;
    }

    for (int i = 0; i < totalCustomers; i++) {
        if (strcmp(customerAccounts[i].customerCode, customerCode) == 0 && customerAccounts[i].isApproved == 1) {
            found = 1;
            break;
        }
    }

    if (!found) {
        printf(RED "Customer code not found or not approved.\n" RESET);
        getchar();
        getchar();
        return;
    }

    while (1) {
        printf(CYAN "Enter Unit No to de-assign from customer (or type 'done' to finish): " RESET);
        scanf("%s", unitNo);

        if (strcasecmp(unitNo, "done") == 0) {
            break;
        }

        found = 0;
        for (int i = 0; i < totalAssignments; i++) {
            if (strcmp(airconAssignments[i].customerCode, customerCode) == 0 && 
                strcmp(airconAssignments[i].unitNo, unitNo) == 0) {
                for (int j = i; j < totalAssignments - 1; j++) {
                    airconAssignments[j] = airconAssignments[j + 1];
                }
                totalAssignments--;
                saveAirconAssignments();
                printf(GREEN "Aircon unit %s de-assigned from customer %s successfully.\n" RESET, unitNo, customerCode);
                logAction("Admin", customerCode, "De-assigned Aircon Unit");
                found = 1;
                break;
            }
        }

        if (!found) {
            printf(RED "Unit No not found or not assigned to the specified customer. Please try again.\n" RESET);
        }
    }

    printf(CYAN "\nPress Enter to continue..." RESET);
    getchar();
    getchar();
}

void viewAllApprovedServiceRequests(const char* technicianCode) {
    clearScreen();
    displayTimeBar();
    
    printf(CYAN "\n                               APPROVED SERVICE REQUESTS                               \n" RESET);
    
    printf(CYAN "+-------------+--------+------------+-----------+------------------+------------------+------------------+\n" RESET);
    printf(CYAN "| Customer    | Unit   | Service    | Status    | Last Updated    | Notes           |\n" RESET);
    printf(CYAN "+-------------+--------+------------+-----------+------------------+------------------+\n" RESET);

    for (int i = 0; i < totalRequests; i++) {
        if (strcmp(serviceRequests[i].status, "Approved") == 0) {
            if (i % 2 == 0) {
                printf(GREEN "| %-11s | %-6s | %-10s | %-9s | %-16s | %-14s |\n" RESET,
                       serviceRequests[i].customerCode,
                       serviceRequests[i].unitNo,
                       serviceRequests[i].serviceType,
                       serviceRequests[i].status,
                       serviceRequests[i].lastUpdated,
                       strlen(serviceRequests[i].reason) > 0 ? serviceRequests[i].reason : 
                       (strcmp(serviceRequests[i].status, "Approved") == 0 ? serviceRequests[i].eta : "N/A"));
            } else {
                printf(YELLOW "| %-11s | %-6s | %-10s | %-9s | %-16s | %-14s |\n" RESET,
                       serviceRequests[i].customerCode,
                       serviceRequests[i].unitNo,
                       serviceRequests[i].serviceType,
                       serviceRequests[i].status,
                       serviceRequests[i].lastUpdated,
                       strlen(serviceRequests[i].reason) > 0 ? serviceRequests[i].reason : 
                       (strcmp(serviceRequests[i].status, "Approved") == 0 ? serviceRequests[i].eta : "N/A"));
            }
        }
    }

    printf(CYAN "+-------------+--------+------------+-----------+------------------+------------------+\n" RESET);

    while (1) {
        printf(CYAN "Enter Unit No to assign to yourself (or type 'back' to return): " RESET);
        char unitNo[20];
        scanf("%s", unitNo);

        if (strcasecmp(unitNo, "back") == 0) {
            return;
        }

        int found = 0;
        for (int i = 0; i < totalRequests; i++) {
            if (strcmp(serviceRequests[i].unitNo, unitNo) == 0 && strcmp(serviceRequests[i].status, "Approved") == 0) {
                strcpy(serviceRequests[i].technicianCode, technicianCode);
                strcpy(serviceRequests[i].status, "In Progress");
                time_t now;
                time(&now);
                strftime(serviceRequests[i].lastUpdated, sizeof(serviceRequests[i].lastUpdated), "%Y-%m-%d %H:%M:%S", localtime(&now));
                saveServiceRequestData();
                printf(GREEN "Service request assigned to you successfully.\n" RESET);
                logAction("Technician", technicianCode, "Assigned to Service Request");
                found = 1;
                break;
            }
        }

        if (!found) {
            printf(RED "Service request not found or already assigned. Please try again.\n" RESET);
        }
    }
}

void collaborateWithOtherTechnicians(const char* technicianCode) {
    clearScreen();
    displayTimeBar();
    
    printf(CYAN "\n                               COLLABORATE WITH OTHER TECHNICIANS                               \n" RESET);
    
    printf(CYAN "+-------------+--------+------------+-----------+------------------+------------------+------------------+\n" RESET);
    printf(CYAN "| Customer    | Unit   | Service    | Status    | Last Updated    | Notes           |\n" RESET);
    printf(CYAN "+-------------+--------+------------+-----------+------------------+------------------+\n" RESET);

    for (int i = 0; i < totalRequests; i++) {
        if (strcmp(serviceRequests[i].status, "In Progress") == 0 && strcmp(serviceRequests[i].technicianCode, technicianCode) != 0) {
            if (i % 2 == 0) {
                printf(GREEN "| %-11s | %-6s | %-10s | %-9s | %-16s | %-14s |\n" RESET,
                       serviceRequests[i].customerCode,
                       serviceRequests[i].unitNo,
                       serviceRequests[i].serviceType,
                       serviceRequests[i].status,
                       serviceRequests[i].lastUpdated,
                       strlen(serviceRequests[i].reason) > 0 ? serviceRequests[i].reason : 
                       (strcmp(serviceRequests[i].status, "In Progress") == 0 ? serviceRequests[i].eta : "N/A"));
            } else {
                printf(YELLOW "| %-11s | %-6s | %-10s | %-9s | %-16s | %-14s |\n" RESET,
                       serviceRequests[i].customerCode,
                       serviceRequests[i].unitNo,
                       serviceRequests[i].serviceType,
                       serviceRequests[i].status,
                       serviceRequests[i].lastUpdated,
                       strlen(serviceRequests[i].reason) > 0 ? serviceRequests[i].reason : 
                       (strcmp(serviceRequests[i].status, "In Progress") == 0 ? serviceRequests[i].eta : "N/A"));
            }
        }
    }

    printf(CYAN "+-------------+--------+------------+-----------+------------------+------------------+\n" RESET);

    while (1) {
        printf(CYAN "Enter Unit No to collaborate on (or type 'back' to return): " RESET);
        char unitNo[20];
        scanf("%s", unitNo);

        if (strcasecmp(unitNo, "back") == 0) {
            return;
        }

        int found = 0;
        for (int i = 0; i < totalRequests; i++) {
            if (strcmp(serviceRequests[i].unitNo, unitNo) == 0 && strcmp(serviceRequests[i].status, "In Progress") == 0) {
                strcat(serviceRequests[i].technicianCode, ",");
                strcat(serviceRequests[i].technicianCode, technicianCode);
                time_t now;
                time(&now);
                strftime(serviceRequests[i].lastUpdated, sizeof(serviceRequests[i].lastUpdated), "%Y-%m-%d %H:%M:%S", localtime(&now));
                saveServiceRequestData();
                printf(GREEN "You are now collaborating on this service request.\n" RESET);
                logAction("Technician", technicianCode, "Collaborating on Service Request");
                found = 1;
                break;
            }
        }

        if (!found) {
            printf(RED "Service request not found or already assigned. Please try again.\n" RESET);
        }
    }
}

int main() {
    loadAdminData();
    loadUserData();
    loadAirconData();
    loadTechnicianData();
    loadServiceRequestData();
    loadAirconAssignments();

    displayMainMenu();

    return 0;
}