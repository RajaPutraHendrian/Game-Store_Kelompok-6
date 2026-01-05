#ifndef MenuUser_
#define MenuUser_

#include <iostream>
#include "GameStore.h"
#include "UserSystem.h"
using namespace std;

void menuWishlist(GameStore &store) {
    int choice;
    do {
        cout << "\n=== MENU WISHLIST ===";
        cout << "\n1. Tambah Game ke Wishlist";
        cout << "\n2. Lihat & Kelola Wishlist";
        cout << "\n0. Kembali";
        cout << "\nPilih: ";

        if (!(cin >> choice)) {
            cout << "Input tidak valid.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            choice = -1;
            continue;
        }

        switch (choice) {
            case 1: store.addToWishlistWrapper(); break;
            case 2: store.showWishlist(); break;
            case 0: break;
            default: cout << "Pilihan tidak valid!\n";
        }
    } while (choice != 0);
}

void menuLibrary(GameStore &store) {
    int choice;
    do {
        cout << "\n=== MENU LIBRARY ===";
        cout << "\n1. Beli Game";
        cout << "\n2. Lihat & Kelola Library/Unduhan/Rating";
        cout << "\n3. Beri Rating Game (Akses Cepat)";
        cout << "\n0. Kembali";
        cout << "\nPilih: ";

        if (!(cin >> choice)) {
            cout << "Input tidak valid.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            choice = -1;
            continue;
        }

        switch (choice) {
            case 1: store.purchaseGameWrapper(); break;
            case 2: store.showLibrary(); break;
            case 3: store.rateGame(); break;
            case 0: break;
            default: cout << "Pilihan tidak valid!\n";
        }
    } while (choice != 0);
}

void menuGameStore(GameStore &store, UserSystem &userSys) {
    int choice;
    do {
        cout << "\n\n=== MENU TOKO GAME (USER) ===";
        cout << "\nUser: " << userSys.getCurrentUser();
        cout << "\n1. Tampilkan semua game";
        cout << "\n2. Search Game";
        cout << "\n3. Filter & Sort";
        cout << "\n4. Library (Game yang Sudah Dibeli/Unduhan/Rating)";
        cout << "\n5. Wishlist (Game yang Diinginkan)";
        cout << "\n6. Riwayat Browsing (History)";
        cout << "\n7. Lihat History Struk (Riwayat Pembelian)";
        cout << "\n8. Logout";
        cout << "\n0. Keluar Program";
        cout << "\nPilih: ";

        if (!(cin >> choice)) {
            cout << "Input tidak valid.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            choice = -1;
            continue;
        }

        switch (choice) {
            case 1: {
                GameNode* games[10000];
                int gameCount = 0;
                store.showMenu("All", games, gameCount);
                
                if (gameCount > 0) {
                    cout << "\n\nPilih nomor game untuk aksi cepat (0 untuk kembali): ";
                    int gameChoice;
                    if (cin >> gameChoice && gameChoice > 0 && gameChoice <= gameCount) {
                        store.showQuickActionMenu(games[gameChoice - 1]->name);
                    } else {
                        if (cin.fail()) {
                            cout << "\n⚠️ Input tidak valid! Harap masukkan angka.\n";
                        } else if (gameChoice > gameCount) {
                            cout << "\n⚠️ Nomor game " << gameChoice << " tidak ditemukan. ";
                            cout << "Maksimal nomor yang tersedia: " << gameCount << "\n";
                        }
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Tekan Enter untuk melanjutkan...";
                        cin.get();
                    }
                }
                break;
            }
            case 2: store.searchGame(); break;
            case 3: store.showAdvancedFilterMenu(); break;
            case 4: menuLibrary(store); break;
            case 5: menuWishlist(store); break;
            case 6: store.viewBrowsingHistory(); break;
            case 7: store.viewReceiptHistory(); break;
            case 8:
                store.saveDownloadedGames();
                store.savePurchasedGames();
                store.saveWishlist();
                store.saveBrowsingHistory();
                store.clearUserData();
                userSys.logout();
                return;
            case 0:
                store.saveDownloadedGames();
                store.savePurchasedGames();
                store.saveWishlist();
                store.saveBrowsingHistory();
                cout << "Keluar dari program.\n";
                break;
            default: cout << "Pilihan tidak valid!\n";
        }
    } while (choice != 0);
}


#endif
