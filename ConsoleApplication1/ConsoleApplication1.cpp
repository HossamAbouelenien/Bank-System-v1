#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <iomanip>
#include <fstream>

using namespace std;

void showMainMenu();

const string fileName = "myFile.txt";

struct sClient {
    string accountNumber;
    string pinCode;
    string Name;
    string Phone;
    double accountBalance;
    bool markToDelet = false;
};

enum enMainMenuOptions {
    eShowClientList = 1,
    eAddClient = 2,
    eDeletClient = 3,
    eUpdateClient = 4,
    eFindClient = 5,
    eExit = 6,
};

vector<string> splitFunc(string s1, string delimiter = "#//#") {
    int pos = 0;
    string word = "";
    vector<string>vWords;
    while ((pos = s1.find(delimiter)) != string::npos) {
        word = s1.substr(0, pos);
        if (word != "") {
            vWords.push_back(word);
        }
        s1.erase(0, pos + delimiter.length());
    }
    if (s1 != "") {
        vWords.push_back(s1);
    }
    return vWords;
}

sClient convertLineToRecord(string line , string sperator = "#//#") {
    sClient client;
    vector<string>vClient;
    vClient = splitFunc(line, sperator);
    client.accountNumber = vClient[0];
    client.pinCode = vClient[1];
    client.Name = vClient[2];
    client.Phone = vClient[3];
    client.accountBalance = stod(vClient[4]);
    return client;
}

string convertRecordToLine(sClient client, string delimiter = "#//#") {
    string line = "";
    line += client.accountNumber + delimiter;
    line += client.pinCode + delimiter;
    line += client.Name + delimiter;
    line += client.Phone + delimiter;
    line += to_string(client.accountBalance) ;
    return line;
}

vector<sClient> loadDataFromFile(string fileName ) {
    sClient client;
    fstream file;
    vector<sClient>vClients;
    file.open(fileName, ios::in);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            client = convertLineToRecord(line);
            vClients.push_back(client);
        }
        file.close();
    }
    return vClients;
}

void printClient(sClient client) {
    cout << "| " << left << setw(20) << client.accountNumber;
    cout << "| " << left << setw(20) << client.pinCode;
    cout << "| " << left << setw(20) << client.Name;
    cout << "| " << left << setw(30) << client.Phone;
    cout << "| " << left << setw(20) << client.accountBalance;
}

void printClientInfo(sClient client) {
    cout << "Account Number  : " << client.accountNumber << endl;
    cout << "Pin Code        : " << client.pinCode << endl;
    cout << "Account Name    : " << client.Name << endl;
    cout << "Account Phone   : " << client.Phone << endl;
    cout << "Account Balance : " << client.accountBalance << endl;
}

void showClients() {
    vector<sClient>vClients = loadDataFromFile(fileName);
    for (sClient& c : vClients) {
        printClient(c);
        cout << "------------------------------------------------------------------------------------------------------------------------" << endl;

    }
}

string readAccountNumber() {
    string accountNumber = "";
    cout << "Enter The Account Number :  ";
    cin >> accountNumber;
    return accountNumber;
}

void showClientList() {
    cout << "==================================================" << endl;
    cout << "\t\tClients List Screen" << endl;
    cout << "==================================================" << endl;
    cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "| " << left << setw(20) << "Account Number";
    cout << "| " << left << setw(20) << "Pin Code";
    cout << "| " << left << setw(20) << "Account Name";
    cout << "| " << left << setw(30) << "Phone";
    cout << "| " << left << setw(20) << "Account Balance";
    cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
    showClients();

}

bool checkIfTheClientFound(string fileName, string accountNumber) {
    vector<sClient>vClints = loadDataFromFile(fileName);
    for (sClient& c : vClints) {
        if (c.accountNumber == accountNumber) {          
            return true;
        }
    }
    return false;
}

bool checkIfTheClientFound(string fileName, string accountNumber , sClient &client) {
    vector<sClient>vClints = loadDataFromFile(fileName);
    for (sClient& c : vClints) {
        if (c.accountNumber == accountNumber) {
            client = c;
            return true;
        }
    }
    return false;
}

sClient readNewClient() {
    sClient client;
    cout << "Enter Account Number   :  ";
    cin >> client.accountNumber;
    while (checkIfTheClientFound(fileName, client.accountNumber)) {
        cout << "The Client is Already Exit, please Enter Another Account Number ? ";
        cin >> client.accountNumber;
    }
    cout << "Enter Pin Code         :  ";
    cin >> client.pinCode;
    cout << "Enter Name             :  ";
    cin >> client.Name;
    cout << "Enter Phone            :  ";
    cin >> client.Phone;
    cout << "Enter Account Balance  :  ";
    cin >> client.accountBalance;
    return client;
}

sClient readUpdatedClient(string accountNumber) {
    sClient client;
    client.accountNumber = accountNumber;
    cout << "Enter Pin Code         :  ";
    cin >> client.pinCode;
    cout << "Enter Name             :  ";
    cin >> client.Name;
    cout << "Enter Phone            :  ";
    cin >> client.Phone;
    cout << "Enter Account Balance  :  ";
    cin >> client.accountBalance;
    return client;
}

void addClientToFile(string fileName , sClient client) {
    string line;
    fstream file;
    file.open(fileName, ios::out | ios::app);
    if (file.is_open()) {
        line = convertRecordToLine(client);
        file << line << endl;
        file.close();
    }
    
}

void addNewClient() {
    sClient client;
    client = readNewClient();
    addClientToFile(fileName, client);
}

void addNewClients() {
    cout << "\nAdding New Clients : \n\n";
    char addmore = 'n';
    do {
        addNewClient();
        cout << "Client Added Successfully , do you want to add more Clients ? y/n ";
        cin >> addmore;
    } while (addmore == 'y' || addmore == 'Y');
}

void showAddClient() {
    cout << "==================================================" << endl;
    cout << "\t\tAdd Clients Screen" << endl;
    cout << "==================================================" << endl;
    addNewClients();
}

void markTheClientToDeleted(vector<sClient>& vClients, string accountNumber) {
    for (sClient& c : vClients) {
        if (c.accountNumber == accountNumber) {
            c.markToDelet = true;
            break;
        }
    }
}

void saveChangesToFile(vector<sClient>vClients , string fileName) {
    string line;
    fstream file;
    file.open(fileName, ios::out);
    if (file.is_open()) {
        for (sClient& c : vClients) {
            if (c.markToDelet == false) {
                line = convertRecordToLine(c);
                file << line << endl;
            }
        }
        file.close();
    }
}

void deleteClient() {
    vector<sClient>vClients = loadDataFromFile(fileName);
    string accountNumber = readAccountNumber();
    sClient client;
    char answer = 'n';
    if (checkIfTheClientFound(fileName , accountNumber , client)) {
        printClientInfo(client);
        cout << "are you sure , you want to delet this client ? y/n  ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y') {
        markTheClientToDeleted(vClients, accountNumber);
        saveChangesToFile(vClients,fileName);
        vClients = loadDataFromFile(fileName);
        cout << "\n\nThe Client has been deleted Successfully !" << endl;
        }
    }
    else {
        cout << "The Client (" << accountNumber <<") is Not Exit"<<endl;
    
    }
}

void DeletClients() {
    cout << "\nDeleting the client : \n" << endl;
    char addmore = 'n';
    do {

        deleteClient();
        cout << "\n\nDo you want to delete more y/n ? ";
        cin >> addmore;
    } while (addmore == 'y' || addmore == 'Y');
}

void showDeletClients() {
    cout << "==================================================" << endl;
    cout << "\t\Delete Clients Screen" << endl;
    cout << "==================================================" << endl;
    DeletClients();
}

void updateClient() {
    vector<sClient>vClients = loadDataFromFile(fileName);
    sClient client;
    char answer = 'n';
    string accountNumber = readAccountNumber();
    if (checkIfTheClientFound(fileName,accountNumber,client)) {
        printClientInfo(client);
        cout << "\nAre you sure to update this client ? y/n ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y') {
            for (sClient& c : vClients) {
                if (c.accountNumber == accountNumber) {
                    c = readUpdatedClient(accountNumber);
                    saveChangesToFile(vClients, fileName);
                    cout << "\n\nThe Client has been Updated Successfully !" << endl;
                    break;
                }
            }
        }
    }
    else {
        cout << "This client(" << accountNumber << ") is Not exit " << endl;
    }
}

void UpdateClients() {
    cout << "\nUpdating the client : \n" << endl;
    char answer = 'n';
    do {
        updateClient();

        cout << "\n\nDo you want to update more Clients ? y/n  ";
        cin >> answer;
    } while (answer == 'y' || answer == 'Y');
}

void showUpdateClients() {
    cout << "==================================================" << endl;
    cout << "\t\tUpdate Clients Screen" << endl;
    cout << "==================================================" << endl;
    UpdateClients();
}

void findClient() {
    sClient client;
    string accountNumber = readAccountNumber();
    if (checkIfTheClientFound(fileName, accountNumber, client)) {
        cout << "The following is Client's details : \n";
        printClientInfo(client);
    }
    else {
        cout << "This CLient("<<accountNumber<< ") is Not exit. " << endl;
    }
}

void findClients() {
    char answer = 'n';
    do {
        findClient();
        cout << "Do You Want to search about more Clients ?  y/n ";
        cin >> answer;
    } while (answer == 'y' || answer == 'Y');
}

void showFindClients() {
    cout << "==================================================" << endl;
    cout << "\t\tFind Clients Screen" << endl;
    cout << "==================================================" << endl;
    findClients();
}

void showExit() {
    cout << "==================================================" << endl;
    cout << "\t\tProgram Ends :)" << endl;
    cout << "==================================================" << endl;
}

void backToMainMenu() {
    cout << "\n\nPress any key to go to main menu .....  ";
    system("pause > 0");
    showMainMenu();
}

void performMainOptions(enMainMenuOptions mainMenuOption) {
    switch (mainMenuOption) {
    case (enMainMenuOptions::eShowClientList):
        {
            system("cls");
            showClientList();
            backToMainMenu();
            break;
        }
    case(enMainMenuOptions::eAddClient):
        {
            system("cls");
            showAddClient();
            backToMainMenu();
            break;
        }
    case(enMainMenuOptions::eDeletClient):
        {
            system("cls");
            showDeletClients();
            backToMainMenu();
            break;
        }
    case(enMainMenuOptions::eUpdateClient):
        {
            system("cls");
            showUpdateClients();
            backToMainMenu();
            break;
        }
    case(enMainMenuOptions::eFindClient):
        {
            system("cls");
            showFindClients();
            backToMainMenu();
            break;
        }
    case(enMainMenuOptions::eExit):
        {
            system("cls");
            showExit();
            break;
        }


    }
}

short readMainOption() {
    cout << "Choose , What do you want to do [ 1 to 6 ] ? ";
    short choice = 0;
    cin >> choice;
    return choice;
}
void showMainMenu() {
    system("cls");
    cout << "==================================================" << endl;
    cout << "\t\tMain Menu Screen." << endl;
    cout << "==================================================" << endl;
    cout << "\n\t[1] Show Client List." << endl;
    cout << "\t[2] Add New Client." << endl;
    cout << "\t[3] Delet Client." << endl;
    cout << "\t[4] Update Client." << endl;
    cout << "\t[5] Find Client." << endl;
    cout << "\t[6] Exit.\n" << endl;
    performMainOptions((enMainMenuOptions)readMainOption());

}

int main()
{
    showMainMenu();
    system("pause > 0");
    return 0;
}

