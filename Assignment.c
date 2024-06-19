#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Data Structures
struct Patient {
    int id;
    char name[50];
    char password[20];
};

struct Appointment {
    int appointmentId;
    int patientId;
    char date[20];
    char doctor[50];
    char status[20];
};

struct Bill {
    int billId;
    int patientId;
    char date[20];
    double amount;
    char description[100];
};

int loggedInPatientId = -1;

// Function Declaration
void login() {
    char user[50];
    char password[20];
    char line[100];
    struct Patient patient;
    int found = 0;

    printf("Enter your UserID: ");
    scanf("%s", user);
    printf("Enter your password: ");
    scanf("%s", password);

    FILE *file = fopen("patients.txt", "r");

    while(fgets(line,sizeof(line),file))
    {
        line[strcspn(line,"\n")] = '\0';
        char *patientId = strtok(line,"|");
        char *name = strtok(NULL,"|");
        char *pass = strtok(NULL,"|");
        if(strcmp(user,patientId)==0 && strcmp(password,pass)==0)
        {
            printf("Login Successful\n");
            found = 1;
            menu(user);
        }
    }
    fclose(file);

    if (!found)
    {
        printf("Error\n");
    }
}

void logout() {
    loggedInPatientId = -1;
    printf("Logged out successfully.\n");
}

void viewAppointments(char*user) {
    char line[100];
    FILE *file = fopen("appointments.txt", "r");

    printf("Your Appointments:\n");
    while(fgets(line,sizeof(line),file))
    {
        line[strcspn(line,"\n")] = '\0';
        char *appointmentId = strtok(line,"|");
        char *patientId = strtok(NULL,"|");
        char *date = strtok(NULL,"|");
        char *doctor = strtok(NULL,"|");
        char *status = strtok(NULL,"|");
        if(strcmp(user,patientId)==0)
        {
            printf("%s, %s, %s, %s\n\n",patientId, date, doctor,status);
            menu(user);
        }
    }
    fclose(file);
}

void rescheduleAppointment(char*user) {
    char aptid[50];
    char newDate[50];
    char line[255];
    char line1[100];
    struct Appointment appointment;
    struct Appointment appointments[100];
    int found1 = 0;

    FILE *file = fopen("appointments.txt", "r");

    while(fgets(line,sizeof(line),file))
    {
        line[strcspn(line,"\n")] = '\0';
        char *appointmentId = strtok(line,"|");
        char *patientId = strtok(NULL,"|");
        char *date = strtok(NULL,"|");
        char *doctor = strtok(NULL,"|");
        char *status = strtok(NULL,"|");
        if(strcmp(user,patientId)==0)
        {
            printf("\n%s, %s, %s\n\n",appointmentId, date, doctor);
        }
    }
    fclose(file);

    printf("Enter Appointment ID to reschedule: ");
    scanf("%s", aptid);
    printf("Enter new date (YYYY-MM-DD): ");
    scanf("%s", newDate);
    
    FILE * file1 = fopen("appointments.txt","r");
    FILE * file2 = fopen("temp.txt", "w");

    while(fgets(line1,sizeof(line1),file1))
    {
        line1[strcspn(line1,"\n")] = '\0';
        char *appointmentId = strtok(line1,"|");
        char *patientId = strtok(NULL,"|");
        char *date = strtok(NULL,"|");
        char *doctor = strtok(NULL,"|");
        char *status = strtok(NULL,"|");

        if (strcmp(aptid,appointmentId)==0 && strcmp(user,patientId) == 0)
        {
            date = newDate;
            found1 = 1;
        }

        fprintf(file2, "%s|%s|%s|%s|%s\n", appointmentId, patientId, date, doctor, status);
    }
    fclose(file1);
    fclose(file2);

    if (!found1)
    {
        printf("Appointment is not found\n");
        remove("temp.txt"); 
        menu(user);
    }

    remove("appointments.txt");
    rename("temp.txt", "appointments.txt");
    menu(user);
}

void cancelAppointment(char*user) {
    char aptid[50];
    char cancel[50];
    char line[255];
    char line1[100];
    struct Appointment appointment;
    struct Appointment appointments[100];
    int found1 = 0;

    FILE *file = fopen("appointments.txt", "r");

    while(fgets(line,sizeof(line),file))
    {
        line[strcspn(line,"\n")] = '\0';
        char *appointmentId = strtok(line,"|");
        char *patientId = strtok(NULL,"|");
        char *date = strtok(NULL,"|");
        char *doctor = strtok(NULL,"|");
        char *status = strtok(NULL,"|");
        if(strcmp(user,patientId)==0)
        {
            printf("%s, %s, %s\n\n",appointmentId, date, doctor);
        }
        
    }
    fclose(file);

    printf("Enter Appointment ID to cancel: ");
    scanf("%s", aptid);

    printf("Are you sure want to cancel this appointment? (y/n): ");
    scanf("%s", cancel);
                
    if (strcmp (cancel, "Y") == 0 || strcmp(cancel, "y") == 0)
    {
        FILE * file1 = fopen("appointments.txt","r");
        FILE * file2 = fopen("temp.txt", "w");

        while(fgets(line1,sizeof(line1),file1))
        {
            line1[strcspn(line1,"\n")] = '\0';
            char *appointmentId = strtok(line1,"|");
            char *patientId = strtok(NULL,"|");
            char *date = strtok(NULL,"|");
            char *doctor = strtok(NULL,"|");
            char *status = strtok(NULL,"|");

            if (strcmp(aptid,appointmentId)==0 && strcmp(user,patientId) == 0 && strcmp(status,"Available") == 0)
            {
                status = "Cancelled";
                found1 = 1;
            }
            else if (strcmp(aptid,appointmentId)==0 && strcmp(user,patientId) == 0 && strcmp(status,"Cancelled")==0)
            {
                printf("\nError: This Appointment has been cancelled.\n\n");
                found1 = 1;
            }

            fprintf(file2, "%s|%s|%s|%s|%s\n", appointmentId, patientId, date, doctor, status);
        }
        fclose(file1);
        fclose(file2);

        if (!found1)
        {
            printf("Appointment is not found\n");
            remove("temp.txt"); 
            menu(user);
        }

        remove("appointments.txt");
        rename("temp.txt", "appointments.txt");
        menu(user);
    }

    else if (strcmp (cancel, "N") == 0 || strcmp(cancel, "n") == 0)
    {
        menu(user);
    }
     
}

void viewEHR(char*user) {
    char line[100];
    FILE *file = fopen("ehr.txt", "r");

    printf("\nYour Electronic Health Record:\n");
    while(fgets(line,sizeof(line),file))
    {
        line[strcspn(line,"\n")] = '\0';
        char *patientId = strtok(line,"|");
        char *name = strtok(NULL,"|");
        char *bloodtype = strtok(NULL,"|");
        char *allergies = strtok(NULL,"|");
        char *medicalcond = strtok(NULL,"|");
        char *surgery = strtok(NULL,"|");
        char *date = strtok(NULL,"|");
        if(strcmp(user,patientId)==0)
        {
            printf("Patient ID: %s\nPatient Name: %s\nBlood Type: %s\nAllergies: %s\nMedical Condition: %s\nSurgical History: %s\nDate: %s\n\n",patientId, name, bloodtype, allergies, medicalcond, surgery, date);
            menu(user);
        }
    }
    fclose(file);
}

void viewBills(char*user) {
    char line[100];
    char code[50];
    int found=0;
    FILE *file = fopen("bills.txt", "r");

    printf("\nEnter your billing ID: \n");
    scanf("%s",code);
    while(fgets(line,sizeof(line),file))
    {
        line[strcspn(line,"\n")] = '\0';
        char *billingId = strtok(line,"|");
        char *patientId = strtok(NULL,"|");
        char *name = strtok(NULL,"|");
        char *activities = strtok(NULL,"|");
        char *price = strtok(NULL,"|");
        char *pay = strtok(NULL,"|");
        char *insurance = strtok(NULL,"|");
        if(strcmp(user,patientId)==0 && strcmp(code,billingId)==0)
        {
            printf("\nBillingId: %s\npatientId: %s\nPatient Name: %s\nActivities: %s\n%s\n%s\n%s\n\n",billingId,patientId, name, activities, price, pay, insurance);
            found=1;
            menu(user);
        }
    }

    if (!found)
    {
        printf("Billing ID is not found.");
        menu(user);
    }
    
    fclose(file);
}

void menu(char*user) {
    int choice;

    printf(" __________________________________\n");
    printf("|                                  |\n");
    printf("|=== Patients Management System ===|\n");
    printf("|__________________________________|\n");
    printf("|                                  |\n");
    printf("|1. View Appointments              |\n");
    printf("|2. Reschedule Appointment         |\n");
    printf("|3. Cancel Appointment             |\n");
    printf("|4. View Electronic Health Records |\n");
    printf("|5. View Bills                     |\n");
    printf("|6. Logout                         |\n");
    printf("|__________________________________|\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            viewAppointments(user);
            break;
        case 2:
            rescheduleAppointment(user);
            break;
        case 3:
            cancelAppointment(user);
            break;
        case 4:
            viewEHR(user);
            break;
        case 5:
            viewBills(user);
            break;
        case 6:
            logout();
            break;
        default:
            printf("Invalid choice. Please try again.\n");
    }
}

int main() {
    login();
}