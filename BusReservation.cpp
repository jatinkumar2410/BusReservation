// Brought to you by code-projects.org
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <conio.h> // For getch()
#include <algorithm>

using namespace std;

// Constants
const int MAX_BUSES = 6;
const int MAX_SEATS = 32;
const int TICKET_PRICE = 200;

// Global Variables
string busNames[MAX_BUSES] = {
    "Cardiff Express",
    "Belfast Express",
    "Derby Express",
    "Chester Express",
    "Newport Express",
    "Truro Express"
};

// Function Prototypes
void login();
void displayMenu();
void viewBusList();
void bookTickets();
void cancelBooking();
void viewStatus();
int readNumber(int busNo, vector<int>& seatNumbers);
void readNames(int busNo, vector<string>& passengerNames);
void updateSeatFile(int busNo, int availableSeats);
void updateStatusFiles(int busNo, const vector<int>& seatNumbers, const vector<string>& passengerNames);
void clearInputBuffer();

int main()
{
    login();
    int choice;

    do {
        system("cls");
        displayMenu();
        cout << "\t\t\tEnter Your Choice:: ";
        cin >> choice;

        switch(choice)
        {
            case 1:
                viewBusList();
                break;
            case 2:
                bookTickets();
                break;
            case 3:
                cancelBooking();
                break;
            case 4:
                viewStatus();
                break;
            case 5:
                cout << "\n\n\t----------------------------------------------------------------------------------------------------------\n";
                cout << "\t\t\t\t\tThank You For Using This System\t\t\t\t\t\t\n";
                cout << "\t----------------------------------------------------------------------------------------------------------\n";
                cout << "\t\t\t Brought To You By code-projects.org\n";
                system("pause");
                break;
            default:
                cout << "Invalid choice! Please try again.";
                system("pause");
        }
    } while(choice != 5);

    return 0;
}

void displayMenu()
{
    cout << "\n\n\n";
    cout << "====================================== WELCOME TO BUS RESERVATION SYSTEM ======================================\n\n\n";
    cout << "\t\t\t\t\t[1]=> View Bus List\n\n";
    cout << "\t\t\t\t\t[2]=> Book Tickets\n\n";
    cout << "\t\t\t\t\t[3]=> Cancel Booking\n\n";
    cout << "\t\t\t\t\t[4]=> Bus Status Board\n\n";
    cout << "\t\t\t\t\t[5]=> Exit\n\n";
    cout << "===============================================================================================================\n\n";
}

void viewBusList()
{
    system("cls");
    cout << "\n\n\n";
    cout << "=========================================== BUS RESERVATION SYSTEM ============================================\n\n\n";
    for(int i = 0; i < MAX_BUSES; ++i)
    {
        cout << "\t\t\t\t\t[" << (i+1) << "]  =>  " << busNames[i] << "\n\n";
    }
    system("pause");
}

void bookTickets()
{
    system("cls");
    cout << "=========================================== BUS RESERVATION SYSTEM ============================================\n\n\n";
    viewBusList();
    
    int busNo;
    cout << "Enter the Bus number:---> ";
    cin >> busNo;

    if(busNo < 1 || busNo > MAX_BUSES)
    {
        cout << "Invalid Bus Number!";
        system("pause");
        return;
    }

    system("cls");
    cout << "=========================================== BUS RESERVATION SYSTEM ============================================\n\n\n";
    cout << "Your Bus Number is " << busNo << " ********** " << busNames[busNo - 1] << "\n";

    // Read available seats
    string seatFile = "tr" + to_string(busNo) + ".txt";
    ifstream seatInput(seatFile);
    int availableSeats = 0;
    if(seatInput.is_open())
    {
        seatInput >> availableSeats;
        seatInput.close();
    }
    else
    {
        cout << "Error opening seat file!";
        system("pause");
        return;
    }

    if(availableSeats <= 0)
    {
        cout << "There are no available seats in this bus.\n";
        system("pause");
        return;
    }

    cout << "\n\n\n\t\t\t\tAvailable Seats:------> " << availableSeats << "\n";
    int booking;
    cout << "\n\t\t\t\tNumber of Tickets:-----> ";
    cin >> booking;

    if(booking <= 0 || booking > availableSeats)
    {
        cout << "Invalid number of tickets!";
        system("pause");
        return;
    }

    availableSeats -= booking;
    updateSeatFile(busNo, availableSeats);

    // Collect passenger details
    vector<int> seatNumbers;
    vector<string> passengerNames;
    cout << "\n";
    for(int i = 0; i < booking; ++i)
    {
        cout << "============================ Enter the details for ticket no " << (i+1) << " ============================\n\n\n";
        int seatNo;
        string name;
        cout << "\t\t\t\tEnter the seat number:---> ";
        cin >> seatNo;
        if(seatNo < 1 || seatNo > MAX_SEATS)
        {
            cout << "\t\t\t\tInvalid seat number. Please try booking again.\n";
            --i; // Retry this iteration
            continue;
        }
        cout << "\t\t\t\tEnter the name of person:---> ";
        cin >> ws; // Clear any leading whitespace
        getline(cin, name);
        passengerNames.push_back(name);
        seatNumbers.push_back(seatNo);
        cout << "\n======================================================================================================\n\n\n";
    }

    // Update status files
    updateStatusFiles(busNo, seatNumbers, passengerNames);

    cout << "\n\t\t\t\tThe Total booking amount is " << (TICKET_PRICE * booking) << " rupees.\n";
    system("pause");
}

void cancelBooking()
{
    system("cls");
    cout << "=========================================== BUS RESERVATION SYSTEM ============================================\n\n\n";
    viewBusList();
    
    int busNo;
    cout << "Enter the Bus number:----> ";
    cin >> busNo;

    if(busNo < 1 || busNo > MAX_BUSES)
    {
        cout << "Invalid Bus Number!";
        system("pause");
        return;
    }

    // Read current bookings
    vector<int> seatNumbers;
    vector<string> passengerNames;
    readNumber(busNo, seatNumbers);
    readNames(busNo, passengerNames);

    // Display current status
    cout << "\n____________________________________________________________________________________________________________________\n";
    cout << "                                Bus.no--> " << busNo << " ----> " << busNames[busNo - 1] << "                                                                 \n";
    cout << "____________________________________________________________________________________________________________________\n";

    string tempname[MAX_SEATS + 1];
    fill_n(tempname, MAX_SEATS + 1, "Empty ");

    for(size_t i = 0; i < seatNumbers.size(); ++i)
    {
        if(seatNumbers[i] >=1 && seatNumbers[i] <= MAX_SEATS)
            tempname[seatNumbers[i]] = passengerNames[i];
    }

    int index = 1;
    for(int i = 0; i < 8; ++i)
    {
        cout << "\t\t\t\t";
        for(int j = 0; j < 4; ++j)
        {
            if(index > MAX_SEATS) break;
            cout << index << "." << tempname[index] << "\t";
            index++;
        }
        cout << "\n";
    }

    // Prompt for seat to cancel
    int seat_no;
    cout << "\nEnter the seat number to cancel:---> ";
    cin >> seat_no;

    if(seat_no < 1 || seat_no > MAX_SEATS)
    {
        cout << "Invalid seat number!";
        system("pause");
        return;
    }

    // Find and remove booking
    bool found = false;
    for(size_t i = 0; i < seatNumbers.size(); ++i)
    {
        if(seatNumbers[i] == seat_no)
        {
            seatNumbers.erase(seatNumbers.begin() + i);
            passengerNames.erase(passengerNames.begin() + i);
            found = true;
            break;
        }
    }

    if(found)
    {
        // Update status files
        updateStatusFiles(busNo, seatNumbers, passengerNames);

        // Update available seats
        string seatFile = "tr" + to_string(busNo) + ".txt";
        ifstream seatInput(seatFile);
        int availableSeats = 0;
        if(seatInput.is_open())
        {
            seatInput >> availableSeats;
            seatInput.close();
        }
        availableSeats += 1; // Increment available seats by 1
        updateSeatFile(busNo, availableSeats);

        cout << "\n\n======================================================================================================\n";
        cout << "\t\t\t\tYour " << TICKET_PRICE << " rupees has been returned.\n";
        cout << "======================================================================================================\n";
    }
    else
    {
        cout << "Seat number not found or already empty.";
    }
    system("pause");
}

void viewStatus()
{
    system("cls");
    cout << "=========================================== BUS RESERVATION SYSTEM ============================================\n\n\n";
    
    int busNo;
    cout << "Enter the number of bus:----> ";
    cin >> busNo;

    if(busNo < 1 || busNo > MAX_BUSES)
    {
        cout << "Invalid Bus Number!";
        system("pause");
        return;
    }

    // Read current bookings
    vector<int> seatNumbers;
    vector<string> passengerNames;
    readNumber(busNo, seatNumbers);
    readNames(busNo, passengerNames);

    cout << "____________________________________________________________________________________________________________________\n";
    cout << "                                Bus.no--> " << busNo << " ----> " << busNames[busNo - 1] << "                                                                 \n";
    cout << "____________________________________________________________________________________________________________________\n";

    string tempname[MAX_SEATS + 1];
    fill_n(tempname, MAX_SEATS + 1, "Empty ");

    for(size_t i = 0; i < seatNumbers.size(); ++i)
    {
        if(seatNumbers[i] >=1 && seatNumbers[i] <= MAX_SEATS)
            tempname[seatNumbers[i]] = passengerNames[i];
    }

    int index = 1;
    for(int i = 0; i < 8; ++i)
    {
        cout << "\t\t\t\t";
        for(int j = 0; j < 4; ++j)
        {
            if(index > MAX_SEATS) break;
            cout << index << "." << tempname[index] << "\t";
            index++;
        }
        cout << "\n";
    }

    system("pause");
}

void login()
{
    int attempts = 0;
    const int MAX_ATTEMPTS = 3;
    string uname, pword;

    while(attempts < MAX_ATTEMPTS)
    {
        system("cls");
        cout << "\n  =========================  LOGIN FORM  ==========================  \n";
        cout << " \n                         ENTER USERNAME:- ";
        cin >> uname;
        cout << " \n                         ENTER PASSWORD:- ";

        // Hide password input
        pword = "";
        char ch;
        while((ch = _getch()) != 13) // 13 is ASCII for Enter
        {
            if(ch == 8) // Handle backspace
            {
                if(!pword.empty())
                {
                    pword.pop_back();
                    cout << "\b \b";
                }
            }
            else
            {
                pword += ch;
                cout << '*';
            }
        }
        cout << "\n";

        if(uname == "user" && pword == "pass")
        {
            cout << "  \n\n\n         WELCOME TO OUR SYSTEM !!!! LOGIN IS SUCCESSFUL";
            cout << "\n\n\n\t\t\t\tPress any key to continue...";
            _getch();
            return;
        }
        else
        {
            cout << "\n       SORRY !!!!  LOGIN IS UNSUCCESSFUL";
            attempts++;
            if(attempts >= MAX_ATTEMPTS)
            {
                cout << "\nSorry you have entered the wrong username and password for three times!!!";
                system("pause");
                exit(0);
            }
            cout << "\nPlease try again.\n";
            system("pause");
        }
    }
}

int readNumber(int busNo, vector<int>& seatNumbers)
{
    string numberFile = "number" + to_string(busNo) + ".txt";
    ifstream inFile(numberFile);
    if(!inFile.is_open())
    {
        // If file doesn't exist, create it
        ofstream outFile(numberFile);
        outFile.close();
        return 0;
    }

    int num;
    while(inFile >> num)
    {
        seatNumbers.push_back(num);
    }
    inFile.close();
    return seatNumbers.size();
}

void readNames(int busNo, vector<string>& passengerNames)
{
    string statusFile = "status" + to_string(busNo) + ".txt";
    ifstream inFile(statusFile);
    if(!inFile.is_open())
    {
        // If file doesn't exist, create it
        ofstream outFile(statusFile);
        outFile.close();
        return;
    }

    string name;
    while(inFile >> ws && getline(inFile, name, ' '))
    {
        passengerNames.push_back(name);
    }
    inFile.close();
}

void updateSeatFile(int busNo, int availableSeats)
{
    string seatFile = "tr" + to_string(busNo) + ".txt";
    ofstream outFile(seatFile, ios::trunc);
    if(outFile.is_open())
    {
        outFile << availableSeats;
        outFile.close();
    }
    else
    {
        cout << "Error updating seat file!";
    }
}

void updateStatusFiles(int busNo, const vector<int>& seatNumbers, const vector<string>& passengerNames)
{
    // Update number file
    string numberFile = "number" + to_string(busNo) + ".txt";
    ofstream numOut(numberFile, ios::trunc);
    if(numOut.is_open())
    {
        for(auto num : seatNumbers)
        {
            numOut << num << " ";
        }
        numOut.close();
    }
    else
    {
        cout << "Error updating number file!";
    }

    // Update status file
    string statusFile = "status" + to_string(busNo) + ".txt";
    ofstream statusOut(statusFile, ios::trunc);
    if(statusOut.is_open())
    {
        for(auto name : passengerNames)
        {
            statusOut << name << " ";
        }
        statusOut.close();
    }
    else
    {
        cout << "Error updating status file!";
    }
}

void clearInputBuffer()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
