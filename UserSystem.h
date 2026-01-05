#ifndef UserSystem_
#define UserSystem_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "User.h"
#include "Constants.h"
#include "doublelinklist.h"
using namespace std;

class UserSystem {
private:
    DoublyCircularLinkedList<User> users;           // DCLL for users!
    DoublyCircularLinkedList<Publisher> publishers;  // DCLL for publishers!
    string currentUser;  // Stores NIK
    bool isLoggedIn;
    bool isPublisher;
    bool isAdmin;

    void loadUsers();
    void loadPublishers();
    void saveAllUsers();
    void saveAllPublishers();
    void saveUserToFile(const User& u);
    void savePublisherToFile(const Publisher& p);
    bool nikExistsInBoth(const string& nik);
    string getPasswordWithConfirm();

public:
    UserSystem();
    
    void registerUser();
    void registerPublisher();
    void forgotPasswordUser();
    void forgotPasswordPublisher();
    bool loginUser();
    void logout();
    
    bool checkLoggedIn();
    string getCurrentUser();  // Returns NIK
    bool getIsPublisher();
    bool getIsAdmin();
    string getPublisherStudio();
    
    // Admin: Lihat daftar semua user dan publisher
    void listAllUsers();
    void listAllPublishers();
};

UserSystem::UserSystem() {
    currentUser = "";
    isLoggedIn = false;
    isPublisher = false;
    isAdmin = false;
    loadUsers();
    loadPublishers();
}

string UserSystem::getPasswordWithConfirm() {
    string pwd1, pwd2;
    cout << "Password: ";
    cin >> pwd1;
    cout << "Konfirmasi Password: ";
    cin >> pwd2;
    
    if (pwd1 != pwd2) {
        cout << "Password tidak cocok!" << endl;
        return "";
    }
    if (pwd1.find('|') != string::npos) {
        cout << "Password tidak boleh mengandung '|'!" << endl;
        return "";
    }
    return pwd1;
}

void UserSystem::loadUsers() {
    ifstream file(USER_FILE.c_str());
    if (!file) return;
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string nik, nama, alamat, password, email;
        int hari, bulan, tahun, jenisKelamin, golDarah, agama, statusNikah, kewarganegaraan;
        string pekerjaan;
        
        // Parse format: NIK|Nama|Alamat|DD|MM|YYYY|JK|GD|Agama|Status|Pekerjaan|Kewarganegaraan|Password|Email
        getline(ss, nik, '|');
        getline(ss, nama, '|');
        getline(ss, alamat, '|');
        ss >> hari;
        ss.ignore();
        ss >> bulan;
        ss.ignore();
        ss >> tahun;
        ss.ignore();
        ss >> jenisKelamin;
        ss.ignore();
        ss >> golDarah;
        ss.ignore();
        ss >> agama;
        ss.ignore();
        ss >> statusNikah;
        ss.ignore();
        getline(ss, pekerjaan, '|');
        ss >> kewarganegaraan;
        ss.ignore();
        getline(ss, password, '|');
        getline(ss, email);
        
        // Reconstruct KTP
        Date tanggalLahir;
        tanggalLahir.hari = hari;
        tanggalLahir.bulan = bulan;
        tanggalLahir.tahun = tahun;
        
        KTP ktp(nik, nama, alamat, tanggalLahir,
                (bool)jenisKelamin, (GolonganDarah)golDarah,
                (Agama)agama, (statusNikah == 1), pekerjaan,
                (Kewarganegaraan)kewarganegaraan);
        
        User newUser;
        newUser.ktpData = ktp;
        newUser.password = password;
        newUser.email = email;
        users.InsertBack(newUser);  // DLL insert!
    }
    file.close();
}

void UserSystem::loadPublishers() {
    ifstream file(PUBLISHER_FILE.c_str());
    if (!file) return;
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string nik, nama, alamat, password, email, namaStudio, negara, tahunBerdiri;
        int hari, bulan, tahun, jenisKelamin, golDarah, agama, statusNikah, kewarganegaraan;
        string pekerjaan;
        
        // Parse KTP + Publisher info
        getline(ss, nik, '|');
        getline(ss, nama, '|');
        getline(ss, alamat, '|');
        ss >> hari;
        ss.ignore();
        ss >> bulan;
        ss.ignore();
        ss >> tahun;
        ss.ignore();
        ss >> jenisKelamin;
        ss.ignore();
        ss >> golDarah;
        ss.ignore();
        ss >> agama;
        ss.ignore();
        ss >> statusNikah;
        ss.ignore();
        getline(ss, pekerjaan, '|');
        ss >> kewarganegaraan;
        ss.ignore();
        getline(ss, password, '|');
        getline(ss, email, '|');
        getline(ss, namaStudio, '|');
        getline(ss, negara, '|');
        getline(ss, tahunBerdiri);
        
        // Reconstruct
        Date tanggalLahir;
        tanggalLahir.hari = hari;
        tanggalLahir.bulan = bulan;
        tanggalLahir.tahun = tahun;
        
        KTP ktp(nik, nama, alamat, tanggalLahir,
                (bool)jenisKelamin, (GolonganDarah)golDarah,
                (Agama)agama, (statusNikah == 1), pekerjaan,
                (Kewarganegaraan)kewarganegaraan);
        
        Publisher newPub;
        newPub.ktpData = ktp;
        newPub.password = password;
        newPub.email = email;
        newPub.namaStudio = namaStudio;
        newPub.negara = negara;
        newPub.tahunBerdiri = tahunBerdiri;
        publishers.InsertBack(newPub);  // DLL insert!
    }
    file.close();
}

void UserSystem::saveAllUsers() {
    ofstream file(USER_FILE.c_str());
    if (!file) return;
    
    // Traverse DLL and save all users
    int len = users.Length();
    for (int i = 0; i < len; i++) {
        const User& u = users.Get(i);
        const KTP& ktp = u.ktpData;
        file << ktp.getNIK() << "|"
             << ktp.getNama() << "|"
             << ktp.getAlamat() << "|"
             << ktp.getTanggalLahir().hari << "|"
             << ktp.getTanggalLahir().bulan << "|"
             << ktp.getTanggalLahir().tahun << "|"
             << (int)ktp.getJenisKelamin() << "|"
             << (int)ktp.getGolonganDarah() << "|"
             << (int)ktp.getAgama() << "|"
             << (ktp.getStatusPerkawinan() ? 1 : 0) << "|"
             << ktp.getPekerjaan() << "|"
             << (int)ktp.getKewarganegaraan() << "|"
             << u.password << "|"
             << u.email << endl;
    }
    file.close();
}

void UserSystem::saveAllPublishers() {
    ofstream file(PUBLISHER_FILE.c_str());
    if (!file) return;
    
    // Traverse DLL and save all publishers
    int len = publishers.Length();
    for (int i = 0; i < len; i++) {
        const Publisher& p = publishers.Get(i);
        const KTP& ktp = p.ktpData;
        file << ktp.getNIK() << "|"
             << ktp.getNama() << "|"
             << ktp.getAlamat() << "|"
             << ktp.getTanggalLahir().hari << "|"
             << ktp.getTanggalLahir().bulan << "|"
             << ktp.getTanggalLahir().tahun << "|"
             << (int)ktp.getJenisKelamin() << "|"
             << (int)ktp.getGolonganDarah() << "|"
             << (int)ktp.getAgama() << "|"
             << (ktp.getStatusPerkawinan() ? 1 : 0) << "|"
             << ktp.getPekerjaan() << "|"
             << (int)ktp.getKewarganegaraan() << "|"
             << p.password << "|"
             << p.email << "|"
             << p.namaStudio << "|"
             << p.negara << "|"
             << p.tahunBerdiri << endl;
    }
    file.close();
}

void UserSystem::saveUserToFile(const User& u) {
    ofstream file(USER_FILE.c_str(), ios::app);
    if (!file) return;
    
    const KTP& ktp = u.ktpData;
    file << ktp.getNIK() << "|"
         << ktp.getNama() << "|"
         << ktp.getAlamat() << "|"
         << ktp.getTanggalLahir().hari << "|"
         << ktp.getTanggalLahir().bulan << "|"
         << ktp.getTanggalLahir().tahun << "|"
         << (int)ktp.getJenisKelamin() << "|"
         << (int)ktp.getGolonganDarah() << "|"
         << (int)ktp.getAgama() << "|"
         << (ktp.getStatusPerkawinan() ? 1 : 0) << "|"
         << ktp.getPekerjaan() << "|"
         << (int)ktp.getKewarganegaraan() << "|"
         << u.password << "|"
         << u.email << endl;
    file.close();
}

void UserSystem::savePublisherToFile(const Publisher& p) {
    ofstream file(PUBLISHER_FILE.c_str(), ios::app);
    if (!file) return;
    
    const KTP& ktp = p.ktpData;
    file << ktp.getNIK() << "|"
         << ktp.getNama() << "|"
         << ktp.getAlamat() << "|"
         << ktp.getTanggalLahir().hari << "|"
         << ktp.getTanggalLahir().bulan << "|"
         << ktp.getTanggalLahir().tahun << "|"
         << (int)ktp.getJenisKelamin() << "|"
         << (int)ktp.getGolonganDarah() << "|"
         << (int)ktp.getAgama() << "|"
         << (ktp.getStatusPerkawinan() ? 1 : 0) << "|"
         << ktp.getPekerjaan() << "|"
         << (int)ktp.getKewarganegaraan() << "|"
         << p.password << "|"
         << p.email << "|"
         << p.namaStudio << "|"
         << p.negara << "|"
         << p.tahunBerdiri << endl;
    file.close();
}

bool UserSystem::nikExistsInBoth(const string& nik) {
    // Check in users DLL
    int userLen = users.Length();
    for (int i = 0; i < userLen; i++) {
        if (users.Get(i).ktpData.getNIK() == nik)
            return true;
    }
    
    // Check in publishers DLL
    int pubLen = publishers.Length();
    for (int i = 0; i < pubLen; i++) {
        if (publishers.Get(i).ktpData.getNIK() == nik)
            return true;
    }
    
    return false;
}

void UserSystem::registerUser() {
    User newUser;
    cout << "\n================================" << endl;
    cout << "        REGISTER AKUN USER " << endl;
    cout << "================================" << endl;
    cout << "\n=== DATA KTP ===" << endl;
    
    cin >> newUser.ktpData;
    
    if (nikExistsInBoth(newUser.ktpData.getNIK())) {
        cout << "NIK sudah terdaftar!" << endl;
        return;
    }
    
    cout << "\n=== AUTENTIKASI ===" << endl;
    newUser.password = getPasswordWithConfirm();
    if (newUser.password.empty()) {
        cout << "Registrasi dibatalkan." << endl;
        return;
    }
    
    cout << "Email (optional, tekan Enter untuk skip): ";
    cin.ignore();
    getline(cin, newUser.email);
    
    users.InsertBack(newUser);  // DLL insert!
    saveUserToFile(newUser);
    
    cout << "\n================================" << endl;
    cout << "  Akun User berhasil dibuat!" << endl;
    cout << "  NIK: " << newUser.ktpData.getNIK() << endl;
    cout << "================================" << endl;
}

void UserSystem::registerPublisher() {
    Publisher newPublisher;
    cout << "\n================================" << endl;
    cout << "     REGISTER AKUN PUBLISHER " << endl;
    cout << "================================" << endl;
    cout << "\n=== DATA KTP ===" << endl;
    
    cin >> newPublisher.ktpData;
    
    if (nikExistsInBoth(newPublisher.ktpData.getNIK())) {
        cout << "NIK sudah terdaftar!" << endl;
        return;
    }
    
    cout << "\n=== AUTENTIKASI ===" << endl;
    newPublisher.password = getPasswordWithConfirm();
    if (newPublisher.password.empty()) {
        cout << "Registrasi dibatalkan." << endl;
        return;
    }
    
    cout << "Email (optional): ";
    cin.ignore();
    getline(cin, newPublisher.email);
    
    cout << "\n=== DATA PUBLISHER ===" << endl;
    cout << "Masukkan nama studio: ";
    getline(cin, newPublisher.namaStudio);
    if (newPublisher.namaStudio.find('|') != string::npos) {
        cout << "Nama studio tidak boleh mengandung '|'!" << endl;
        return;
    }
    
    cout << "Masukkan negara: ";
    getline(cin, newPublisher.negara);
    if (newPublisher.negara.find('|') != string::npos) {
        cout << "Negara tidak boleh mengandung '|'!" << endl;
        return;
    }
    
    cout << "Masukkan tahun berdiri: ";
    getline(cin, newPublisher.tahunBerdiri);
    if (newPublisher.tahunBerdiri.find('|') != string::npos) {
        cout << "Tahun berdiri tidak boleh mengandung '|'!" << endl;
        return;
    }
    
    publishers.InsertBack(newPublisher);  // DLL insert!
    savePublisherToFile(newPublisher);
    
    cout << "\n================================" << endl;
    cout << "  Akun Publisher berhasil dibuat!" << endl;
    cout << "  NIK: " << newPublisher.ktpData.getNIK() << endl;
    cout << "  Studio: " << newPublisher.namaStudio << endl;
    cout << "================================" << endl;
}

void UserSystem::forgotPasswordUser() {
    cout << "\n================================" << endl;
    cout << "      LUPA PASSWORD (USER) " << endl;
    cout << "================================" << endl;
    
    string nik;
    cout << "Masukkan NIK: ";
    cin >> nik;
    cin.ignore();
    
    // Find user by NIK in DLL
    int userIndex = -1;
    int userLen = users.Length();
    for (int i = 0; i < userLen; i++) {
        if (users.Get(i).ktpData.getNIK() == nik) {
            userIndex = i;
            break;
        }
    }
    
    if (userIndex == -1) {
        cout << "NIK tidak ditemukan." << endl;
        return;
    }
    
    cout << "\n=== VERIFIKASI IDENTITAS ===" << endl;
    cout << "Untuk memverifikasi, jawab pertanyaan berikut:\n" << endl;
    
    string nama;
    cout << "1. Nama Lengkap: ";
    getline(cin, nama);
    
    int tanggalLahir;
    cout << "2. Tanggal Lahir (DD): ";
    cin >> tanggalLahir;
    cin.ignore();
    
    string pekerjaan;
    cout << "3. Pekerjaan: ";
    getline(cin, pekerjaan);
    
    User& u = users.Get(userIndex);
    if (u.ktpData.getNama() == nama &&
        u.ktpData.getTanggalLahir().hari == tanggalLahir &&
        u.ktpData.getPekerjaan() == pekerjaan) {
        
        cout << "\n================================" << endl;
        cout << "      VERIFIKASI BERHASIL! " << endl;
        cout << "================================" << endl;
        
        string newPassword = getPasswordWithConfirm();
        if (newPassword.empty()) {
            return;
        }
        
        u.password = newPassword;
        saveAllUsers();
        cout << "Password berhasil diubah!" << endl;
    } else {
        cout << "\n================================" << endl;
        cout << "      VERIFIKASI GAGAL! " << endl;
        cout << "================================" << endl;
        cout << "Data yang Anda masukkan tidak cocok." << endl;
    }
}

void UserSystem::forgotPasswordPublisher() {
    cout << "\n================================" << endl;
    cout << "    LUPA PASSWORD (PUBLISHER) " << endl;
    cout << "================================" << endl;
    
    string nik;
    cout << "Masukkan NIK: ";
    cin >> nik;
    cin.ignore();
    
    // Find publisher by NIK in DLL
    int publisherIndex = -1;
    int pubLen = publishers.Length();
    for (int i = 0; i < pubLen; i++) {
        if (publishers.Get(i).ktpData.getNIK() == nik) {
            publisherIndex = i;
            break;
        }
    }
    
    if (publisherIndex == -1) {
        cout << "NIK tidak ditemukan." << endl;
        return;
    }
    
    cout << "\n=== VERIFIKASI IDENTITAS ===" << endl;
    cout << "Untuk memverifikasi, jawab pertanyaan berikut:\n" << endl;
    
    string nama;
    cout << "1. Nama Lengkap: ";
    getline(cin, nama);
    
    int tanggalLahir;
    cout << "2. Tanggal Lahir (DD): ";
    cin >> tanggalLahir;
    cin.ignore();
    
    string pekerjaan;
    cout << "3. Pekerjaan: ";
    getline(cin, pekerjaan);
    
    Publisher& p = publishers.Get(publisherIndex);
    if (p.ktpData.getNama() == nama &&
        p.ktpData.getTanggalLahir().hari == tanggalLahir &&
        p.ktpData.getPekerjaan() == pekerjaan) {
        
        cout << "\n================================" << endl;
        cout << "      VERIFIKASI BERHASIL! " << endl;
        cout << "================================" << endl;
        
        string newPassword = getPasswordWithConfirm();
        if (newPassword.empty()) {
            return;
        }
        
        p.password = newPassword;
        saveAllPublishers();
        cout << "Password berhasil diubah!" << endl;
    } else {
        cout << "\n================================" << endl;
        cout << "      VERIFIKASI GAGAL! " << endl;
        cout << "================================" << endl;
        cout << "Data yang Anda masukkan tidak cocok." << endl;
    }
}

bool UserSystem::loginUser() {
    string nik, password;
    cout << "\n================================" << endl;
    cout << "          LOGIN AKUN " << endl;
    cout << "================================" << endl;
    cout << "Masukkan NIK: ";
    cin >> nik;
    cout << "Masukkan password: ";
    cin >> password;

    if (nik == "ADMIN" && password == "ADMIN") {
        cout << "\nLogin berhasil sebagai ADMIN! Selamat datang!\n" << endl;
        currentUser = nik;
        isLoggedIn = true;
        isPublisher = false;
        isAdmin = true;
        return true;
    }

    // Check users DLL
    int userLen = users.Length();
    for (int i = 0; i < userLen; i++) {
        User& u = users.Get(i);
        if (u.ktpData.getNIK() == nik) {
            if (u.password == password) {
                cout << "\nLogin berhasil sebagai USER! Selamat datang, " 
                     << u.ktpData.getNama() << "!\n" << endl;
                currentUser = nik;
                isLoggedIn = true;
                isPublisher = false;
                isAdmin = false;
                return true;
            } else {
                cout << "\nPassword salah." << endl;
                string pilihan;
                cout << "Lupa password? (y/n): ";
                cin >> pilihan;
                if (pilihan == "y" || pilihan == "Y") {
                    cin.ignore();
                    forgotPasswordUser();
                }
                return false;
            }
        }
    }

    // Check publishers DLL
    int pubLen = publishers.Length();
    for (int i = 0; i < pubLen; i++) {
        Publisher& p = publishers.Get(i);
        if (p.ktpData.getNIK() == nik) {
            if (p.password == password) {
                cout << "\nLogin berhasil sebagai PUBLISHER! Selamat datang, " 
                     << p.ktpData.getNama() << "!\n" << endl;
                currentUser = nik;
                isLoggedIn = true;
                isPublisher = true;
                isAdmin = false;
                return true;
            } else {
                cout << "\nPassword salah." << endl;
                string pilihan;
                cout << "Lupa password? (y/n): ";
                cin >> pilihan;
                if (pilihan == "y" || pilihan == "Y") {
                    cin.ignore();
                    forgotPasswordPublisher();
                }
                return false;
            }
        }
    }

    cout << "\nNIK tidak ditemukan." << endl;
    return false;
}

void UserSystem::logout() {
    currentUser = "";
    isLoggedIn = false;
    isPublisher = false;
    isAdmin = false;
    cout << "\nBerhasil logout!\n" << endl;
}

bool UserSystem::checkLoggedIn() {
    return isLoggedIn;
}

string UserSystem::getCurrentUser() {
    return currentUser;  // Returns NIK
}

bool UserSystem::getIsPublisher() {
    return isPublisher;
}

bool UserSystem::getIsAdmin() {
    return isAdmin;
}

string UserSystem::getPublisherStudio() {
    if (!isPublisher) return "";
    
    // Find publisher by NIK in DLL
    int pubLen = publishers.Length();
    for (int i = 0; i < pubLen; i++) {
        if (publishers.Get(i).ktpData.getNIK() == currentUser) {
            return publishers.Get(i).namaStudio;
        }
    }
    return "";
}

// Admin: Lihat daftar semua user
void UserSystem::listAllUsers() {
    cout << "\n========================================" << endl;
    cout << "         DAFTAR SEMUA USER              " << endl;
    cout << "========================================" << endl;
    
    int userLen = users.Length();
    
    if (userLen == 0) {
        cout << "Tidak ada user terdaftar." << endl;
        return;
    }
    
    cout << left << setw(5) << "No"
         << setw(20) << " NIK"
         << setw(30) << " Nama"
         << setw(30) << " Email" << endl;
    cout << string(85, '-') << endl;
    
    for (int i = 0; i < userLen; i++) {
        const User& u = users.Get(i);
        cout << setw(5) << (i + 1)
             << setw(20) << u.ktpData.getNIK()
             << setw(30) << u.ktpData.getNama()
             << setw(30) << (u.email.empty() ? "-" : u.email) << endl;
    }
    
    cout << "\nTotal User: " << userLen << endl;
}

// Admin: Lihat daftar semua publisher
void UserSystem::listAllPublishers() {
    cout << "\n========================================" << endl;
    cout << "       DAFTAR SEMUA PUBLISHER           " << endl;
    cout << "========================================" << endl;
    
    int pubLen = publishers.Length();
    
    if (pubLen == 0) {
        cout << "Tidak ada publisher terdaftar." << endl;
        return;
    }
    
    cout << left << setw(5) << "No"
         << setw(20) << " NIK"
         << setw(25) << " Nama"
         << setw(25) << " Studio"
         << setw(15) << " Negara" << endl;
    cout << string(90, '-') << endl;
    
    for (int i = 0; i < pubLen; i++) {
        const Publisher& p = publishers.Get(i);
        cout << setw(5) << (i + 1)
             << setw(20) << p.ktpData.getNIK()
             << setw(25) << p.ktpData.getNama()
             << setw(25) << p.namaStudio
             << setw(15) << p.negara << endl;
    }
    
    cout << "\nTotal Publisher: " << pubLen << endl;
}

#endif
