#ifndef GameMenu_
#define GameMenu_

#include <iostream>
#include <iomanip>
#include <cctype>
#include "StringUtils.h"
#include "Constants.h"
#include <string>
#include "Game.h"
using namespace std;

bool comparePriceLowToHigh(GameNode* a, GameNode* b);
bool compareNameAZ(GameNode* a, GameNode* b);
bool compareMostPopular(GameNode* a, GameNode* b);
bool compareHighestRated(GameNode* a, GameNode* b);

class GameMenu {
private:
    GameNode* head;
    GameNode* tail;
    int count;
    
    GameNode** createNodeArray(int& outCount, const string& filter = "All");
    void bubbleSortArray(GameNode** arr, int size, bool (*compareFunc)(GameNode*, GameNode*));

public:
    GameMenu();
    ~GameMenu();
    
    void clear();
    bool exists(const string& gameName);
    void addGame(const string& name, int price, const string& category, const string& publisher = "Default");
    GameNode* findGame(const string& gameName);
    void updateGame(const string& oldName, const string& newName, int newPrice, const string& newCategory);
    void incrementSales(const string& gameName);
    void updateGameRating(const string& gameName, const string& username, int rating);
    void updateGameRatingWithReview(const string& gameName, const string& username, int rating, const string& review);
    int getUserRatingForGame(const string& gameName, const string& username);
    
    void displayAll(const string& filter, GameNode** resultArray, int& resultCount, int maxResults = 100);
    void displayByPublisher(const string& publisherStudio);
    void searchGames(const string& keyword, GameNode** resultArray, int& resultCount, int maxResults = 100);
    void displayAllSorted(const string& filter, GameNode** resultArray, int& resultCount,
                         int sortOption = 0, int priceFilter = 0, int freeFilter = 0);
    
    int getCount() const;
};

void toLowerStr(string& str) {
    for (size_t i = 0; i < str.length(); ++i) {
        str[i] = tolower(static_cast<unsigned char>(str[i]));
    }
}

bool comparePriceLowToHigh(GameNode* a, GameNode* b) {
    return a->price < b->price;
}

bool compareNameAZ(GameNode* a, GameNode* b) {
    string nameA = a->name;
    string nameB = b->name;
    toLowerStr(nameA);
    toLowerStr(nameB);
    return nameA < nameB;
}

bool compareMostPopular(GameNode* a, GameNode* b) {
    return a->salesCount > b->salesCount;
}

bool compareHighestRated(GameNode* a, GameNode* b) {
    return a->getAvgRating() > b->getAvgRating();
}

GameMenu::GameMenu() : head(NULL), tail(NULL), count(0) {}

GameMenu::~GameMenu() {
    clear();
}

void GameMenu::clear() {
    GameNode* cur = head;
    while (cur != NULL) {
        GameNode* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    head = NULL;
    tail = NULL;
    count = 0;
}

bool GameMenu::exists(const string& gameName) {
    string cleaned_name = trim(gameName);
    GameNode* cur = head;
    while (cur != NULL) {
        if (cur->name == cleaned_name) return true;
        cur = cur->next;
    }
    return false;
}

void GameMenu::addGame(const string& name, int price, const string& category, const string& publisher) {
    string cleaned_name = trim(name);
    if (exists(cleaned_name)) return;
    
    GameNode* newNode = new GameNode(cleaned_name, price, category, publisher);
    
    newNode->gameRatingSystem->loadFromFile(cleaned_name + RATING_FILE_SUFFIX);
    newNode->gameRatingSystem->getOverallRating(newNode->totalRating, newNode->ratingCount);
    
    if (head == NULL) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    count++;
}

GameNode* GameMenu::findGame(const string& gameName) {
    string cleaned_name = trim(gameName);
    GameNode* cur = head;
    while (cur != NULL) {
        if (cur->name == cleaned_name) return cur;
        cur = cur->next;
    }
    return NULL;
}

void GameMenu::updateGame(const string& oldName, const string& newName, int newPrice, const string& newCategory) {
    GameNode* game = findGame(oldName);
    if (game != NULL) {
        string cleaned_new_name = trim(newName);
        if (oldName != cleaned_new_name && exists(cleaned_new_name)) return;
        game->name = cleaned_new_name;
        game->price = newPrice;
        game->category = newCategory;
    }
}

void GameMenu::incrementSales(const string& gameName) {
    GameNode* game = findGame(gameName);
    if (game != NULL) {
        game->salesCount++;
    }
}

void GameMenu::updateGameRating(const string& gameName, const string& username, int rating) {
    GameNode* game = findGame(gameName);
    if (game == NULL) return;

    game->gameRatingSystem->addOrUpdateRating(username, rating);
    game->gameRatingSystem->getOverallRating(game->totalRating, game->ratingCount);
}

void GameMenu::updateGameRatingWithReview(const string& gameName, const string& username, int rating, const string& review) {
    GameNode* game = findGame(gameName);
    if (game == NULL) return;

    game->gameRatingSystem->addOrUpdateRating(username, rating, review);
    game->gameRatingSystem->getOverallRating(game->totalRating, game->ratingCount);
}

int GameMenu::getUserRatingForGame(const string& gameName, const string& username) {
    GameNode* game = findGame(gameName);
    if (game == NULL) return 0;
    return game->gameRatingSystem->getUserRating(username);
}

void GameMenu::displayAll(const string& filter, GameNode** resultArray, int& resultCount, int maxResults) {
    const int itemsPerPage = 50;
    GameNode* pageStartNode = head;
    bool exitLoop = false;
    int pageNumber = 1;
    resultCount = 0;

    while (!exitLoop) {
        cout << "\n=== GAME STORE (Page " << pageNumber << ") ===";
        cout << "\nFilter: " << filter << " (Menampilkan " << itemsPerPage << " item per halaman)\n";
        cout << left << setw(4) << "No"
             << setw(30) << " Nama Game"
             << setw(10) << " Harga"
             << setw(15) << " Kategori"
             << setw(15) << " Rating"
             << setw(15) << " Publisher" << endl;
        cout << string(90, '-') << endl;

        GameNode* cur = pageStartNode;
        int nodesTraversed = 0;
        int itemsPrinted = 0;
        int startIndex = (pageNumber - 1) * itemsPerPage;

        while (cur != NULL && nodesTraversed < itemsPerPage) {
            if (filter == "All" || cur->category.find(filter) != string::npos) {
                int displayNum = startIndex + itemsPrinted + 1;
                cout << setw(4) << displayNum
                     << setw(30) << truncateString(cur->name, 27)
                     << setw(10) << cur->price
                     << setw(15) << cur->category
                     << setw(15) << cur->getRatingDisplay()
                     << setw(15) << cur->publisher << endl;
                
                // Track in result array
                if (resultCount < maxResults) {
                    resultArray[resultCount] = cur;
                    resultCount++;
                }
                itemsPrinted++;
            }
            cur = cur->next;
            nodesTraversed++;
        }

        if (itemsPrinted == 0) {
            cout << "(Tidak ada game untuk ditampilkan di halaman ini)\n";
        }

        cout << string(90, '-') << endl;
        cout << "Navigasi: [N] Next    [P] Previous    [Q] Quit to Menu\n";
        cout << "Pilihan Anda: ";

        char navChoice;
        if (!(cin >> navChoice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            navChoice = ' ';
        }
        navChoice = toupper(static_cast<unsigned char>(navChoice));

        switch (navChoice) {
            case 'Q':
                exitLoop = true;
                break;
            case 'N':
                if (cur != NULL) {
                    pageStartNode = cur;
                    pageNumber++;
                } else {
                    cout << "\n** Anda sudah di halaman terakhir. **\n";
                    cout << "Tekan Enter untuk melanjutkan...";
                    cin.ignore(1000, '\n');
                    cin.get();
                }
                break;
            case 'P':
                if (pageStartNode == head) {
                    cout << "\n** Anda sudah di halaman pertama. **\n";
                    cout << "Tekan Enter untuk melanjutkan...";
                    cin.ignore(1000, '\n');
                    cin.get();
                } else {
                    GameNode* temp = pageStartNode;
                    for (int i = 0; i < itemsPerPage; i++) {
                        if (temp->prev != NULL) {
                            temp = temp->prev;
                        } else {
                            break;
                        }
                    }
                    pageStartNode = temp;
                    if (pageNumber > 1) pageNumber--;
                }
                break;
            default:
                cout << "\nPilihan tidak valid. Coba lagi.\n";
                cout << "Tekan Enter untuk melanjutkan...";
                cin.ignore(1000, '\n');
                cin.get();
                break;
        }
    }
}

void GameMenu::displayByPublisher(const string& publisherStudio) {
    cout << "\n================================" << endl;
    cout << "        DAFTAR GAME ANDA " << endl;
    cout << "================================" << endl;

    cout << left << setw(4) << "No"
         << setw(30) << " Nama Game"
         << setw(12) << " Harga"
         << setw(15) << " Kategori"
         << setw(15) << " Rating"
         << setw(10) << " Terjual" << endl;
    cout << string(86, '-') << endl;

    int index = 1;
    bool found = false;
    GameNode* cur = head;
    while (cur != NULL) {
        if (cur->publisher == publisherStudio) {
            cout << setw(4) << index++
                 << setw(30) << truncateString(cur->name, 27)
                 << setw(12) << cur->price
                 << setw(15) << cur->category
                 << setw(15) << cur->getRatingDisplay()
                 << setw(10) << cur->salesCount << endl;
            found = true;
        }
        cur = cur->next;
    }

    if (!found) {
        cout << "Anda belum memiliki game yang disetujui.\n";
    }
}

void GameMenu::searchGames(const string& keyword, GameNode** resultArray, int& resultCount, int maxResults) {
    cout << "\n================================" << endl;
    cout << "            SEARCH GAME " << endl;
    cout << "================================" << endl;
    cout << left << setw(4) << "No"
         << setw(30) << " Nama Game"
         << setw(10) << " Harga"
         << setw(15) << " Kategori"
         << setw(15) << " Rating"
         << setw(15) << " Publisher" << endl;
    cout << string(90, '-') << endl;

    int index = 1;
    bool found = false;
    resultCount = 0;
    GameNode* cur = head;
    while (cur != NULL) {
        string lowerKeyword = trim(keyword);
        string lowerGameName = cur->name;
        for (size_t i = 0; i < lowerKeyword.length(); ++i)
            lowerKeyword[i] = tolower(static_cast<unsigned char>(lowerKeyword[i]));
        for (size_t i = 0; i < lowerGameName.length(); ++i)
            lowerGameName[i] = tolower(static_cast<unsigned char>(lowerGameName[i]));
            
        if (lowerGameName.find(lowerKeyword) != string::npos) {
            cout << setw(4) << index++
                 << setw(30) << truncateString(cur->name, 27)
                 << setw(10) << cur->price
                 << setw(15) << cur->category
                 << setw(15) << cur->getRatingDisplay()
                 << setw(15) << cur->publisher << endl;
            
            if (resultCount < maxResults) {
                resultArray[resultCount] = cur;
                resultCount++;
            }
            found = true;
        }
        cur = cur->next;
    }

    if (!found) {
        cout << "Tidak ada game dengan keyword \"" << keyword << "\"\n";
    }
}

int GameMenu::getCount() const {
    return count;
}

GameNode** GameMenu::createNodeArray(int& outCount, const string& filter) {
    outCount = 0;
    GameNode* cur = head;
    while (cur != NULL) {
        if (filter == "All" || cur->category.find(filter) != string::npos) {
            outCount++;
        }
        cur = cur->next;
    }
    
    if (outCount == 0) return NULL;
    
    GameNode** arr = new GameNode*[outCount];
    int idx = 0;
    cur = head;
    while (cur != NULL) {
        if (filter == "All" || cur->category.find(filter) != string::npos) {
            arr[idx++] = cur;
        }
        cur = cur->next;
    }
    return arr;
}

void GameMenu::bubbleSortArray(GameNode** arr, int size, bool (*compareFunc)(GameNode*, GameNode*)) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (compareFunc(arr[j + 1], arr[j])) {
                GameNode* temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void GameMenu::displayAllSorted(const string& filter, GameNode** resultArray, int& resultCount,
                               int sortOption, int priceFilter, int freeFilter) {
    const int itemsPerPage = 50;
    
    resultCount = 0;
    int arraySize = 0;
    GameNode** gameArray = createNodeArray(arraySize, filter);
    
    if (gameArray == NULL || arraySize == 0) {
        cout << "\n=== GAME STORE ===";
        cout << "\nFilter: " << filter << "\n";
        cout << "(Tidak ada game untuk ditampilkan)\n";
        return;
    }
    
    switch (sortOption) {
        case 1: bubbleSortArray(gameArray, arraySize, comparePriceLowToHigh); break;
        case 2: bubbleSortArray(gameArray, arraySize, compareNameAZ); break;
        case 3: bubbleSortArray(gameArray, arraySize, compareMostPopular); break;
        case 4: bubbleSortArray(gameArray, arraySize, compareHighestRated); break;
        default: break;
    }
    
    int currentPage = 1;
    bool exitLoop = false;
    
    while (!exitLoop) {
        cout << "\n=== GAME STORE (Page " << currentPage << ") ===";
        cout << "\nFilter: " << filter;
        
        if (sortOption > 0) {
            cout << " | Sort: ";
            switch (sortOption) {
                case 1: cout << "Price (Low-High)"; break;
                case 2: cout << "Name (A-Z)"; break;
                case 3: cout << "Most Popular"; break;
                case 4: cout << "Highest Rated"; break;
            }
        }
        if (priceFilter > 0) {
            cout << " | Price: ";
            switch (priceFilter) {
                case 1: cout << "<50k"; break;
                case 2: cout << "50k-100k"; break;
                case 3: cout << ">100k"; break;
            }
        }
        if (freeFilter > 0) {
            cout << " | Type: " << (freeFilter == 1 ? "Free" : "Paid");
        }
        
        cout << "\n" << left << setw(4) << "No"
             << setw(30) << " Nama Game"
             << setw(10) << " Harga"
             << setw(15) << " Kategori"
             << setw(15) << " Rating"
             << setw(15) << " Publisher" << endl;
        cout << string(90, '-') << endl;
        
        int startIdx = (currentPage - 1) * itemsPerPage;
        int endIdx = startIdx + itemsPerPage;
        if (endIdx > arraySize) endIdx = arraySize;
        
        int displayCount = 0;
        for (int i = startIdx; i < endIdx; i++) {
            GameNode* game = gameArray[i];
            
            if (priceFilter == 1 && game->price >= 50000) continue;
            if (priceFilter == 2 && (game->price < 50000 || game->price > 100000)) continue;
            if (priceFilter == 3 && game->price <= 100000) continue;
            
            if (freeFilter == 1 && game->price != 0) continue;
            if (freeFilter == 2 && game->price == 0) continue;
            
            cout << setw(4) << (i + 1)
                 << setw(30) << truncateString(game->name, 27)
                 << setw(10) << game->price
                 << setw(15) << game->category
                 << setw(15) << game->getRatingDisplay()
                 << setw(15) << game->publisher << endl;
            
            // Track in result array
            if (resultCount < 100) {
                resultArray[resultCount] = game;
                resultCount++;
            }
            displayCount++;
        }
        
        if (displayCount == 0) {
            cout << "(Tidak ada game yang sesuai filter di hal aman ini)\n";
        }
        
        cout << string(90, '-') << endl;
        cout << "Navigasi: [N] Next    [P] Previous    [Q] Quit to Menu\n";
        cout << "Pilihan Anda: ";
        
        char navChoice;
        if (!(cin >> navChoice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            navChoice = ' ';
        }
        navChoice = toupper(static_cast<unsigned char>(navChoice));
        
        switch (navChoice) {
            case 'Q':
                exitLoop = true;
                break;
            case 'N':
                if (endIdx < arraySize) {
                    currentPage++;
                } else {
                    cout << "\n** Anda sudah di halaman terakhir. **\n";
                    cout << "Tekan Enter untuk melanjutkan...";
                    cin.ignore(1000, '\n');
                    cin.get();
                }
                break;
            case 'P':
                if (currentPage > 1) {
                    currentPage--;
                } else {
                    cout << "\n** Anda sudah di halaman pertama. **\n";
                    cout << "Tekan Enter untuk melanjutkan...";
                    cin.ignore(1000, '\n');
                    cin.get();
                }
                break;
            default:
                cout << "\nPilihan tidak valid. Coba lagi.\n";
                cout << "Tekan Enter untuk melanjutkan...";
                cin.ignore(1000, '\n');
                cin.get();
                break;
        }
    }
    
    delete[] gameArray;
}

#endif

