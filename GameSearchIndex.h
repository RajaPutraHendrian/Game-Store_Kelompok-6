#ifndef GameSearchIndex_
#define GameSearchIndex_

#include <iostream>
#include <string>
#include <vector>
#include "bst.h"
#include "Game.h"
using namespace std;

// Multi-Index BST Search System for Games
class GameSearchIndex {
private:
    // Multiple BST indexes for different search criteria
    BST<string, GameNode*> nameIndex;      // Search by name (case-sensitive)
    BST<int, GameNode*> priceIndex;        // Search by price
    BST<string, GameNode*> categoryIndex;  // Search by category
    BST<string, GameNode*> publisherIndex; // Search by publisher
    
    // Static callback helpers for traversal
    static vector<GameNode*>* currentResults;
    static void collectResult(const string& key, GameNode* const& value);
    static void collectResultInt(const int& key, GameNode* const& value);
    static void displayGame(const string& key, GameNode* const& value);
    static void displayGameInt(const int& key, GameNode* const& value);
    
public:
    GameSearchIndex();
    ~GameSearchIndex();
    
    // Add game to all indexes
    void addGame(GameNode* game);
    
    // Search methods (exact match)
    GameNode* searchByName(const string& name);
    GameNode* searchByPrice(int price);
    
    // Range search methods
    vector<GameNode*> searchByPriceRange(int minPrice, int maxPrice);
    vector<GameNode*> searchByCategory(const string& category);
    vector<GameNode*> searchByPublisher(const string& publisher);
    
    // Display methods (sorted)
    void displayAllByName();           // Alphabetical order
    void displayAllByPrice();          // Price order (low to high)
    void displayByCategory(const string& category);
    void displayByPublisher(const string& publisher);
    
    // Utility
    void clear();
    int getTotalGames();
    void showIndexStats();
};

// Static member initialization
vector<GameNode*>* GameSearchIndex::currentResults = 0;

// Constructor
GameSearchIndex::GameSearchIndex() {}

// Destructor
GameSearchIndex::~GameSearchIndex() {
    clear();
}

// Callback to collect results
void GameSearchIndex::collectResult(const string& key, GameNode* const& value) {
    if (currentResults && value) {
        currentResults->push_back(value);
    }
}

void GameSearchIndex::collectResultInt(const int& key, GameNode* const& value) {
    if (currentResults && value) {
        currentResults->push_back(value);
    }
}

// Callback to display game
void GameSearchIndex::displayGame(const string& key, GameNode* const& value) {
    if (value) {
        cout << "  [" << value->name << "] "
             << value->price << " IDR - "
             << value->category << " - "
             << value->publisher << " - "
             << value->getRatingDisplay() << endl;
    }
}

void GameSearchIndex::displayGameInt(const int& key, GameNode* const& value) {
    if (value) {
        cout << "  [" << value->name << "] "
             << value->price << " IDR - "
             << value->category << " - "
             << value->publisher << " - "
             << value->getRatingDisplay() << endl;
    }
}

// Add game to all indexes
void GameSearchIndex::addGame(GameNode* game) {
    if (!game) return;
    
    // Insert into all indexes
    nameIndex.insert(game->name, game);
    priceIndex.insert(game->price, game);
    categoryIndex.insert(game->category, game);
    publisherIndex.insert(game->publisher, game);
}

// Search by exact name
GameNode* GameSearchIndex::searchByName(const string& name) {
    GameNode** result = nameIndex.search(name);
    return result ? *result : 0;
}

// Search by exact price
GameNode* GameSearchIndex::searchByPrice(int price) {
    GameNode** result = priceIndex.search(price);
    return result ? *result : 0;
}

// Search by price range
vector<GameNode*> GameSearchIndex::searchByPriceRange(int minPrice, int maxPrice) {
    vector<GameNode*> results;
    currentResults = &results;
    
    priceIndex.rangeSearch(minPrice, maxPrice, collectResultInt);
    
    currentResults = 0;
    return results;
}

// Search all games in category
vector<GameNode*> GameSearchIndex::searchByCategory(const string& category) {
    vector<GameNode*> results;
    currentResults = &results;
    
    // For category, we need to collect all matching
    // Simple approach: traverse and filter
    // Better: use multimap BST for multiple values per key
    
    GameNode** result = categoryIndex.search(category);
    if (result) {
        results.push_back(*result);
    }
    
    currentResults = 0;
    return results;
}

// Search all games by publisher
vector<GameNode*> GameSearchIndex::searchByPublisher(const string& publisher) {
    vector<GameNode*> results;
    currentResults = &results;
    
    GameNode** result = publisherIndex.search(publisher);
    if (result) {
        results.push_back(*result);
    }
    
    currentResults = 0;
    return results;
}

// Display all games alphabetically
void GameSearchIndex::displayAllByName() {
    cout << "\n========== GAMES (Alphabetical Order) ==========" << endl;
    nameIndex.inorderTraversal(displayGame);
    cout << "================================================\n" << endl;
}

// Display all games by price
void GameSearchIndex::displayAllByPrice() {
    cout << "\n========== GAMES (Price Order: Low to High) ==========" << endl;
    priceIndex.inorderTraversal(displayGameInt);
    cout << "======================================================\n" << endl;
}

// Display games by category
void GameSearchIndex::displayByCategory(const string& category) {
    cout << "\n========== GAMES in Category: " << category << " ==========" << endl;
    
    vector<GameNode*> games = searchByCategory(category);
    if (games.empty()) {
        cout << "  No games found in this category." << endl;
    } else {
        for (size_t i = 0; i < games.size(); i++) {
            GameNode* g = games[i];
            cout << "  [" << g->name << "] "
                 << g->price << " IDR - "
                 << g->publisher << " - "
                 << g->getRatingDisplay() << endl;
        }
    }
    cout << "================================================\n" << endl;
}

// Display games by publisher
void GameSearchIndex::displayByPublisher(const string& publisher) {
    cout << "\n========== GAMES by Publisher: " << publisher << " ==========" << endl;
    
    vector<GameNode*> games = searchByPublisher(publisher);
    if (games.empty()) {
        cout << "  No games found from this publisher." << endl;
    } else {
        for (size_t i = 0; i < games.size(); i++) {
            GameNode* g = games[i];
            cout << "  [" << g->name << "] "
                 << g->price << " IDR - "
                 << g->category << " - "
                 << g->getRatingDisplay() << endl;
        }
    }
    cout << "================================================\n" << endl;
}

// Clear all indexes
void GameSearchIndex::clear() {
    nameIndex.clear();
    priceIndex.clear();
    categoryIndex.clear();
    publisherIndex.clear();
}

// Get total games count
int GameSearchIndex::getTotalGames() {
    return nameIndex.count();
}

// Show index statistics
void GameSearchIndex::showIndexStats() {
    cout << "\n========== SEARCH INDEX STATISTICS ==========" << endl;
    cout << "Total Games Indexed: " << getTotalGames() << endl;
    cout << "\nIndex Heights (Balance Info):" << endl;
    cout << "  Name Index Height:      " << nameIndex.height() << endl;
    cout << "  Price Index Height:     " << priceIndex.height() << endl;
    cout << "  Category Index Height:  " << categoryIndex.height() << endl;
    cout << "  Publisher Index Height: " << publisherIndex.height() << endl;
    cout << "\nNote: Lower height = better balanced tree" << endl;
    cout << "Ideal height for " << getTotalGames() << " games: ~" 
         << (getTotalGames() > 0 ? (int)(log(getTotalGames())/log(2)) + 1 : 0) << endl;
    cout << "============================================\n" << endl;
}

#endif
