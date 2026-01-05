#include <iostream>
#include "UserSystem.h"
#include "GameStore.h"
#include "MenuUser.h"
#include "MenuPublisher.h"
#include "MenuAdmin.h"
using namespace std;

int main() {
	system("color 03");
    UserSystem userSys;
    GameStore store;
    
    store.loadSampleMenu();
    store.loadAllPublisherGames();

    int choice;
    do {
        cout << "   _____          __  __ ______    _____ _______ ____  _____  ______ " << endl;
		cout << "  / ____|   /\\   |  \\/  |  ____|  / ____|__   __/ __ \\|  __ \\|  ____|" << endl;
		cout << " | |  __   /  \\  | \\  / | |__    | (___    | | | |  | | |__) | |__   " << endl;
		cout << " | | |_ | / /\\ \\ | |\\/| |  __|    \\___ \\   | | | |  | |  _  /|  __|  " << endl;
		cout << " | |__| |/ ____ \\| |  | | |____   ____) |  | | | |__| | | \\ \\| |____ " << endl;
		cout << "  \\_____/_/    \\_\\_|  |_|______| |_____/   |_|  \\____/|_|  \\_\\______|" << endl;
		cout << endl;
		cout << "=========================" << endl;
        cout << "1. Registrasi Akun" << endl;
        cout << "2. Login Akun" << endl;
        cout << "3. Keluar" << endl;
        cout << "Pilih: ";

        if (!(cin >> choice)) {
            cout << "Input tidak valid.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            choice = -1;
            continue;
        }

        switch (choice) {
            case 1: {
                int roleChoice;
                cout << "\n================================" << endl;
                cout << "          PILIH ROLE AKUN" << endl;
                cout << "================================" << endl;
                cout << "1. User (Pembeli Game)" << endl;
                cout << "2. Publisher (Penerbit Game)" << endl;
                cout << "Pilih: ";
                if (!(cin >> roleChoice)) {
                    cout << "Input tidak valid.\n";
                    cin.clear();
                    cin.ignore(1000, '\n');
                    roleChoice = -1;
                }

                if (roleChoice == 1) {
                    userSys.registerUser();
                } else if (roleChoice == 2) {
                    userSys.registerPublisher();
                } else {
                    cout << "Pilihan tidak valid!\n";
                }
                break;
            }
            case 2:
                if (userSys.loginUser()) {
                    if (userSys.getIsAdmin()) {
                        menuAdmin(store, userSys);
                    } else if (userSys.getIsPublisher()) {
                        store.setUser(userSys.getCurrentUser(), true, userSys.getPublisherStudio());
                        menuPublisher(store, userSys);
                    } else {
                        store.setUser(userSys.getCurrentUser(), false);
                        menuGameStore(store, userSys);
                    }
                }
                break;
            case 3:
                cout << "Keluar dari program.\n";
                break;
            default:
                cout << "Pilihan tidak valid!\n";
        }
    } while (choice != 3);

    return 0;
}
