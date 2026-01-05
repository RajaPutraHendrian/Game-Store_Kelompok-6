#ifndef TransactionCounter_
#define TransactionCounter_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class TransactionCounter {
private:
    static const string COUNTER_FILE;
    
    int readCounter() {
        ifstream file(COUNTER_FILE.c_str());
        if (!file) return 1;
        
        int counter;
        file >> counter;
        file.close();
        return counter;
    }
    
    void writeCounter(int counter) {
        ofstream file(COUNTER_FILE.c_str());
        if (file) {
            file << counter;
            file.close();
        }
    }

public:
    string generateTransactionID() {
        int counter = readCounter();
        
        stringstream ss;
        ss << "TRX";
        
        if (counter < 10) ss << "000";
        else if (counter < 100) ss << "00";
        else if (counter < 1000) ss << "0";
        
        ss << counter;
        
        writeCounter(counter + 1);
        
        return ss.str();
    }
};

const string TransactionCounter::COUNTER_FILE = "transaction_counter.txt";

#endif
