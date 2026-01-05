#ifndef GameStore_
#define GameStore_

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cctype>
#include <ctime>
#include <sstream>
#include "Constants.h"
#include "StringUtils.h"
#include <string>
#include "GameMenu.h"
#include "GameList.h"
#include "BrowsingStack.h"
#include "Queue.h"
#include "PendingGame.h"
#include "Membership.h"
#include "PaymentSystem.h"
#include "TransactionCounter.h"
using namespace std;

class GameStore {
private:
    GameMenu gameMenu;
    GameList wishlist;
    GameList purchased;
    GameList downloaded;
    GameBrowsingStack browsingHistory;
    Queue<PendingGame> pendingGamesQueue;
    
    string currentUsername;
    bool isPublisherMode;
    string publisherStudio;
    MembershipSystem memberSys;

    void loadPendingGamesFromFile();
    void savePendingGamesToFile();
    void updatePublisherSalesFile(const string& gameName);
    
    void printPurchaseReceipt(const string& gameName, int originalPrice, 
                             int finalPrice, bool usedVoucher,
                             const string& paymentMethod);
    void saveReceiptToFile(const string& receiptContent);

public:
    GameStore();
    
    void setUser(const string& username, bool isPublisher, const string& studio = "");
    void clearUserData();
    void loadSampleMenu();
    void loadAllPublisherGames();
    
    int countPendingGames();
    void viewPendingGames();
    void approveGame();
    void rejectGame(const string& reason);
    void verifyGameProcess();
    
    void checkRejectionNotifications();
    void viewPendingGamesByPublisher();
    
    void showMenu(const string& filter, GameNode** resultArray, int& resultCount);
    void searchGame();
    void purchaseGame(const string& gameName, bool fromWishlist = false);
    void purchaseGameWrapper();
    void addToWishlist(const string& gameName);
    void addToWishlistWrapper();
    void showWishlist();
    void addDownloadedGame(const string& gameName);
    void addDownloadedGameWrapper();
    void deleteDownloadedGame();
    void showDownloadedGames();
    void rateGame();
    void showLibrary();
    void saveDownloadedGames();
    void savePurchasedGames();
    void saveWishlist();
    
    void addPublisherGame();
    void viewPublisherGames();
    void updatePublisherGame();
    void viewPublisherStatistics();
    
    void showMenuAdvanced(const string& filter = "All", int sortOption = 0, 
                         int priceFilter = 0, int freeFilter = 0);
    void showAdvancedFilterMenu();
    
    void viewBrowsingHistory();
    void viewGameDetail(const string& gameName);
    void showQuickActionMenu(const string& gameName);
    void saveBrowsingHistory();
    
    // Lihat history struk pembelian
    void viewReceiptHistory();
};

GameStore::GameStore() {
    currentUsername = "";
    isPublisherMode = false;
    publisherStudio = "";
    loadPendingGamesFromFile();
}

void GameStore::loadPendingGamesFromFile() {
    ifstream file(PENDING_GAMES_FILE.c_str());
    if (!file) return;
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        size_t p = 0;
        size_t np;
        
        np = line.find('|', p);
        if (np == string::npos) continue;
        string gameName = line.substr(p, np - p);
        p = np + 1;
        
        np = line.find('|', p);
        if (np == string::npos) continue;
        int price = stringToInt(line.substr(p, np - p));
        p = np + 1;
        
        np = line.find('|', p);
        if (np == string::npos) continue;
        string category = line.substr(p, np - p);
        p = np + 1;
        
        np = line.find('|', p);
        if (np == string::npos) continue;
        string publisherUsername = line.substr(p, np - p);
        p = np + 1;
        
        string publisherStudio = line.substr(p);
        
        PendingGame pg(gameName, price, category, publisherUsername, publisherStudio);
        pendingGamesQueue.Enqueue(pg);
    }
    file.close();
}

void GameStore::savePendingGamesToFile() {
    ofstream file(PENDING_GAMES_FILE.c_str());
    if (!file) return;
    
    Queue<PendingGame> tempQueue;
    
    while (!pendingGamesQueue.IsEmpty()) {
        PendingGame pg;
        pendingGamesQueue.Dequeue(pg);
        
        file << pg.gameName << "|" << pg.price << "|" 
             << pg.category << "|" << pg.publisherUsername << "|"
             << pg.publisherStudio << endl;
        
        tempQueue.Enqueue(pg);
    }
    
    while (!tempQueue.IsEmpty()) {
        PendingGame pg;
        tempQueue.Dequeue(pg);
        pendingGamesQueue.Enqueue(pg);
    }
    
    file.close();
}

void GameStore::setUser(const string& username, bool isPublisher, const string& studio) {
    currentUsername = username;
    isPublisherMode = isPublisher;
    publisherStudio = studio;
    
    if (!isPublisher) {
        wishlist.loadFromFile(currentUsername + "_wishlist.txt");
        purchased.loadFromFile(currentUsername + "_library.txt");
        downloaded.loadFromFile(currentUsername + DOWNLOAD_FILE_SUFFIX);
        browsingHistory.loadFromFile(currentUsername + "_history.txt");
        memberSys.setUser(username);
    }
}

void GameStore::clearUserData() {
    wishlist.clear();
    purchased.clear();
    downloaded.clear();
    browsingHistory.clear();
    memberSys.resetUser();
    currentUsername = "";
    isPublisherMode = false;
    publisherStudio = "";
}

void GameStore::loadSampleMenu() {
    gameMenu.addGame("Pong (1972)", 53000, "Adventure");
}

void GameStore::loadAllPublisherGames() {
    ifstream pubFile(PUBLISHER_FILE.c_str());
    if (!pubFile) return;
    
    string line;
    while (getline(pubFile, line)) {
        size_t pos = line.find('|');
        if (pos == string::npos) continue;
        string username = line.substr(0, pos);
        
        size_t studioPos = 0;
        for (int i = 0; i < 14; i++) {
            studioPos = line.find('|', studioPos);
            if (studioPos == string::npos) break;
            studioPos++;
        }
        if (studioPos == string::npos) continue;
        
        size_t nextPos = line.find('|', studioPos);
        if (nextPos == string::npos) continue;
        string studioName = line.substr(studioPos, nextPos - studioPos);
        
        string gameFile = username + "_games.txt";
        ifstream gFile(gameFile.c_str());
        if (!gFile) continue;
        
        string gameLine;
        while (getline(gFile, gameLine)) {
            size_t p = 0;
            size_t np;
            
            np = gameLine.find('|', p);
            if (np == string::npos) continue;
            string gameName = gameLine.substr(p, np - p);
            p = np + 1;
            
            np = gameLine.find('|', p);
            if (np == string::npos) continue;
            int price = stringToInt(gameLine.substr(p, np - p));
            p = np + 1;
            
            np = gameLine.find('|', p);
            if (np == string::npos) continue;
            string category = gameLine.substr(p, np - p);
            p = np + 1;
            
            // Field ke-4 adalah sales count, bukan publisher
            // Publisher diambil dari studioName yang sudah di-parse sebelumnya
            
            gameMenu.addGame(gameName, price, category, studioName);
        }
        gFile.close();
    }
    pubFile.close();
}

int GameStore::countPendingGames() {
    return pendingGamesQueue.Size();
}

void GameStore::viewPendingGames() {
    cout << "\n========================================" << endl;
    cout << "        DAFTAR GAME MENUNGGU VERIFIKASI " << endl;
    cout << "        (SISTEM ANTRIAN FIFO) " << endl;
    cout << "========================================" << endl;

    if (pendingGamesQueue.IsEmpty()) {
        cout << "Tidak ada game yang menunggu verifikasi.\n";
        return;
    }

    cout << left << setw(4) << "No"
         << setw(30) << " Nama Game"
         << setw(10) << " Harga"
         << setw(15) << " Kategori"
         << setw(20) << " Publisher" << endl;
    cout << string(80, '-') << endl;

    Queue<PendingGame> tempQueue;
    int index = 1;
    
    while (!pendingGamesQueue.IsEmpty()) {
        PendingGame pg;
        pendingGamesQueue.Dequeue(pg);
        
        cout << setw(4) << index++
             << setw(30) << truncateString(pg.gameName, 27)
             << setw(10) << pg.price
             << setw(15) << pg.category
             << setw(20) << pg.publisherStudio << endl;
        
        tempQueue.Enqueue(pg);
    }
    
    while (!tempQueue.IsEmpty()) {
        PendingGame pg;
        tempQueue.Dequeue(pg);
        pendingGamesQueue.Enqueue(pg);
    }
    
    cout << "\n** Game terdepan (Front) akan diverifikasi terlebih dahulu **\n";
}

void GameStore::approveGame() {
    if (pendingGamesQueue.IsEmpty()) {
        cout << "Tidak ada game untuk diverifikasi.\n";
        return;
    }

    PendingGame game;
    pendingGamesQueue.Dequeue(game);
    
    string gameFile = game.publisherUsername + "_games.txt";
    ofstream gFile(gameFile.c_str(), ios::app);
    if (gFile) {
        gFile << game.gameName << "|" << game.price << "|" 
              << game.category << "|0" << endl;
        gFile.close();
    }

    gameMenu.addGame(game.gameName, game.price, game.category, game.publisherStudio);
    
    savePendingGamesToFile();

    cout << "\n================================" << endl;
    cout << "  GAME BERHASIL DISETUJUI!" << endl;
    cout << "================================" << endl;
    cout << "Game \"" << game.gameName << "\" dari studio " << game.publisherStudio 
         << " telah DISETUJUI dan dipublikasikan!\n";
}

void GameStore::rejectGame(const string& reason) {
    if (pendingGamesQueue.IsEmpty()) {
        cout << "Tidak ada game untuk diverifikasi.\n";
        return;
    }

    PendingGame game;
    pendingGamesQueue.Dequeue(game);
    
    string rejectionFile = game.publisherUsername + REJECTION_FILE_SUFFIX;
    ofstream rFile(rejectionFile.c_str(), ios::app);
    if (rFile) {
        rFile << "Game: " << game.gameName << " | Alasan: " << reason << endl;
        rFile.close();
    }

    savePendingGamesToFile();

    cout << "\n================================" << endl;
    cout << "      GAME DITOLAK" << endl;
    cout << "================================" << endl;
    cout << "Game \"" << game.gameName << "\" dari studio " << game.publisherStudio 
         << " telah DITOLAK.\n";
    cout << "Publisher akan menerima notifikasi penolakan.\n";
}

void GameStore::verifyGameProcess() {
    if (pendingGamesQueue.IsEmpty()) {
        cout << "\nTidak ada game yang menunggu verifikasi.\n";
        return;
    }
    
    cout << "\n================================" << endl;
    cout << "   GAME TERDEPAN (FRONT) QUEUE" << endl;
    cout << "================================" << endl;
    
    try {
        PendingGame frontGame = pendingGamesQueue.Front();
        
        cout << "Nama Game  : " << frontGame.gameName << endl;
        cout << "Harga      : Rp " << frontGame.price << endl;
        cout << "Kategori   : " << frontGame.category << endl;
        cout << "Publisher  : " << frontGame.publisherStudio << endl;
        cout << "NIK        : " << frontGame.publisherUsername << endl;
        cout << "================================" << endl;
        
    } catch(QueueEmpty) {
        cout << "Queue kosong.\n";
        return;
    }

    cout << "\n1. Setujui Game (Dequeue & Approve)" << endl;
    cout << "2. Tolak Game (Dequeue & Reject)" << endl;
    cout << "0. Batal" << endl;
    cout << "Pilih: ";
    int action;
    if (!(cin >> action)) {
        cout << "Input tidak valid.\n";
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    if (action == 1) {
        approveGame();
    } else if (action == 2) {
        cin.ignore(1000, '\n');
        cout << "Masukkan alasan penolakan: ";
        string reason;
        getline(cin, reason);
        rejectGame(reason);
    } else {
        cout << "Dibatalkan.\n";
    }
}

void GameStore::checkRejectionNotifications() {
    string rejectionFile = currentUsername + REJECTION_FILE_SUFFIX;
    ifstream file(rejectionFile.c_str());
    
    if (!file) return;

    string line;
    bool hasRejections = false;
    
    cout << "\n========================================" << endl;
    cout << "        NOTIFIKASI PENOLAKAN GAME " << endl;
    cout << "========================================" << endl;
    
    while (getline(file, line)) {
        if (!line.empty()) {
            cout << "* " << line << endl;
            hasRejections = true;
        }
    }
    file.close();

    if (hasRejections) {
        cout << "========================================" << endl;
        cout << "\nTekan Enter untuk menghapus notifikasi...";
        cin.ignore(1000, '\n');
        cin.get();
        
        ofstream clearFile(rejectionFile.c_str());
        clearFile.close();
    }
}

void GameStore::viewPendingGamesByPublisher() {
    cout << "\n========================================" << endl;
    cout << "        GAME ANDA YANG MENUNGGU VERIFIKASI " << endl;
    cout << "========================================" << endl;

    if (pendingGamesQueue.IsEmpty()) {
        cout << "Tidak ada game yang menunggu verifikasi.\n";
        return;
    }

    cout << left << setw(4) << "No"
         << setw(30) << " Nama Game"
         << setw(10) << " Harga"
         << setw(15) << " Kategori"
         << setw(20) << " Posisi di Antrian" << endl;
    cout << string(80, '-') << endl;

    Queue<PendingGame> tempQueue;
    int index = 1;
    int positionInQueue = 1;
    bool found = false;
    
    while (!pendingGamesQueue.IsEmpty()) {
        PendingGame pg;
        pendingGamesQueue.Dequeue(pg);
        
        if (pg.publisherUsername == currentUsername) {
            cout << setw(4) << index++
                 << setw(30) << truncateString(pg.gameName, 27)
                 << setw(10) << pg.price
                 << setw(15) << pg.category
                 << setw(20) << ("Antrian ke-" + intToString(positionInQueue)) << endl;
            found = true;
        }
        
        tempQueue.Enqueue(pg);
        positionInQueue++;
    }
    
    while (!tempQueue.IsEmpty()) {
        PendingGame pg;
        tempQueue.Dequeue(pg);
        pendingGamesQueue.Enqueue(pg);
    }

    if (!found) {
        cout << "Anda tidak memiliki game yang menunggu verifikasi.\n";
    } else {
        cout << "\n** Game dengan antrian lebih kecil akan diverifikasi lebih dahulu **\n";
    }
}

void GameStore::showMenu(const string& filter, GameNode** resultArray, int& resultCount) {
    gameMenu.displayAll(filter, resultArray, resultCount, 10000); // Increased limit to support selection of all games
}

void GameStore::searchGame() {
    string keyword;
    cout << "\nKetik nama game: ";
    cin.ignore(1000, '\n');
    getline(cin, keyword);
    
    GameNode* results[100];
    int resultCount = 0;
    gameMenu.searchGames(keyword, results, resultCount, 100);
    
    if (resultCount == 0) return;
    
    int choice = -1;
    
    // Single result -> immediate action
    if (resultCount == 1) {
        cout << "\nDitemukan 1 game yang sesuai!\n";
        choice = 1;
    } else {
        // Multiple results -> ask number
        cout << "\nPilih nomor game (0 untuk batal): ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            return;
        }
    }
    
    if (choice < 1 || choice > resultCount) return;
    
    GameNode* selectedGame = results[choice - 1];
    showQuickActionMenu(selectedGame->name);
}

void GameStore::purchaseGame(const string& gameName, bool fromWishlist) {
    GameNode* game = gameMenu.findGame(gameName);
    if (game == NULL) {
        cout << "Game \"" << gameName << "\" tidak ditemukan di menu toko.\n";
        return;
    }

    if (purchased.exists(gameName)) {
        cout << "Game \"" << gameName << "\" sudah ada di library Anda (sudah dibeli).\n";
        return;
    }

    int originalPrice = game->price;
    int finalPrice = originalPrice;
    bool willUseVoucher = false;

    if (memberSys.hasVoucher()) {
        if (memberSys.askUseVoucher()) {
            willUseVoucher = true;
            finalPrice = static_cast<int>(originalPrice * 0.7);
        }
    }

    cout << "Harga \"" << gameName << "\" adalah " << originalPrice;
    if (willUseVoucher) cout << " (30% voucher akan diterapkan, jadi " << finalPrice << ")";
    cout << ". Konfirmasi pembelian (Y/N): ";
    char confirmation;
    cin >> confirmation;

    if (toupper(static_cast<unsigned char>(confirmation)) == 'Y') {
        // === PAYMENT PROCESS ===
        PaymentSystem payment;
        
        cout << "\n--- PROSES PEMBAYARAN ---\n";
        if (!payment.selectPaymentMethod()) {
            cout << "Pembayaran dibatalkan.\n";
            return;
        }
        
        // Process payment
        payment.displayPaymentInfo(finalPrice);
        
        // === COMPLETE PURCHASE ===
        if (willUseVoucher) memberSys.useVoucher();
        
        purchased.push_back(gameName);
        if (fromWishlist) wishlist.remove(gameName);
        
        purchased.saveToFile(currentUsername + "_library.txt");
        wishlist.saveToFile(currentUsername + "_wishlist.txt");
        
        gameMenu.incrementSales(gameName);
        updatePublisherSalesFile(gameName);
        
        // Print receipt with payment method
        printPurchaseReceipt(gameName, originalPrice, finalPrice, 
                            willUseVoucher, payment.getPaymentMethodName());
        
        memberSys.addPurchaseAndMaybeGrantVoucher();
    } else {
        cout << "Pembelian dibatalkan.\n";
    }
}

void GameStore::updatePublisherSalesFile(const string& gameName) {
    GameNode* game = gameMenu.findGame(gameName);
    if (game == NULL || game->publisher == "Default") return;
    
    ifstream pubFile(PUBLISHER_FILE.c_str());
    if (!pubFile) return;
    
    string targetUsername = "";
    string line;
    while (getline(pubFile, line)) {
        size_t studioPos = 0;
        for (int i = 0; i < 14; i++) {
            studioPos = line.find('|', studioPos);
            if (studioPos == string::npos) break;
            studioPos++;
        }
        if (studioPos == string::npos) continue;
        
        size_t nextPos = line.find('|', studioPos);
        if (nextPos == string::npos) continue;
        string studioName = line.substr(studioPos, nextPos - studioPos);
        
        if (studioName == game->publisher) {
            targetUsername = line.substr(0, line.find('|'));
            break;
        }
    }
    pubFile.close();
    
    if (targetUsername.empty()) return;
    
    string gameFile = targetUsername + "_games.txt";
    ifstream gIn(gameFile.c_str());
    if (!gIn) return;
    
    string tempFile = targetUsername + "_games_temp.txt";
    ofstream gOut(tempFile.c_str());
    if (!gOut) {
        gIn.close();
        return;
    }
    
    string gLine;
    while (getline(gIn, gLine)) {
        size_t p = 0;
        size_t np = gLine.find('|', p);
        if (np == string::npos) continue;
        string gName = gLine.substr(p, np - p);
        
        if (gName == gameName) {
            p = np + 1;
            np = gLine.find('|', p);
            if (np == string::npos) continue;
            string priceStr = gLine.substr(p, np - p);
            p = np + 1;
            
            np = gLine.find('|', p);
            if (np == string::npos) continue;
            string cat = gLine.substr(p, np - p);
            p = np + 1;
            
            int sales = stringToInt(gLine.substr(p)) + 1;
            
            gOut << gName << "|" << priceStr << "|" << cat << "|" << sales << endl;
        } else {
            gOut << gLine << endl;
        }
    }
    gIn.close();
    gOut.close();
    
    remove(gameFile.c_str());
    rename(tempFile.c_str(), gameFile.c_str());
}

void GameStore::purchaseGameWrapper() {
    string gameName;
    cout << "\nKetik nama game yang ingin dibeli: ";
    cin.ignore(1000, '\n');
    getline(cin, gameName);
    purchaseGame(gameName);
}

void GameStore::addToWishlist(const string& gameName) {
    GameNode* game = gameMenu.findGame(gameName);
    if (game == NULL) {
        cout << "Game \"" << gameName << "\" tidak ditemukan di menu toko.\n";
        return;
    }

    if (purchased.exists(gameName)) {
        cout << "Game \"" << gameName << "\" sudah dibeli, tidak perlu di Wishlist.\n";
        return;
    }

    if (wishlist.exists(gameName)) {
        cout << "Game \"" << gameName << "\" sudah ada di Wishlist.\n";
        return;
    }

    wishlist.push_back(gameName);
    wishlist.saveToFile(currentUsername + "_wishlist.txt");
    cout << "Game \"" << gameName << "\" berhasil ditambahkan ke Wishlist!\n";
}

void GameStore::addToWishlistWrapper() {
    string gameName;
    cout << "\nKetik nama game yang ingin ditambahkan ke Wishlist: ";
    cin.ignore(1000, '\n');
    getline(cin, gameName);
    addToWishlist(gameName);
}

void GameStore::showWishlist() {
    cout << "\n=========================================" << endl;
    cout << "                  WISHLIST ANDA          " << endl;
    cout << "=========================================" << endl;

    if (wishlist.size() == 0) {
        cout << "Wishlist Anda kosong.\n";
        return;
    }

    wishlist.printList();
    cout << "\nMasukkan nomor game untuk Opsi (Beli/Hapus Wishlist) (0 untuk batal): ";
    int choice;
    if (!(cin >> choice)) {
        cout << "Pilihan tidak valid atau dibatalkan.\n";
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    if (choice <= 0) return;

    string gameToManage = wishlist.getAt(choice);
    if (gameToManage.empty()) {
        cout << "Pilihan tidak valid.\n";
        return;
    }

    cout << "\nOpsi untuk \"" << gameToManage << "\":\n";
    cout << "1. Beli Game\n2. Hapus dari Wishlist\n3. Batal\nPilih: ";
    int action;
    if (!(cin >> action)) {
        cout << "Input tidak valid.\n";
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    if (action == 1) {
        purchaseGame(gameToManage, true);
    } else if (action == 2) {
        if (wishlist.remove(gameToManage)) {
            wishlist.saveToFile(currentUsername + "_wishlist.txt");
            cout << "Game \"" << gameToManage << "\" berhasil dihapus dari Wishlist.\n";
        } else {
            cout << "Gagal menghapus dari wishlist.\n";
        }
    }
}

void GameStore::addDownloadedGame(const string& gameName) {
    if (!purchased.exists(gameName)) {
        cout << "Gagal mengunduh. Game \"" << gameName << "\" belum dibeli.\n";
        return;
    }
    if (downloaded.exists(gameName)) {
        cout << "Game \"" << gameName << "\" sudah ada di daftar unduhan.\n";
        return;
    }
    downloaded.push_back(gameName);
    downloaded.saveToFile(currentUsername + DOWNLOAD_FILE_SUFFIX);
    cout << "Game \"" << gameName << "\" berhasil diunduh dan siap dimainkan!\n";
}

void GameStore::addDownloadedGameWrapper() {
    string gameName;
    cout << "\nKetik nama game yang ingin diunduh (HARUS SUDAH DIBELI): ";
    cin.ignore(1000, '\n');
    getline(cin, gameName);
    addDownloadedGame(gameName);
}

void GameStore::deleteDownloadedGame() {
    if (downloaded.size() == 0) {
        cout << "\nDaftar unduhan kosong.\n";
        return;
    }

    cout << "\n=== DAFTAR GAME YANG DIUNDUH ===\n";
    downloaded.printList();
    cout << "Pilih nomor game untuk dihapus (0 untuk batal): ";
    int choice;
    if (!(cin >> choice)) {
        cout << "Input tidak valid.\n";
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }
    if (choice <= 0) return;
    string name = downloaded.getAt(choice);
    if (name.empty()) {
        cout << "Pilihan tidak valid.\n";
        return;
    }
    if (downloaded.remove(name)) {
        downloaded.saveToFile(currentUsername + DOWNLOAD_FILE_SUFFIX);
        cout << "Game berhasil dihapus dari daftar unduhan.\n";
    } else {
        cout << "Gagal menghapus.\n";
    }
}

void GameStore::showDownloadedGames() {
    cout << "\n================================" << endl;
    cout << "    DAFTAR GAME YANG DIUNDUH " << endl;
    cout << "================================" << endl;

    if (downloaded.size() == 0) {
        cout << "Tidak ada game yang diunduh.\n";
        return;
    }
    downloaded.printList();
}

void GameStore::rateGame() {
    if (downloaded.size() == 0) {
        cout << "\nAnda harus mengunduh game terlebih dahulu untuk memberikan rating.\n";
        return;
    }
    
    cout << "\n=== BERI RATING & REVIEW GAME ===\n" << endl;
    downloaded.printList();
    cout << "Pilih nomor game yang ingin diberi rating (0 untuk batal): ";
    int choice;
    if (!(cin >> choice)) {
        cout << "Input tidak valid.\n";
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }
    if (choice <= 0) return;
    
    string gameToRate = downloaded.getAt(choice);
    if (gameToRate.empty()) {
        cout << "Pilihan tidak valid.\n";
        return;
    }
    
    int currentRating = gameMenu.getUserRatingForGame(gameToRate, currentUsername);
    
    cout << "\nGame: " << gameToRate << endl;
    if (currentRating > 0) {
        cout << "Rating Anda saat ini: " << currentRating << "/5" << endl;
    } else {
        cout << "Anda belum memberikan rating untuk game ini." << endl;
    }
    
    // Get rating
    int newRating;
    cout << "\nMasukkan rating baru (1-5) (0 untuk batal): ";
    if (!(cin >> newRating)) {
        cout << "Input tidak valid.\n";
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    if (newRating < 0 || newRating > 5) {
        cout << "Rating tidak valid! Harus antara 1 sampai 5.\n";
        return;
    }

    if (newRating == 0) {
        cout << "Pemberian rating dibatalkan.\n";
        return;
    }
    
    // Get review text (NEW!)
    cin.ignore(1000, '\n');
    cout << "\n=== TULIS REVIEW (DENGAN HUFFMAN COMPRESSION) ===" << endl;
    cout << "Masukkan review Anda (tekan Enter 2x untuk selesai):" << endl;
    cout << "Review: ";
    
    string review;
    getline(cin, review);
    
    // Optional: allow multi-line (skip for simplicity)
    if (review.empty()) {
        cout << "Review dikosongkan (hanya rating yang akan disimpan).\n";
    }
    
    // Update game rating with review (will be compressed in GameRating)
    gameMenu.updateGameRatingWithReview(gameToRate, currentUsername, newRating, review);
    
    cout << "\n========================================" << endl;
    cout << "  RATING & REVIEW BERHASIL DISIMPAN!" << endl;
    cout << "========================================" << endl;
    cout << "Game: " << gameToRate << endl;
    cout << "Rating: " << newRating << "/5" << endl;
    if (!review.empty()) {
        cout << "Review: " << review.substr(0, 50) 
             << (review.length() > 50 ? "..." : "") << endl;
    }
    cout << "========================================\n" << endl;
}

void GameStore::showLibrary() {
    cout << "\n=========================================" << endl;
    cout << "                  LIBRARY ANDA           " << endl;
    cout << "=========================================" << endl;

    if (purchased.size() == 0) {
        cout << "Library Anda kosong.\n";
        return;
    }

    int total = purchased.size();
    cout << left << setw(4) << "No" << setw(40) << "Nama Game" << setw(15) << "Status" << setw(15) << "Rating Anda" << endl;
    cout << string(74, '-') << endl;

    for (int i = 1; i <= total; ++i) {
        string g = purchased.getAt(i);
        string status = downloaded.exists(g) ? "TERUNDUH" : "BELUM DIUNDUH";
        int userRating = gameMenu.getUserRatingForGame(g, currentUsername);
        string ratingDisplay = (userRating > 0) ? intToString(userRating) + "/5" : "N/A";
        
        cout << setw(4) << i << setw(40) << truncateString(g, 37) << setw(15) << status << setw(15) << ratingDisplay << endl;
    }

    cout << "\nMasukkan nomor game untuk Aksi (Unduh/Hapus Unduhan/Rating) (0 untuk batal): ";
    int choice;
    if (!(cin >> choice)) {
        cout << "Pilihan tidak valid atau dibatalkan.\n";
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    if (choice > 0 && choice <= purchased.size()) {
        string gameToManage = purchased.getAt(choice);
        bool isDownloaded = downloaded.exists(gameToManage);
        cout << "\nAksi untuk \"" << gameToManage << "\":\n";
        cout << "1. Unduh Game";
        if (isDownloaded) {
            cout << "\n2. Hapus Unduhan";
            cout << "\n3. Beri/Ubah Rating";
            cout << "\n4. Batal\nPilih: ";
        } else {
            cout << "\n2. Batal\nPilih: ";
        }

        int action;
        if (!(cin >> action)) {
            cout << "Input tidak valid.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            return;
        }

        if (isDownloaded) {
            if (action == 1) {
                addDownloadedGame(gameToManage);
            } else if (action == 2) {
                if (downloaded.remove(gameToManage)) {
                    downloaded.saveToFile(currentUsername + DOWNLOAD_FILE_SUFFIX);
                    cout << "Game \"" << gameToManage << "\" berhasil dihapus dari unduhan.\n";
                } else {
                    cout << "Game belum diunduh.\n";
                }
            } else if (action == 3) {
                int currentRating = gameMenu.getUserRatingForGame(gameToManage, currentUsername);
                cout << "\nGame: " << gameToManage << endl;
                if (currentRating > 0) {
                    cout << "Rating Anda saat ini: " << currentRating << "/5" << endl;
                } else {
                    cout << "Anda belum memberikan rating untuk game ini." << endl;
                }
                
                int newRating;
                cout << "Masukkan rating baru (1-5) (0 untuk batal): ";
                if (!(cin >> newRating)) {
                    cout << "Input tidak valid.\n";
                    cin.clear();
                    cin.ignore(1000, '\n');
                    return;
                }

                if (newRating >= 1 && newRating <= 5) {
                    gameMenu.updateGameRating(gameToManage, currentUsername, newRating);
                    cout << "\nAnda berhasil memberikan rating " << newRating << "/5 untuk game \"" << gameToManage << "\".\n";
                } else {
                    cout << "Pemberian rating dibatalkan/rating tidak valid.\n";
                }
            }
        } else {
            if (action == 1) {
                addDownloadedGame(gameToManage);
            } else if (action == 2) {
                cout << "Dibatalkan.\n";
            }
        }
    }
}

void GameStore::saveDownloadedGames() {
    downloaded.saveToFile(currentUsername + DOWNLOAD_FILE_SUFFIX);
}

void GameStore::savePurchasedGames() {
    purchased.saveToFile(currentUsername + "_library.txt");
}

void GameStore::saveWishlist() {
    wishlist.saveToFile(currentUsername + "_wishlist.txt");
}

void GameStore::addPublisherGame() {
    if (!isPublisherMode) {
        cout << "Akses ditolak. Hanya publisher yang dapat menambah game.\n";
        return;
    }

    string gameName, category;
    int price;
    
    cout << "\n================================" << endl;
    cout << "            TAMBAH GAME " << endl;
    cout << "================================" << endl;
    
    cin.ignore(1000, '\n');
    cout << "Nama Game       : ";
    getline(cin, gameName);
    gameName = trim(gameName);

    if (gameName.find('|') != string::npos) {
        cout << "Nama game tidak boleh mengandung karakter '|'!\n";
        return;
    }

    if (gameMenu.exists(gameName)) {
        cout << "Game dengan nama tersebut sudah ada di katalog publik!\n";
        return;
    }

    cout << "Harga (Rp)      : ";
    if (!(cin >> price)) {
        cout << "Input harga tidak valid!\n";
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    if (price < 0) {
        cout << "Harga tidak boleh negatif!\n";
        return;
    }

    cin.ignore(1000, '\n');
    cout << "Kategori        : ";
    getline(cin, category);
    category = trim(category);

    if (category.find('|') != string::npos) {
        cout << "Kategori tidak boleh mengandung karakter '|'!\n";
        return;
    }

    PendingGame newGame(gameName, price, category, currentUsername, publisherStudio);
    pendingGamesQueue.Enqueue(newGame);
    
    savePendingGamesToFile();

    cout << "\n================================" << endl;
    cout << "     GAME BERHASIL DIAJUKAN!" << endl;
    cout << "================================" << endl;
    cout << "Game \"" << gameName << "\" berhasil masuk ANTRIAN verifikasi!\n";
    cout << "Posisi di antrian: " << pendingGamesQueue.Size() << endl;
    cout << "Menunggu persetujuan dari Admin (FIFO - First In First Out).\n";
}

void GameStore::viewPublisherGames() {
    if (!isPublisherMode) {
        cout << "Akses ditolak.\n";
        return;
    }

    gameMenu.displayByPublisher(publisherStudio);
}

void GameStore::updatePublisherGame() {
    if (!isPublisherMode) {
        cout << "Akses ditolak.\n";
        return;
    }

    viewPublisherGames();
    
    cout << "\nMasukkan nama game yang ingin diupdate (ketik BATAL untuk batal): ";
    cin.ignore(1000, '\n');
    string gameName;
    getline(cin, gameName);
    gameName = trim(gameName);
    
    if (gameName == "BATAL" || gameName == "batal") {
        cout << "Dibatalkan.\n";
        return;
    }

    GameNode* game = gameMenu.findGame(gameName);
    if (game == NULL || game->publisher != publisherStudio) {
        cout << "Game tidak ditemukan atau bukan milik Anda.\n";
        return;
    }

    cout << "\n================================" << endl;
    cout << "            UPDATE GAME " << endl;
    cout << "================================" << endl;
    cout << "Game: " << gameName << endl;
    cout << "\n1. Update Nama" << endl;
    cout << "2. Update Harga" << endl;
    cout << "3. Update Kategori" << endl;
    cout << "4. Update Semua" << endl;
    cout << "0. Batal" << endl;
    cout << "Pilih: ";
    
    int choice;
    if (!(cin >> choice)) {
        cout << "Input tidak valid.\n";
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }
    cin.ignore(1000, '\n');

    if (choice == 0) {
        cout << "Dibatalkan.\n";
        return;
    }

    string oldName = game->name;
    string newName = game->name;
    int newPrice = game->price;
    string newCategory = game->category;

    if (choice == 1 || choice == 4) {
        cout << "Nama baru: ";
        getline(cin, newName);
        newName = trim(newName);
        
        if (newName.find('|') != string::npos || newName.empty()) {
            cout << "Nama tidak valid!\n";
            return;
        }
        if (newName != oldName && gameMenu.exists(newName)) {
            cout << "Nama game sudah digunakan!\n";
            return;
        }
    }

    if (choice == 2 || choice == 4) {
        cout << "Harga baru (Rp): ";
        if (!(cin >> newPrice)) {
            cout << "Input harga tidak valid!\n";
            cin.clear();
            cin.ignore(1000, '\n');
            return;
        }

        if (newPrice < 0) {
            cout << "Harga tidak valid!\n";
            return;
        }
        if (choice == 2) cin.ignore(1000, '\n');
    }

    if (choice == 3 || choice == 4) {
        if (choice == 4) {
            cin.ignore(1000, '\n');
        }
        cout << "Kategori baru: ";
        getline(cin, newCategory);
        newCategory = trim(newCategory);
        
        if (newCategory.find('|') != string::npos || newCategory.empty()) {
            cout << "Kategori tidak valid!\n";
            return;
        }
    }

    string gameFile = currentUsername + "_games.txt";
    string tempFile = currentUsername + "_games_temp.txt";
    
    ifstream gIn(gameFile.c_str());
    ofstream gOut(tempFile.c_str());
    
    if (!gIn || !gOut) {
        cout << "Error membuka file!\n";
        if (gIn) gIn.close();
        if (gOut) gOut.close();
        return;
    }

    string line;
    while (getline(gIn, line)) {
        size_t p = 0;
        size_t np = line.find('|', p);
        if (np == string::npos) continue;
        string gName = line.substr(p, np - p);
        
        if (gName == oldName) {
            for (int i = 0; i < 3; i++) {
                p = line.find('|', p) + 1;
            }
            int sales = stringToInt(line.substr(p));
            gOut << newName << "|" << newPrice << "|" << newCategory << "|" << sales << endl;
        } else {
            gOut << line << endl;
        }
    }
    
    gIn.close();
    gOut.close();

    remove(gameFile.c_str());
    rename(tempFile.c_str(), gameFile.c_str());

    gameMenu.updateGame(oldName, newName, newPrice, newCategory);

    cout << "Game berhasil diupdate!\n";
}

void GameStore::viewPublisherStatistics() {
    if (!isPublisherMode) {
        cout << "Akses ditolak.\n";
        return;
    }

    cout << "\n================================" << endl;
    cout << "      STATISTIK PENJUALAN " << endl;
    cout << "================================" << endl;
    cout << "Publisher: " << publisherStudio << endl;
    cout << "================================" << endl;

    ifstream gFile((currentUsername + "_games.txt").c_str());
    if (!gFile) {
        cout << "Belum ada game yang diupload.\n";
        return;
    }

    int totalSales = 0;
    long long totalRevenue = 0;
    int gameCount = 0;
    string bestGame = "";
    int maxSales = 0;

    cout << left << setw(4) << "No"
         << setw(30) << " Nama Game"
         << setw(12) << " Harga"
         << setw(10) << " Terjual"
         << setw(15) << " Revenue"
         << setw(15) << " Rating" << endl;
    cout << string(96, '-') << endl;

    string line;
    int index = 1;
    while (getline(gFile, line)) {
        size_t p = 0;
        size_t np;
        
        np = line.find('|', p);
        if (np == string::npos) continue;
        string gName = line.substr(p, np - p);
        p = np + 1;
        
        np = line.find('|', p);
        if (np == string::npos) continue;
        int price = stringToInt(line.substr(p, np - p));
        p = np + 1;
        
        np = line.find('|', p);
        if (np == string::npos) continue;
        string cat = line.substr(p, np - p);
        p = np + 1;
        
        int sales = stringToInt(line.substr(p));
        
        long long revenue = (long long)price * sales;
        totalSales += sales;
        totalRevenue += revenue;
        gameCount++;
        
        if (sales > maxSales) {
            maxSales = sales;
            bestGame = gName;
        }
        
        GameNode* game = gameMenu.findGame(gName);
        string ratingDisplay = (game != NULL) ? game->getRatingDisplay() : "N/A";

        cout << setw(4) << index++
             << setw(30) << truncateString(gName, 27)
             << setw(12) << price
             << setw(10) << sales
             << setw(15) << revenue
             << setw(20) << ratingDisplay << endl;
    }
    gFile.close();

    cout << string(96, '=') << endl;
    cout << "Total Game Diupload    : " << gameCount << endl;
    cout << "Total Game Terjual     : " << totalSales << endl;
    cout << "Total Revenue          : Rp " << totalRevenue << endl;
    
    if (gameCount > 0) {
        double avgSales = (double)totalSales / gameCount;
        cout << "Rata-rata Penjualan    : " << fixed << setprecision(2) << avgSales << " per game\n";
    }

    if (totalSales > 0) {
        cout << "\nGame Terlaris          : " << bestGame << " (" << maxSales << " terjual)\n";
    }
}

void GameStore::showMenuAdvanced(const string& filter, int sortOption, 
                                 int priceFilter, int freeFilter) {
    GameNode* dummyGames[100];
    int dummyCount = 0;
    gameMenu.displayAllSorted(filter, dummyGames, dummyCount, sortOption, priceFilter, freeFilter);
}

void GameStore::showAdvancedFilterMenu() {
    cout << "\n=== ADVANCED FILTER & SORT ===" << endl;
    
    string filterChoice;
    cout << "\nFilter kategori (ketik 'All' atau nama kategori): ";
    cin.ignore(1000, '\n');
    getline(cin, filterChoice);
    if (filterChoice.empty()) filterChoice = "All";
    
    int sortOption = 0;
    cout << "\nSort by:\n0. Default\n1. Harga (Rendah-Tinggi)\n2. Nama (A-Z)\n3. Paling Populer\n4. Rating Tertinggi\nPilih: ";
    if (!(cin >> sortOption)) {
        cin.clear();
        cin.ignore(1000, '\n');
        sortOption = 0;
    }
    
    int priceFilter = 0;
    cout << "\nFilter harga:\n0. Semua\n1. < 50k\n2. 50k-100k\n3. > 100k\nPilih: ";
    if (!(cin >> priceFilter)) {
        cin.clear();
        cin.ignore(1000, '\n');
        priceFilter = 0;
    }
    
    int freeFilter = 0;
    cout << "\nTipe game:\n0. Semua\n1. Free\n2. Paid\nPilih: ";
    if (!(cin >> freeFilter)) {
        cin.clear();
        cin.ignore(1000, '\n');
        freeFilter = 0;
    }
    
    GameNode* games[100];
    int gameCount = 0;
    gameMenu.displayAllSorted(filterChoice, games, gameCount, sortOption, priceFilter, freeFilter);
    
    if (gameCount > 0) {
        cout << "\n\nPilih nomor game untuk aksi cepat (0 untuk kembali): ";
        int gameChoice;
        if (cin >> gameChoice && gameChoice > 0 && gameChoice <= gameCount) {
            showQuickActionMenu(games[gameChoice - 1]->name);
        } else {
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}


void GameStore::viewBrowsingHistory() {
    browsingHistory.showHistory();
    
    if (browsingHistory.isEmpty()) return;
    
    cout << "\nPilihan:\n";
    cout << "1. Lihat Detail Game dari History\n";
    cout << "2. Clear History\n";
    cout << "0. Kembali\n";
    cout << "Pilih: ";
    
    int choice;
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }
    
    if (choice == 1) {
        cout << "Masukkan nomor game: ";
        int num;
        if (!(cin >> num)) {
            cin.clear();
            cin.ignore(1000, '\n');
            return;
        }
        
        string gameName = browsingHistory.getAt(num);
        if (!gameName.empty()) {
            viewGameDetail(gameName);
        } else {
            cout << "Nomor tidak valid.\n";
        }
    } else if (choice == 2) {
        browsingHistory.clear();
        browsingHistory.saveToFile(currentUsername + "_history.txt");
        cout << "History berhasil dihapus!\n";
    }
}

void GameStore::viewGameDetail(const string& gameName) {
    GameNode* game = gameMenu.findGame(gameName);
    if (game == NULL) {
        cout << "Game tidak ditemukan.\n";
        return;
    }
    
    cout << "\n================================" << endl;
    cout << "       DETAIL GAME" << endl;
    cout << "================================" << endl;
    cout << "Nama Game  : " << game->name << endl;
    cout << "Harga      : Rp " << game->price << endl;
    cout << "Kategori   : " << game->category << endl;
    cout << "Publisher  : " << game->publisher << endl;
    cout << "Rating     : " << game->getRatingDisplay() << endl;
    cout << "Terjual    : " << game->salesCount << " kali" << endl;
    cout << "================================" << endl;
    
    browsingHistory.pushGame(gameName);
    browsingHistory.saveToFile(currentUsername + "_history.txt");
}

void GameStore::showQuickActionMenu(const string& gameName) {
    cout << "\n=== AKSI CEPAT: " << gameName << " ===" << endl;
    cout << "1. Beli Game" << endl;
    cout << "2. Tambah ke Wishlist" << endl;
    cout << "3. Lihat Detail" << endl;
    cout << "0. Batal" << endl;
    cout << "Pilih: ";
    
    int action;
    if (!(cin >> action)) {
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }
    
    switch(action) {
        case 1: 
            purchaseGame(gameName); 
            break;
        case 2: 
            addToWishlist(gameName); 
            break;
        case 3: 
            viewGameDetail(gameName); 
            break;
        default: 
            cout << "Pilihan tidak valid!\n";
    }
}

void GameStore::printPurchaseReceipt(const string& gameName, int originalPrice, 
                                     int finalPrice, bool usedVoucher,
                                     const string& paymentMethod) {
    GameNode* game = gameMenu.findGame(gameName);
    
    // Get current timestamp
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    
    char dateStr[80];
    strftime(dateStr, sizeof(dateStr), "%d/%m/%Y %H:%M:%S", timeinfo);
    
    // Generate transaction ID
    TransactionCounter txCounter;
    string receiptID = txCounter.generateTransactionID();
    
    stringstream receipt;
    
    receipt << "\n";
    receipt << "========================================\n";
    receipt << "         STRUK PEMBELIAN GAME\n";
    receipt << "            GAME STORE\n";
    receipt << "========================================\n";
    receipt << "No. Transaksi : " << receiptID << "\n";
    receipt << "Tanggal/Waktu : " << dateStr << "\n";
    receipt << "Username      : " << currentUsername << "\n";
    receipt << "========================================\n";
    receipt << "DETAIL PEMBELIAN:\n";
    receipt << "  Game        : " << gameName << "\n";
    
    if (game) {
        receipt << "  Kategori    : " << game->category << "\n";
        receipt << "  Publisher   : " << game->publisher << "\n";
    }
    
    receipt << "----------------------------------------\n";
    receipt << "  Harga Asli  : Rp " << originalPrice << "\n";
    
    if (usedVoucher) {
        int discount = originalPrice - finalPrice;
        receipt << "  Diskon 30%  : -Rp " << discount << "\n";
        receipt << "  --------------------\n";
    }
    
    receipt << "  TOTAL BAYAR : Rp " << finalPrice << "\n";
    receipt << "========================================\n";
    receipt << "METODE PEMBAYARAN:\n";
    receipt << "  " << paymentMethod << "\n";
    receipt << "========================================\n";
    
    if (usedVoucher) {
        receipt << "* Voucher member telah digunakan\n";
    }
    
    if (memberSys.hasVoucher()) {
        receipt << "* Anda masih memiliki voucher tersisa!\n";
    }
    
    receipt << "\n";
    receipt << "  Terima kasih atas pembelian Anda!\n";
    receipt << "  Game dapat diunduh dari menu Library\n";
    receipt << "\n";
    receipt << "========================================\n";
    
    string receiptStr = receipt.str();
    cout << receiptStr;
    
    // Save to file
    saveReceiptToFile(receiptStr);
}

void GameStore::saveReceiptToFile(const string& receiptContent) {
    string filename = currentUsername + "_receipts.txt";
    ofstream file(filename.c_str(), ios::app);
    if (file) {
        file << receiptContent << "\n\n";
        file.close();
        cout << "* Struk telah disimpan di: " << filename << "\n" << endl;
    }
}

void GameStore::saveBrowsingHistory() {
    browsingHistory.saveToFile(currentUsername + "_history.txt");
}

// Lihat history struk pembelian
void GameStore::viewReceiptHistory() {
    string filename = currentUsername + "_receipts.txt";
    ifstream file(filename.c_str());
    
    cout << "\n========================================" << endl;
    cout << "         HISTORY STRUK PEMBELIAN        " << endl;
    cout << "========================================" << endl;
    
    if (!file) {
        cout << "Belum ada riwayat pembelian." << endl;
        cout << "File: " << filename << " tidak ditemukan." << endl;
        return;
    }
    
    string line;
    bool hasContent = false;
    
    while (getline(file, line)) {
        cout << line << endl;
        hasContent = true;
    }
    
    file.close();
    
    if (!hasContent) {
        cout << "Belum ada riwayat pembelian." << endl;
    } else {
        cout << "\n========================================" << endl;
        cout << "           AKHIR HISTORY STRUK          " << endl;
        cout << "========================================" << endl;
    }
}

#endif
