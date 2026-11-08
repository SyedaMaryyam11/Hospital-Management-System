#include <iostream>
#include <fstream>
#include <cstring>
#include <limits>
#include <iomanip>
#include <cstdlib>

using namespace std;

const int MAX_PATIENTS = 100;
const char DATA_FILE[] = "patients.txt";

struct Patient {
    int id;
    char name[50];
    int age;
    char disease[50];
    int consultationFee;
    int medicineCharges;
    int otherCharges;
    int bill;
};

Patient patients[MAX_PATIENTS];
int totalPatients = 0;

int readInt(const char message[]) {
    int value;

    while (true) {
        cout << message;

        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Invalid input. Please enter a whole number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void readText(const char message[], char destination[], int size) {
    while (true) {
        cout << message;
        cin.getline(destination, size);

        if (cin) {
            if (strlen(destination) > 0) {
                return;
            }
            cout << "This field cannot be empty.\n";
        }
        else {
            cout << "Input is too long. Please try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.get();
}

void clearScreen() {
    // system("cls") works in the Windows console used by Visual Studio.
    system("cls");
}

int findPatient(int id) {
    for (int i = 0; i < totalPatients; i++) {
        if (patients[i].id == id) {
            return i;
        }
    }

    return -1;
}

void calculateBill(int index) {
    patients[index].bill =
        patients[index].consultationFee +
        patients[index].medicineCharges +
        patients[index].otherCharges;
}

void saveToFile() {
    ofstream file(DATA_FILE);

    if (!file) {
        cout << "\nError: Patient records could not be saved.\n";
        return;
    }

    for (int i = 0; i < totalPatients; i++) {
        file << patients[i].id << '\n';
        file << patients[i].name << '\n';
        file << patients[i].age << '\n';
        file << patients[i].disease << '\n';
        file << patients[i].consultationFee << '\n';
        file << patients[i].medicineCharges << '\n';
        file << patients[i].otherCharges << '\n';
        file << patients[i].bill << '\n';
    }

    if (file) {
        cout << "\nRecords saved successfully.\n";
    }
    else {
        cout << "\nError occurred while saving records.\n";
    }

    file.close();
}

void loadFromFile() {
    ifstream file(DATA_FILE);

    if (!file) {
        // The file will be created when the first record is saved.
        return;
    }

    totalPatients = 0;

    while (totalPatients < MAX_PATIENTS) {
        Patient temp;

        if (!(file >> temp.id)) {
            break;
        }

        file.ignore(numeric_limits<streamsize>::max(), '\n');

        if (!file.getline(temp.name, 50)) {
            break;
        }

        if (!(file >> temp.age)) {
            break;
        }

        file.ignore(numeric_limits<streamsize>::max(), '\n');

        if (!file.getline(temp.disease, 50)) {
            break;
        }

        if (!(file >> temp.consultationFee
            >> temp.medicineCharges
            >> temp.otherCharges
            >> temp.bill)) {
            break;
        }

        file.ignore(numeric_limits<streamsize>::max(), '\n');

        if (temp.id <= 0 || temp.age <= 0 || temp.age > 120 ||
            temp.consultationFee < 0 || temp.medicineCharges < 0 ||
            temp.otherCharges < 0) {
            continue;
        }

        bool duplicate = false;

        for (int i = 0; i < totalPatients; i++) {
            if (patients[i].id == temp.id) {
                duplicate = true;
                break;
            }
        }

        if (!duplicate) {
            patients[totalPatients] = temp;
            totalPatients++;
        }
    }

    file.close();
}

void displayPatient(int index) {
    cout << "\n----------------------------------------\n";
    cout << "Patient ID          : " << patients[index].id << '\n';
    cout << "Name                : " << patients[index].name << '\n';
    cout << "Age                 : " << patients[index].age << '\n';
    cout << "Disease             : " << patients[index].disease << '\n';
    cout << "Consultation Fee    : " << patients[index].consultationFee << '\n';
    cout << "Medicine Charges    : " << patients[index].medicineCharges << '\n';
    cout << "Other Charges       : " << patients[index].otherCharges << '\n';
    cout << "Total Bill          : " << patients[index].bill << '\n';
    cout << "----------------------------------------\n";
}

void addPatient() {
    clearScreen();
    cout << "\n========== ADD NEW PATIENT ==========\n";

    if (totalPatients >= MAX_PATIENTS) {
        cout << "\nPatient storage is full.\n";
        pauseScreen();
        return;
    }

    int id = readInt("Enter Patient ID: ");

    if (id <= 0) {
        cout << "\nPatient ID must be positive.\n";
        pauseScreen();
        return;
    }

    if (findPatient(id) != -1) {
        cout << "\nThis patient ID already exists.\n";
        pauseScreen();
        return;
    }

    patients[totalPatients].id = id;
    readText("Enter Patient Name: ", patients[totalPatients].name, 50);

    int age;

    do {
        age = readInt("Enter Patient Age: ");

        if (age <= 0 || age > 120) {
            cout << "Please enter an age between 1 and 120.\n";
        }
    } while (age <= 0 || age > 120);

    patients[totalPatients].age = age;
    strcpy_s(patients[totalPatients].disease, "Not Diagnosed");
    patients[totalPatients].consultationFee = 0;
    patients[totalPatients].medicineCharges = 0;
    patients[totalPatients].otherCharges = 0;
    patients[totalPatients].bill = 0;

    totalPatients++;

    cout << "\nPatient registered successfully!\n";
    saveToFile();
    pauseScreen();
}

void editPatient() {
    clearScreen();
    cout << "\n========== EDIT PATIENT ==========\n";

    int id = readInt("Enter Patient ID: ");
    int index = findPatient(id);

    if (index == -1) {
        cout << "\nPatient not found.\n";
        pauseScreen();
        return;
    }

    cout << "\nCurrent patient details:\n";
    displayPatient(index);

    cout << "\nEnter updated details:\n";
    readText("Enter New Name: ", patients[index].name, 50);

    int age;

    do {
        age = readInt("Enter New Age: ");

        if (age <= 0 || age > 120) {
            cout << "Please enter an age between 1 and 120.\n";
        }
    } while (age <= 0 || age > 120);

    patients[index].age = age;

    cout << "\nPatient information updated successfully.\n";
    saveToFile();
    pauseScreen();
}

void deletePatient() {
    clearScreen();
    cout << "\n========== DELETE PATIENT ==========\n";

    int id = readInt("Enter Patient ID: ");
    int index = findPatient(id);

    if (index == -1) {
        cout << "\nPatient not found.\n";
        pauseScreen();
        return;
    }

    displayPatient(index);

    char confirm;
    cout << "\nAre you sure you want to delete this patient? (Y/N): ";
    cin >> confirm;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (confirm == 'Y' || confirm == 'y') {
        for (int i = index; i < totalPatients - 1; i++) {
            patients[i] = patients[i + 1];
        }

        totalPatients--;

        cout << "\nPatient deleted successfully.\n";
        saveToFile();
    }
    else {
        cout << "\nDeletion cancelled.\n";
    }

    pauseScreen();
}

void searchPatient() {
    clearScreen();
    cout << "\n========== SEARCH PATIENT ==========\n";

    int id = readInt("Enter Patient ID: ");
    int index = findPatient(id);

    if (index == -1) {
        cout << "\nPatient not found.\n";
    }
    else {
        cout << "\nPatient record found:\n";
        displayPatient(index);
    }

    pauseScreen();
}

void viewPatients() {
    clearScreen();
    cout << "\n========== ALL PATIENTS ==========\n";

    if (totalPatients == 0) {
        cout << "\nNo patient records available.\n";
        pauseScreen();
        return;
    }

    cout << "\nTotal registered patients: " << totalPatients << '\n';

    for (int i = 0; i < totalPatients; i++) {
        cout << "\nPatient " << i + 1;
        displayPatient(i);
    }

    pauseScreen();
}

void addDisease() {
    clearScreen();
    cout << "\n========== UPDATE PATIENT DISEASE ==========\n";

    int id = readInt("Enter Patient ID: ");
    int index = findPatient(id);

    if (index == -1) {
        cout << "\nPatient not found.\n";
        pauseScreen();
        return;
    }

    readText("Enter Disease/Diagnosis: ", patients[index].disease, 50);

    cout << "\nPatient diagnosis updated successfully.\n";
    saveToFile();
    pauseScreen();
}

void generateBill() {
    clearScreen();
    cout << "\n========== GENERATE PATIENT BILL ==========\n";

    int id = readInt("Enter Patient ID: ");
    int index = findPatient(id);

    if (index == -1) {
        cout << "\nPatient not found.\n";
        pauseScreen();
        return;
    }

    cout << "\nPatient: " << patients[index].name << '\n';

    int consultation = readInt("Enter Consultation Fee: ");
    int medicine = readInt("Enter Medicine Charges: ");
    int other = readInt("Enter Other Charges: ");

    if (consultation < 0 || medicine < 0 || other < 0) {
        cout << "\nCharges cannot be negative. Bill was not changed.\n";
        pauseScreen();
        return;
    }

    patients[index].consultationFee = consultation;
    patients[index].medicineCharges = medicine;
    patients[index].otherCharges = other;

    calculateBill(index);

    cout << "\n========== HOSPITAL BILL ==========\n";
    cout << "Patient ID          : " << patients[index].id << '\n';
    cout << "Patient Name        : " << patients[index].name << '\n';
    cout << "Disease             : " << patients[index].disease << '\n';
    cout << "Consultation Fee    : " << patients[index].consultationFee << '\n';
    cout << "Medicine Charges    : " << patients[index].medicineCharges << '\n';
    cout << "Other Charges       : " << patients[index].otherCharges << '\n';
    cout << "-----------------------------------\n";
    cout << "TOTAL BILL          : " << patients[index].bill << '\n';
    cout << "===================================\n";

    saveToFile();
    pauseScreen();
}

bool login(const char correctUsername[], const char correctPassword[]) {
    char username[30];
    char password[30];

    cout << "Username: ";
    cin >> setw(30) >> username;

    cout << "Password: ";
    cin >> setw(30) >> password;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (strcmp(username, correctUsername) == 0 &&
        strcmp(password, correctPassword) == 0) {
        cout << "\nLogin successful!\n";
        pauseScreen();
        return true;
    }

    cout << "\nIncorrect username or password.\n";
    pauseScreen();
    return false;
}

void adminMenu() {
    int choice;

    do {
        clearScreen();
        cout << "\n====================================\n";
        cout << "             ADMIN MENU             \n";
        cout << "====================================\n";
        cout << "1. Add Patient\n";
        cout << "2. Edit Patient\n";
        cout << "3. Delete Patient\n";
        cout << "4. Search Patient\n";
        cout << "5. View All Patients\n";
        cout << "6. Add/Update Disease\n";
        cout << "7. Generate Bill\n";
        cout << "8. Logout\n";

        choice = readInt("\nEnter your choice: ");

        switch (choice) {
        case 1: addPatient(); break;
        case 2: editPatient(); break;
        case 3: deletePatient(); break;
        case 4: searchPatient(); break;
        case 5: viewPatients(); break;
        case 6: addDisease(); break;
        case 7: generateBill(); break;
        case 8: cout << "\nLogging out...\n"; break;
        default:
            cout << "\nInvalid choice.\n";
            pauseScreen();
        }
    } while (choice != 8);
}

void doctorMenu() {
    int choice;

    do {
        clearScreen();
        cout << "\n====================================\n";
        cout << "             DOCTOR MENU            \n";
        cout << "====================================\n";
        cout << "1. Search Patient\n";
        cout << "2. View All Patients\n";
        cout << "3. Add/Update Disease\n";
        cout << "4. Logout\n";

        choice = readInt("\nEnter your choice: ");

        switch (choice) {
        case 1: searchPatient(); break;
        case 2: viewPatients(); break;
        case 3: addDisease(); break;
        case 4: cout << "\nLogging out...\n"; break;
        default:
            cout << "\nInvalid choice.\n";
            pauseScreen();
        }
    } while (choice != 4);
}

void receptionistMenu() {
    int choice;

    do {
        clearScreen();
        cout << "\n====================================\n";
        cout << "          RECEPTIONIST MENU         \n";
        cout << "====================================\n";
        cout << "1. Register Patient\n";
        cout << "2. Edit Patient\n";
        cout << "3. Delete Patient\n";
        cout << "4. Search Patient\n";
        cout << "5. View All Patients\n";
        cout << "6. Generate Bill\n";
        cout << "7. Logout\n";

        choice = readInt("\nEnter your choice: ");

        switch (choice) {
        case 1: addPatient(); break;
        case 2: editPatient(); break;
        case 3: deletePatient(); break;
        case 4: searchPatient(); break;
        case 5: viewPatients(); break;
        case 6: generateBill(); break;
        case 7: cout << "\nLogging out...\n"; break;
        default:
            cout << "\nInvalid choice.\n";
            pauseScreen();
        }
    } while (choice != 7);
}

int main() {
    loadFromFile();

    int choice;

    do {
        clearScreen();

        cout << "\n============================================\n";
        cout << "          HOSPITAL MANAGEMENT SYSTEM        \n";
        cout << "============================================\n";
        cout << "             Welcome to HMS                 \n";
        cout << "--------------------------------------------\n";
        cout << "1. Admin Login\n";
        cout << "2. Doctor Login\n";
        cout << "3. Receptionist Login\n";
        cout << "4. Exit\n";
        cout << "--------------------------------------------\n";

        choice = readInt("Enter your choice: ");

        switch (choice) {
        case 1:
            clearScreen();
            cout << "\n========== ADMIN LOGIN ==========\n";

            if (login("admin", "123")) {
                adminMenu();
            }
            break;

        case 2:
            clearScreen();
            cout << "\n========== DOCTOR LOGIN ==========\n";

            if (login("doctor", "123")) {
                doctorMenu();
            }
            break;

        case 3:
            clearScreen();
            cout << "\n========== RECEPTIONIST LOGIN ==========\n";

            if (login("reception", "123")) {
                receptionistMenu();
            }
            break;

        case 4:
            saveToFile();
            cout << "\nThank you for using Hospital Management System!\n";
            break;

        default:
            cout << "\nInvalid choice. Please select 1 to 4.\n";
            pauseScreen();
        }

    } while (choice != 4);

    return 0;
}