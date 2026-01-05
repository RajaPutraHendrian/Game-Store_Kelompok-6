#ifndef MenuAdmin_
#define MenuAdmin_

#include <iostream>
#include "GameStore.h"
#include "UserSystem.h"
using namespace std;

class GameStore;
class UserSystem;

void menuAdmin(GameStore &store, UserSystem &userSys);


void menuAdmin(GameStore &store, UserSystem &userSys) {
    int choice;
    do {
        cout << "\n\n=== DASHBOARD ADMIN ===";
        cout << "\nAdmin: " << userSys.getCurrentUser();
        cout << "\n1. Lihat Game Menunggu Verifikasi";
        cout << "\n2. Verifikasi Game (Setujui/Tolak)";
        cout << "\n3. Browse All Games";
        cout << "\n4. Lihat Daftar Semua User";
        cout << "\n5. Lihat Daftar Semua Publisher";
        cout << "\n6. Lihat Publisher Ranking (AVL Tree)";
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
            case 1: 
                store.viewPendingGames(); 
                break;
            case 2: 
                store.verifyGameProcess(); 
                break;
            case 3: 
            {
                GameNode* dummyGames[100];
                int dummyCount = 0;
                store.showMenu("All", dummyGames, dummyCount); 
                break;
            }
            case 4:
                userSys.listAllUsers();
                break;
            case 5:
                userSys.listAllPublishers();
                break;
            case 6:
                store.viewPublisherRanking();
                break;
            case 7:
                userSys.logout();
                return;
            case 0:
                cout << "Keluar dari program.\n";
                break;
            default: 
                cout << "Pilihan tidak valid!\n";
        }
    } while (choice != 0);
}
#endif
