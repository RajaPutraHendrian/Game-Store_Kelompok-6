#ifndef MenuPublisher_
#define MenuPublisher_

#include <iostream>
#include "GameStore.h"
#include "UserSystem.h"
using namespace std;

class GameStore;
class UserSystem;

void menuPublisher(GameStore &store, UserSystem &userSys) {
    store.checkRejectionNotifications();
    
    int choice;
    do {
        cout << "\n\n=== DASHBOARD PUBLISHER ===";
        cout << "\nStudio: " << userSys.getPublisherStudio();
        cout << "\n1. Tambah Game Baru (Ajukan untuk Verifikasi)";
        cout << "\n2. Lihat Daftar Game Saya (Yang Sudah Disetujui)";
        cout << "\n3. Lihat Game Menunggu Verifikasi";
        cout << "\n4. Update Data Game";
        cout << "\n5. Lihat Statistik Penjualan & Rating";
        cout << "\n6. Browse All Games (Lihat Semua Game)";
        cout << "\n7. Logout";
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
            case 1: store.addPublisherGame(); break;
            case 2: store.viewPublisherGames(); break;
            case 3: store.viewPendingGamesByPublisher(); break;
            case 4: store.updatePublisherGame(); break;
            case 5: store.viewPublisherStatistics(); break;
            case 6: {
                cout << "\n1. Browse Normal\n2. Browse dengan Advanced Filter\nPilih: ";
                int browseChoice;
                if (!(cin >> browseChoice)) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    break;
                }
                if (browseChoice == 2) {
                    store.showAdvancedFilterMenu();
                } else {
                    GameNode* dummyGames[100];
                    int dummyCount = 0;
                    store.showMenu("All", dummyGames, dummyCount);
                }
                break;
            }
            case 7:
                userSys.logout();
                return;
            case 0:
                cout << "Keluar dari program.\n";
                break;
            default: cout << "Pilihan tidak valid!\n";
        }
    } while (choice != 0);
}

#endif
