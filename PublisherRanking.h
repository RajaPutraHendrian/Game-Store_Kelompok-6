#ifndef PublisherRanking_
#define PublisherRanking_

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <map>
#include "AVLTree.h"
#include "Constants.h"
#include "StringUtils.h"
using namespace std;

// Publisher data for ranking
struct PublisherRankData {
    string studio;
    string username;
    int totalSales;
    int gameCount;
    long long totalRevenue;
    
    PublisherRankData() : totalSales(0), gameCount(0), totalRevenue(0) {}
};

// Publisher Ranking System using AVL Tree
class PublisherRanking {
private:
    AVLTree<int, PublisherRankData*> salesRanking;  // Sales → Publisher (AVL!)
    map<string, PublisherRankData*> publisherData;  // Studio → Data
    int totalPublishers;
    
    void loadPublisherGames();
    
public:
    PublisherRanking();
    ~PublisherRanking();
    
    void buildRankings();
    
    // Admin view - full details
    void showFullRankings();
    
    // Publisher view - own rank + limited info
    void showPublisherOwnRank(const string& studio);
    
    // User view - browse by popularity
    void showTopPublishersForBrowsing();
    
    // Utilities
    int getPublisherRank(const string& studio);
    int getTotalPublishers() const { return totalPublishers; }
};

// Constructor
PublisherRanking::PublisherRanking() : totalPublishers(0) {}

// Destructor
PublisherRanking::~PublisherRanking() {
    // Clean up allocated data
    for (map<string, PublisherRankData*>::iterator it = publisherData.begin();
         it != publisherData.end(); ++it) {
        delete it->second;
    }
}

// Load publisher games and calculate stats
void PublisherRanking::loadPublisherGames() {
    // Load all publishers first
    ifstream pubFile(PUBLISHER_FILE.c_str());
    if (!pubFile) return;
    
    string line;
    while (getline(pubFile, line)) {
        if (line.empty()) continue;
        
        size_t pos = line.find('|');
        if (pos == string::npos) continue;
        string username = line.substr(0, pos);
        
        // Extract studio name (6th field)
        size_t studioPos = 0;
        for (int i = 0; i < 6; i++) {
            studioPos = line.find('|', studioPos);
            if (studioPos == string::npos) break;
            studioPos++;
        }
        if (studioPos == string::npos) continue;
        
        size_t nextPos = line.find('|', studioPos);
        if (nextPos == string::npos) continue;
        string studioName = line.substr(studioPos, nextPos - studioPos);
        
        // Create publisher data
        PublisherRankData* data = new PublisherRankData();
        data->studio = studioName;
        data->username = username;
        publisherData[studioName] = data;
        totalPublishers++;
        
        // Load games for this publisher
        string gameFile = username + "_games.txt";
        ifstream gFile(gameFile.c_str());
        if (!gFile) continue;
        
        string gameLine;
        while (getline(gFile, gameLine)) {
            if (gameLine.empty()) continue;
            
            size_t p = 0;
            size_t np;
            
            // Game name
            np = gameLine.find('|', p);
            if (np == string::npos) continue;
            p = np + 1;
            
            // Price
            np = gameLine.find('|', p);
            if (np == string::npos) continue;
            int price = stringToInt(gameLine.substr(p, np - p));
            p = np + 1;
            
            // Category
            np = gameLine.find('|', p);
            if (np == string::npos) continue;
            p = np + 1;
            
            // Sales count
            int sales = stringToInt(gameLine.substr(p));
            
            data->totalSales += sales;
            data->gameCount++;
            data->totalRevenue += (long long)price * sales;
        }
        gFile.close();
    }
    pubFile.close();
}

// Build rankings using AVL tree
void PublisherRanking::buildRankings() {
    // Clear old data
    salesRanking.clear();
    for (map<string, PublisherRankData*>::iterator it = publisherData.begin();
         it != publisherData.end(); ++it) {
        delete it->second;
    }
    publisherData.clear();
    totalPublishers = 0;
    
    // Load fresh data
    loadPublisherGames();
    
    // Insert into AVL tree (auto-balances!)
    for (map<string, PublisherRankData*>::iterator it = publisherData.begin();
         it != publisherData.end(); ++it) {
        salesRanking.insert(it->second->totalSales, it->second);
    }
}

// Admin: Full rankings with all details
void PublisherRanking::showFullRankings() {
    cout << "\n=======================================" << endl;
    cout << "   PUBLISHER RANKINGS (Admin View)" << endl;
    cout << "=======================================" << endl;
    cout << "Platform-wide publisher performance\n" << endl;
    
    if (totalPublishers == 0) {
        cout << "No publishers found." << endl;
        return;
    }
    
    cout << left << setw(4) << "Rank"
         << setw(25) << "Studio"
         << setw(12) << "Sales"
         << setw(15) << "Revenue"
         << setw(10) << "Games" << endl;
    cout << string(66, '-') << endl;
    
    // Get all publishers sorted by sales (descending)
    vector<pair<int, PublisherRankData*> > rankings = salesRanking.getLargestN(totalPublishers);
    
    long long totalSales = 0;
    long long totalRevenue = 0;
    
    for (size_t i = 0; i < rankings.size(); i++) {
        PublisherRankData* data = rankings[i].second;
        
        cout << setw(4) << ("#" + intToString(i + 1))
             << setw(25) << data->studio
             << setw(12) << data->totalSales
             << setw(15) << data->totalRevenue
             << setw(10) << data->gameCount << endl;
        
        totalSales += data->totalSales;
        totalRevenue += data->totalRevenue;
    }
    
    cout << string(66, '=') << endl;
    cout << "Total Publishers: " << totalPublishers << endl;
    cout << "Total Platform Sales: " << totalSales << " games" << endl;
    cout << "Total Platform Revenue: " << totalRevenue << endl;
    cout << "=======================================" << endl;
}

// Publisher: Own rank + limited competitive view
void PublisherRanking::showPublisherOwnRank(const string& studio) {
    cout << "\n=======================================" << endl;
    cout << "      YOUR PUBLISHER RANKING" << endl;
    cout << "=======================================" << endl;
    
    if (publisherData.find(studio) == publisherData.end()) {
        cout << "Your studio not found in rankings." << endl;
        return;
    }
    
    PublisherRankData* myData = publisherData[studio];
    int myRank = getPublisherRank(studio);
    
    cout << "Studio: " << studio << endl;
    cout << "Rank: #" << myRank << " out of " << totalPublishers << endl;
    cout << "Total Sales: " << myData->totalSales << " games" << endl;
    cout << "Games Published: " << myData->gameCount << endl;
    cout << "Total Revenue: " << myData->totalRevenue << endl;
    
    cout << "\n=== TOP 10 PUBLISHERS ===" << endl;
    cout << "(Limited view for competitive privacy)" << endl;
    cout << string(40, '-') << endl;
    
    vector<pair<int, PublisherRankData*> > top10 = salesRanking.getLargestN(10);
    
    for (size_t i = 0; i < top10.size(); i++) {
        PublisherRankData* data = top10[i].second;
        
        cout << "#" << (i + 1) << " " << data->studio;
        
        if (data->studio == studio) {
            cout << " ← YOU";
        }
        
        // Show tier instead of exact numbers
        if (data->totalSales >= 100) {
            cout << " (⭐⭐⭐⭐⭐ Platinum)";
        } else if (data->totalSales >= 50) {
            cout << " (⭐⭐⭐⭐ Gold)";
        } else if (data->totalSales >= 20) {
            cout << " (⭐⭐⭐ Silver)";
        } else if (data->totalSales >= 5) {
            cout << " (⭐⭐ Bronze)";
        } else {
            cout << " (⭐ Starter)";
        }
        cout << endl;
    }
    
    cout << "=======================================" << endl;
}

// User: Browse top publishers
void PublisherRanking::showTopPublishersForBrowsing() {
    cout << "\n=======================================" << endl;
    cout << "     BROWSE BY TOP PUBLISHERS" << endl;
    cout << "=======================================" << endl;
    cout << "Most successful game studios:\n" << endl;
    
    if (totalPublishers == 0) {
        cout << "No publishers found." << endl;
        return;
    }
    
    cout << left << setw(4) << "No"
         << setw(30) << "Studio Name"
         << setw(15) << "Games Available" << endl;
    cout << string(49, '-') << endl;
    
    vector<pair<int, PublisherRankData*> > top10 = salesRanking.getLargestN(10);
    
    for (size_t i = 0; i < top10.size(); i++) {
        PublisherRankData* data = top10[i].second;
        
        cout << setw(4) << (i + 1)
             << setw(30) << data->studio
             << setw(15) << data->gameCount << endl;
    }
    
    cout << "\n(Select publisher number to browse their games)" << endl;
    cout << "=======================================" << endl;
}

// Get publisher rank
int PublisherRanking::getPublisherRank(const string& studio) {
    if (publisherData.find(studio) == publisherData.end()) {
        return -1;
    }
    
    int sales = publisherData[studio]->totalSales;
    vector<pair<int, PublisherRankData*> > all = salesRanking.getLargestN(totalPublishers);
    
    for (size_t i = 0; i < all.size(); i++) {
        if (all[i].second->studio == studio) {
            return i + 1;
        }
    }
    
    return -1;
}

#endif
