#ifndef PendingGame_
#define PendingGame_

#include <string>
#include <iostream>
using namespace std;

struct PendingGame {
    string gameName;
    int price;
    string category;
    string publisherUsername;
    string publisherStudio;
    
    PendingGame();
    PendingGame(const string& name, int p, const string& cat, const string& pubUser, const string& pubStudio);
};

PendingGame::PendingGame() 
    : gameName(""), price(0), category(""), publisherUsername(""), publisherStudio("") {}

PendingGame::PendingGame(const string& name, int p, const string& cat, 
                         const string& pubUser, const string& pubStudio)
    : gameName(name), price(p), category(cat), publisherUsername(pubUser), publisherStudio(pubStudio) {}

ostream& operator<<(ostream& out, const PendingGame& pg) {
    out << pg.gameName << " | " << pg.price << " | " << pg.category 
        << " | " << pg.publisherStudio;
    return out;
}



#endif
