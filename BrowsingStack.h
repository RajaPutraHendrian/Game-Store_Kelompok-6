#ifndef BrowsingStack_
#define BrowsingStack_

#include <iostream>
#include <fstream>
#include "BrowsingStack.h"
#include "StringUtils.h"
#include <string>
using namespace std;

class GameBrowsingStack {
private:
    struct BrowseNode {
        string gameName;
        BrowseNode* next;
        BrowseNode(const string& name);
    };
    
    BrowseNode* top;
    int maxSize;
    int currentSize;
    
    void popBottom();

public:
    GameBrowsingStack(int limit = 10);
    ~GameBrowsingStack();
    
    void clear();
    void pushGame(const string& gameName);
    string popGame();
    string peekTop();
    bool isEmpty() const;
    int size() const;
    void showHistory() const;
    string getAt(int index) const;
    void saveToFile(const string& filename);
    void loadFromFile(const string& filename);
};

GameBrowsingStack::BrowseNode::BrowseNode(const string& name) : gameName(trim(name)), next(NULL) {}

GameBrowsingStack::GameBrowsingStack(int limit) : top(NULL), maxSize(limit), currentSize(0) {}

GameBrowsingStack::~GameBrowsingStack() {
    clear();
}

void GameBrowsingStack::clear() {
    while (top != NULL) {
        BrowseNode* temp = top;
        top = top->next;
        delete temp;
    }
    currentSize = 0;
}

void GameBrowsingStack::pushGame(const string& gameName) {
    string cleaned = trim(gameName);
    if (cleaned.empty()) return;
    
    if (top != NULL && top->gameName == cleaned) return;
    
    if (currentSize >= maxSize) {
        popBottom();
    }
    
    BrowseNode* newNode = new BrowseNode(cleaned);
    newNode->next = top;
    top = newNode;
    currentSize++;
}

string GameBrowsingStack::popGame() {
    if (isEmpty()) return "";
    
    BrowseNode* temp = top;
    string gameName = top->gameName;
    top = top->next;
    delete temp;
    currentSize--;
    
    return gameName;
}

string GameBrowsingStack::peekTop() {
    if (isEmpty()) return "";
    return top->gameName;
}

bool GameBrowsingStack::isEmpty() const {
    return top == NULL;
}

int GameBrowsingStack::size() const {
    return currentSize;
}

void GameBrowsingStack::showHistory() const {
    if (isEmpty()) {
        cout << "\n(Belum ada riwayat browsing)\n";
        return;
    }
    
    cout << "\n================================" << endl;
    cout << "     RIWAYAT BROWSING GAME " << endl;
    cout << "================================" << endl;
    cout << "(Dari terbaru ke terlama)\n\n";
    
    int index = 1;
    BrowseNode* current = top;
    while (current != NULL) {
        cout << index++ << ". " << current->gameName << "\n";
        current = current->next;
    }
    cout << "================================" << endl;
}

void GameBrowsingStack::popBottom() {
    if (isEmpty()) return;
    
    if (top->next == NULL) {
        delete top;
        top = NULL;
        currentSize = 0;
        return;
    }
    
    BrowseNode* current = top;
    while (current->next->next != NULL) {
        current = current->next;
    }
    
    delete current->next;
    current->next = NULL;
    currentSize--;
}

string GameBrowsingStack::getAt(int index) const {
    int idx = 1;
    BrowseNode* current = top;
    while (current != NULL) {
        if (idx == index) return current->gameName;
        idx++;
        current = current->next;
    }
    return "";
}

void GameBrowsingStack::saveToFile(const string& filename) {
    ofstream out(filename.c_str());
    if (!out) return;
    
    BrowseNode* current = top;
    while (current != NULL) {
        out << current->gameName << endl;
        current = current->next;
    }
    out.close();
}

void GameBrowsingStack::loadFromFile(const string& filename) {
    clear();
    
    ifstream in(filename.c_str());
    if (!in) return;
    
    string games[100];
    int count = 0;
    string line;
    
    while (getline(in, line) && count < 100) {
        string cleaned = trim(line);
        if (!cleaned.empty()) {
            games[count++] = cleaned;
        }
    }
    in.close();
    
    for (int i = count - 1; i >= 0; i--) {
        pushGame(games[i]);
    }
}

#endif
