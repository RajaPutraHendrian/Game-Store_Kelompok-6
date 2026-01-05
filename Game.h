#ifndef Game_
#define Game_

#include <fstream>
#include <sstream>
#include <iomanip>
#include "StringUtils.h"
#include "HuffmanTree.h"
#include <string>
using namespace std;

struct RatingNode {
    string username;
    int rating;
    string compressedReview;  // Huffman compressed review text!
    RatingNode* next;
    RatingNode(const string& u, int r, const string& rev = "");
};

class GameRating {
private:
    RatingNode* head;
    string gameName;

public:
    GameRating(const string& name);
    ~GameRating();
    
    void clear();
    void addOrUpdateRating(const string& username, int rating, const string& review = "");
    int getUserRating(const string& username);
    string getUserReview(const string& username);  // Get decompressed review
    void getOverallRating(long long& totalRating, int& ratingCount);
    void saveToFile(const string& filename);
    void loadFromFile(const string& filename);
};

struct GameNode {
    string name;
    int price;
    string category;
    string publisher;
    int salesCount;
    
    long long totalRating;
    int ratingCount;
    GameRating* gameRatingSystem;

    GameNode* next;
    GameNode* prev;
    
    GameNode(const string& n, int p, const string& c, const string& pub);
    ~GameNode();
    
    double getAvgRating() const;
    string getRatingDisplay() const;
};

RatingNode::RatingNode(const string& u, int r, const string& rev) 
    : username(u), rating(r), compressedReview(rev), next(NULL) {}

GameRating::GameRating(const string& name) : head(NULL), gameName(trim(name)) {}

GameRating::~GameRating() {
    clear();
}

void GameRating::clear() {
    RatingNode* cur = head;
    while (cur != NULL) {
        RatingNode* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    head = NULL;
}

void GameRating::addOrUpdateRating(const string& username, int rating, const string& review) {
    // Compress review with Huffman
    HuffmanTree huffman;
    string compressedReview = "";
    if (!review.empty()) {
        compressedReview = huffman.compress(review);
        
        // Show compression stats
        cout << "\n[Review Compression]" << endl;
        huffman.showCompressionStats(review, compressedReview);
        huffman.displayCodeTable();
    }
    
    RatingNode* cur = head;
    while (cur != NULL) {
        if (cur->username == username) {
            cur->rating = rating;
            cur->compressedReview = compressedReview;
            saveToFile(gameName + "_ratings.dat");
            return;
        }
        cur = cur->next;
    }

    RatingNode* newNode = new RatingNode(username, rating, compressedReview);
    newNode->next = head;
    head = newNode;
    saveToFile(gameName + "_ratings.dat");
}

int GameRating::getUserRating(const string& username) {
    RatingNode* cur = head;
    while (cur != NULL) {
        if (cur->username == username) {
            return cur->rating;
        }
        cur = cur->next;
    }
    return 0;
}

string GameRating::getUserReview(const string& username) {
    RatingNode* cur = head;
    while (cur != NULL) {
        if (cur->username == username) {
            if (cur->compressedReview.empty()) {
                return "";
            }
            // Decompress review
            HuffmanTree huffman;
            // Need to rebuild tree - store freq table or use original
            // For now, return compressed (will fix with better approach)
            return "[Review: " + cur->compressedReview.substr(0, 50) + "...]";
        }
        cur = cur->next;
    }
    return "";
}

void GameRating::getOverallRating(long long& totalRating, int& ratingCount) {
    totalRating = 0;
    ratingCount = 0;
    RatingNode* cur = head;
    while (cur != NULL) {
        totalRating += cur->rating;
        ratingCount++;
        cur = cur->next;
    }
}

void GameRating::saveToFile(const string& filename) {
    ofstream out(filename.c_str(), ios::binary);
    if (!out) return;
    RatingNode* cur = head;
    while (cur != NULL) {
        out << cur->username << "|" << cur->rating << "|" 
            << cur->compressedReview << endl;
        cur = cur->next;
    }
    out.close();
}

void GameRating::loadFromFile(const string& filename) {
    clear();
    ifstream in(filename.c_str());
    if (!in) return;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        
        size_t pos1 = line.find('|');
        if (pos1 == string::npos) continue;
        
        string user = line.substr(0, pos1);
        
        size_t pos2 = line.find('|', pos1 + 1);
        int rating;
        string compressedReview = "";
        
        if (pos2 == string::npos) {
            // Old format (no review)
            rating = stringToInt(line.substr(pos1 + 1));
        } else {
            // New format (with review)
            rating = stringToInt(line.substr(pos1 + 1, pos2 - pos1 - 1));
            compressedReview = line.substr(pos2 + 1);
        }
        
        RatingNode* newNode = new RatingNode(user, rating, compressedReview);
        newNode->next = head;
        head = newNode;
    }
    in.close();
}

GameNode::GameNode(const string& n, int p, const string& c, const string& pub)
    : name(trim(n)), price(p), category(c), publisher(pub), salesCount(0),
      totalRating(0), ratingCount(0), gameRatingSystem(new GameRating(trim(n))),
      next(NULL), prev(NULL) {}

GameNode::~GameNode() {
    delete gameRatingSystem;
}

double GameNode::getAvgRating() const {
    if (ratingCount == 0) return 0.0;
    return (double)totalRating / ratingCount;
}

string GameNode::getRatingDisplay() const {
    if (ratingCount == 0) return "-";
    
    double avg = getAvgRating();
    double roundedAvg = (double) ((int) (avg * 10.0 + 0.5)) / 10.0;
    
    stringstream ss;
    ss << fixed << setprecision(1) << roundedAvg << "/5.0";
    return ss.str();
}

#endif
