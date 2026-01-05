#ifndef GameList_
#define GameList_

#include <iostream>
#include <fstream>
#include "StringUtils.h"
#include "array1d.h"
#include <string>
using namespace std;

// GameList using Dynamic Array (Array1D) instead of Linked List
// Benefits: O(1) random access, better cache locality, auto-resize
class GameList {
private:
    Array1D<string> games;  // Dynamic array!

public:
    GameList();
    ~GameList();
    
    void clear();
    bool exists(const string& g);
    void push_back(const string& g);
    bool remove(const string& g);
    int size() const;
    void printList() const;
    void saveToFile(const string& filename);
    void loadFromFile(const string& filename);
    string getAt(int n) const;
};

// Constructor
GameList::GameList() {
    // Array1D initializes itself
}

// Destructor
GameList::~GameList() {
    clear();
}

// Clear all games
void GameList::clear() {
    while (games.Count() > 0) {
        string dummy;
        games.DeleteBack(dummy);
    }
}

// Check if game exists - O(n) but with better cache locality
bool GameList::exists(const string& g) {
    string cleaned_g = trim(g);
    for (int i = 0; i < games.Count(); i++) {
        if (games[i] == cleaned_g) {
            return true;
        }
    }
    return false;
}

// Add game to list - O(1) amortized!
void GameList::push_back(const string& g) {
    if (exists(g)) return;
    
    string cleaned_g = trim(g);
    games.InsertBack(cleaned_g);  // Auto-resize if needed!
}

// Remove game from list - O(n)
bool GameList::remove(const string& g) {
    string cleaned_g = trim(g);
    
    for (int i = 0; i < games.Count(); i++) {
        if (games[i] == cleaned_g) {
            string dummy;
            games.Delete(i, dummy);
            return true;
        }
    }
    return false;
}

// Get size - O(1)!
int GameList::size() const {
    return games.Count();
}

// Print list with numbering
void GameList::printList() const {
    if (games.Count() == 0) {
        cout << "(Kosong)\n";
        return;
    }
    
    for (int i = 0; i < games.Count(); i++) {
        cout << (i + 1) << ". " << games[i] << "\n";
    }
}

// Save to file
void GameList::saveToFile(const string& filename) {
    ofstream out(filename.c_str());
    if (!out) return;
    
    for (int i = 0; i < games.Count(); i++) {
        out << games[i] << endl;
    }
    out.close();
}

// Load from file
void GameList::loadFromFile(const string& filename) {
    clear();
    
    ifstream in(filename.c_str());
    if (!in) return;
    
    string line;
    while (getline(in, line)) {
        string cleaned_line = trim(line);
        if (!cleaned_line.empty()) {
            push_back(cleaned_line);
        }
    }
    in.close();
}

// Get game at index n (1-indexed) - O(1)!
string GameList::getAt(int n) const {
    if (n < 1 || n > games.Count()) {
        return "";
    }
    return games[n - 1];  // Convert to 0-indexed
}

#endif
