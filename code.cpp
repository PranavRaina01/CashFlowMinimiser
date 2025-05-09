#include <bits/stdc++.h>
using namespace std;

#include <string>
#include <set>
#include <climits>
#include <cctype>
#include <iostream>
#include <iomanip>
#include <limits>

// Constants for better maintainability 
const int MAX_BANKS = 100;
const int MAX_TYPES = 10;

// Common payment modes
const vector<string> COMMON_PAYMENT_MODES = {
    "Google_Pay", "PayTM", "PhonePe", "UPI", "NEFT",
    "RTGS", "IMPS", "Bank_Transfer", "Cash", "Cheque"
};

// UI Constants
const string BORDER = "+------------------------------------------------------------------------+";
const string EMPTY_BORDER = "|                                                                        |";
const string BOTTOM_BORDER = "+------------------------------------------------------------------------+";

class Bank {
public:
    string name;
    int netAmount;
    set<string> types;
    
    // Constructor
    Bank() : netAmount(0) {}
};

// UI Helper Functions
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void waitForEnter() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void printHeader(const string& title) {
    cout << "\n" << BORDER << "\n";
    cout << "|" << setw(40 + title.length()/2) << title << setw(40 - title.length()/2) << "|\n";
    cout << BOTTOM_BORDER << "\n\n";
}

void printSection(const string& title) {
    cout << "\n" << string(80, '-') << "\n";
    cout << " " << title << "\n";
    cout << string(80, '-') << "\n";
}

void printError(const string& message) {
    cout << "\n[ERROR] " << message << "\n";
    waitForEnter();
}

void printSuccess(const string& message) {
    cout << "\n[SUCCESS] " << message << "\n";
}

// Input validation functions
bool validateBankName(const string& name) {
    if(name.empty() || name.length() > 50) {
        printError("Bank name must be between 1 and 50 characters long.");
        return false;
    }
    for(char c : name) {
        if(!isalnum(c) && c != '_') {
            printError("Bank name can only contain letters, numbers, and underscores.");
            return false;
        }
    }
    return true;
}

bool validateAmount(int amount) {
    if(amount <= 0 || amount > 1000000) {
        printError("Amount must be between 1 and 1,000,000.");
        return false;
    }
    return true;
}

// Input helper functions
int getIntInput(const string& prompt, int min, int max) {
    int value;
    while(true) {
        cout << prompt;
        if(cin >> value) {
            clearInputBuffer();
            if(value >= min && value <= max) {
                return value;
            }
        } else {
            cin.clear();
            clearInputBuffer();
        }
        printError("Please enter a number between " + to_string(min) + " and " + to_string(max));
    }
}

string getBankName(const string& prompt, const unordered_map<string, int>& existingBanks) {
    while(true) {
        cout << prompt;
        string name;
        cin >> name;
        clearInputBuffer();
        
        if(validateBankName(name)) {
            return name;
        }
    }
}

set<string> getPaymentModes(int numModes) {
    set<string> modes;
    cout << "\nAvailable payment modes:\n";
    for(size_t i = 0; i < COMMON_PAYMENT_MODES.size(); i++) {
        cout << "[" << (i+1) << "] " << COMMON_PAYMENT_MODES[i] << "\n";
    }
    cout << "[0] Enter custom payment mode\n\n";
    
    while(modes.size() < numModes) {
        int choice = getIntInput("Select payment mode " + to_string(modes.size() + 1) + 
                               " (0-" + to_string(COMMON_PAYMENT_MODES.size()) + "): ", 
                               0, COMMON_PAYMENT_MODES.size());
        
        if(choice == 0) {
            cout << "Enter custom payment mode: ";
            string mode;
            cin >> mode;
            clearInputBuffer();
            if(validateBankName(mode)) {
                modes.insert(mode);
            }
        } else {
            modes.insert(COMMON_PAYMENT_MODES[choice - 1]);
        }
    }
    return modes;
}

// Helper functions for finding min/max indices
int getMinIndex(const Bank listOfNetAmounts[], int numBanks) {
    int min = INT_MAX, minIndex = -1;
    for(int i = 0; i < numBanks; i++) {
        if(listOfNetAmounts[i].netAmount == 0) continue;
        if(listOfNetAmounts[i].netAmount < min) {
            minIndex = i;
            min = listOfNetAmounts[i].netAmount;
        }
    }
    return minIndex;
}

int getSimpleMaxIndex(const Bank listOfNetAmounts[], int numBanks) {
    int max = INT_MIN, maxIndex = -1;
    for(int i = 0; i < numBanks; i++) {
        if(listOfNetAmounts[i].netAmount == 0) continue;
        if(listOfNetAmounts[i].netAmount > max) {
            maxIndex = i;
            max = listOfNetAmounts[i].netAmount;
        }
    }
    return maxIndex;
}

pair<int, string> getMaxIndex(const Bank listOfNetAmounts[], int numBanks, int minIndex, const Bank input[], int maxNumTypes) {
    int max = INT_MIN;
    int maxIndex = -1;
    string matchingType;
    
    for(int i = 0; i < numBanks; i++) {
        if(listOfNetAmounts[i].netAmount <= 0) continue;
        
        vector<string> v(maxNumTypes);
        vector<string>::iterator ls = set_intersection(
            listOfNetAmounts[minIndex].types.begin(),
            listOfNetAmounts[minIndex].types.end(),
            listOfNetAmounts[i].types.begin(),
            listOfNetAmounts[i].types.end(),
            v.begin()
        );
        
        if((ls - v.begin()) != 0 && max < listOfNetAmounts[i].netAmount) {
            max = listOfNetAmounts[i].netAmount;
            maxIndex = i;
            matchingType = *(v.begin());
        }
    }
    
    return make_pair(maxIndex, matchingType);
}

void printTransaction(const string& from, const string& to, int amount, const string& type) {
    cout << "| " << setw(20) << left << from 
         << " pays Rs " << setw(8) << right << amount 
         << " to " << setw(20) << left << to 
         << " via " << type << endl;
}

void printAns(vector<vector<pair<int, string>>>& ansGraph, int numBanks, const Bank input[]) {
    printHeader("MINIMIZED CASH FLOW TRANSACTIONS");
    
    for(int i = 0; i < numBanks; i++) {
        for(int j = 0; j < numBanks; j++) {
            if(i == j) continue;
            
            if(ansGraph[i][j].first != 0 && ansGraph[j][i].first != 0) {
                if(ansGraph[i][j].first == ansGraph[j][i].first) {
                    ansGraph[i][j].first = 0;
                    ansGraph[j][i].first = 0;
                }
                else if(ansGraph[i][j].first > ansGraph[j][i].first) {
                    ansGraph[i][j].first -= ansGraph[j][i].first;
                    ansGraph[j][i].first = 0;
                    printTransaction(input[i].name, input[j].name, ansGraph[i][j].first, ansGraph[i][j].second);
                }
                else {
                    ansGraph[j][i].first -= ansGraph[i][j].first;
                    ansGraph[i][j].first = 0;
                    printTransaction(input[j].name, input[i].name, ansGraph[j][i].first, ansGraph[j][i].second);
                }
            }
            else if(ansGraph[i][j].first != 0) {
                printTransaction(input[i].name, input[j].name, ansGraph[i][j].first, ansGraph[i][j].second);
            }
            else if(ansGraph[j][i].first != 0) {
                printTransaction(input[j].name, input[i].name, ansGraph[j][i].first, ansGraph[j][i].second);
            }
            
            ansGraph[i][j].first = 0;
            ansGraph[j][i].first = 0;
        }
    }
    cout << "\n";
}

void minimizeCashFlow(int numBanks, Bank input[], unordered_map<string, int>& indexOf, int numTransactions, vector<vector<int>>& graph, int maxNumTypes) {
    
    // Find net amount of each bank has
    Bank listOfNetAmounts[numBanks];
    
    for(int b = 0; b < numBanks; b++) {
        listOfNetAmounts[b].name = input[b].name;
        listOfNetAmounts[b].types = input[b].types;
        
        int amount = 0;
        // incoming edges
        // column travers
        for(int i = 0; i < numBanks; i++) {
            amount += (graph[i][b]);
        }
        
        // outgoing edges
        // row traverse
        for(int j = 0; j < numBanks; j++) {
            amount += ((-1) * graph[b][j]);
        }
        
        listOfNetAmounts[b].netAmount = amount;
    }
    
    vector<vector<pair<int, string>>> ansGraph(numBanks, vector<pair<int, string>>(numBanks, {0, ""})); // adjacency matrix
    
    
    // find min and max net amount
    int numZeroNetAmounts = 0;
    
    for(int i = 0; i < numBanks; i++) {
        if(listOfNetAmounts[i].netAmount == 0) numZeroNetAmounts++;
    }
    while(numZeroNetAmounts != numBanks) {
        
        int minIndex = getMinIndex(listOfNetAmounts, numBanks);
        pair<int, string> maxAns = getMaxIndex(listOfNetAmounts, numBanks, minIndex, input, maxNumTypes);
        
        int maxIndex = maxAns.first;
        
        if(maxIndex == -1) {
            
            (ansGraph[minIndex][0].first) += abs(listOfNetAmounts[minIndex].netAmount);
            (ansGraph[minIndex][0].second) = *(input[minIndex].types.begin());
            
            int simpleMaxIndex = getSimpleMaxIndex(listOfNetAmounts, numBanks);
            (ansGraph[0][simpleMaxIndex].first) += abs(listOfNetAmounts[minIndex].netAmount);
            (ansGraph[0][simpleMaxIndex].second) = *(input[simpleMaxIndex].types.begin());
            
            listOfNetAmounts[simpleMaxIndex].netAmount += listOfNetAmounts[minIndex].netAmount;
            listOfNetAmounts[minIndex].netAmount = 0;
            
            if(listOfNetAmounts[minIndex].netAmount == 0) numZeroNetAmounts++;
            
            if(listOfNetAmounts[simpleMaxIndex].netAmount == 0) numZeroNetAmounts++;
            
        }
        else {
            int transactionAmount = min(abs(listOfNetAmounts[minIndex].netAmount), listOfNetAmounts[maxIndex].netAmount);
            
            (ansGraph[minIndex][maxIndex].first) += (transactionAmount);
            (ansGraph[minIndex][maxIndex].second) = maxAns.second;
            
            listOfNetAmounts[minIndex].netAmount += transactionAmount;
            listOfNetAmounts[maxIndex].netAmount -= transactionAmount;
            
            if(listOfNetAmounts[minIndex].netAmount == 0) numZeroNetAmounts++;
            
            if(listOfNetAmounts[maxIndex].netAmount == 0) numZeroNetAmounts++;
            
        }
        
    }
    
    printAns(ansGraph, numBanks, input);
    // cout<<"HI\n";
}

// Main function
int main() {
    clearScreen();
    printHeader("CASH FLOW MINIMIZER SYSTEM");
    
    cout << "This system minimizes the number of transactions among multiple banks in the\n";
    cout << "different corners of the world that use different modes of payment. There is\n";
    cout << "one world bank (with all payment modes) to act as an intermediary between\n";
    cout << "banks that have no common mode of payment.\n\n";
    
    // Get number of banks
    int numBanks = getIntInput("Enter the number of banks (1-" + to_string(MAX_BANKS) + "): ", 1, MAX_BANKS);
    
    Bank input[MAX_BANKS];
    unordered_map<string, int> indexOf;
    
    // Input bank details
    printSection("BANK INFORMATION");
    int maxNumTypes = 0;
    
    for(int i = 0; i < numBanks; i++) {
        clearScreen();
        printSection((i == 0 ? "World Bank" : "Bank " + to_string(i)) + " Details");
        
        // Get bank name
        string name = getBankName("Enter bank name: ", indexOf);
        
        input[i].name = name;
        indexOf[name] = i;
        
        // Get payment modes
        int numTypes = getIntInput("Enter number of payment modes (1-" + to_string(MAX_TYPES) + "): ", 1, MAX_TYPES);
        
        if(i == 0) maxNumTypes = numTypes;
        
        input[i].types = getPaymentModes(numTypes);
        printSuccess("Bank details saved successfully!");
    }
    
    // Input transactions
    clearScreen();
    printSection("TRANSACTION INFORMATION");
    
    int numTransactions = getIntInput("Enter number of transactions: ", 1, numBanks * numBanks);
    vector<vector<int>> graph(numBanks, vector<int>(numBanks, 0));
    
    for(int i = 0; i < numTransactions; i++) {
        clearScreen();
        printSection("Transaction " + to_string(i + 1));
        
        cout << "\nAvailable banks:\n";
        for(int j = 0; j < numBanks; j++) {
            cout << "[" << (j+1) << "] " << input[j].name << "\n";
        }
        cout << "\n";
        
        // Get source bank
        int fromBank = getIntInput("Select source bank (1-" + to_string(numBanks) + "): ", 1, numBanks) - 1;
        
        // Get destination bank
        int toBank;
        do {
            toBank = getIntInput("Select destination bank (1-" + to_string(numBanks) + "): ", 1, numBanks) - 1;
            if(toBank == fromBank) {
                printError("Source and destination banks cannot be the same!");
            }
        } while(toBank == fromBank);
        
        // Get amount
        int amount;
        do {
            amount = getIntInput("Enter transaction amount (1-1000000): ", 1, 1000000);
        } while(!validateAmount(amount));
        
        graph[fromBank][toBank] = amount;
        printSuccess("Transaction recorded successfully!");
    }
    
    clearScreen();
    minimizeCashFlow(numBanks, input, indexOf, numTransactions, graph, maxNumTypes);
    return 0;
}


/*
5
A 2 t1 t2
B 1 t1
C 1 t1
D 1 t2
E 1 t2
4
B A 300
C A 700
D B 500
E B 500

--------
5
World_Bank 2 Google_Pay PayTM
Bank_B 1 Google_Pay
Bank_C 1 Google_Pay
Bank_D 1 PayTM
Bank_E 1 PayTM
4
Bank_B World_Bank 300
Bank_C World_Bank 700
Bank_D Bank_B 500
Bank_E Bank_B 500

--------------------

6
B 3 1 2 3
C 2 1 2
D 1 2
E 2 1 3
F 1 3
G 2 2 3
9
G B 30
G D 10
F B 10
F C 30
F D 10
F E 10
B C 40
C D 20
D E 50
*/