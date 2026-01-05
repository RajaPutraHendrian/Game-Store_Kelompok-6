#ifndef Membership_
#define Membership_

#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
using namespace std;

struct Membership {
    int purchaseCount;
    bool hasVoucher;
};

class MembershipSystem {
private:
    Membership data;
    string currentUser;
    
    string getMembershipFile();

public:
    MembershipSystem();
    
    void setUser(const string& username);
    void resetUser();
    void load();
    void save();
    void addPurchaseAndMaybeGrantVoucher();
    bool askUseVoucher();
    void useVoucher();
    bool hasVoucher() const;
};

MembershipSystem::MembershipSystem() {
    data.purchaseCount = 0;
    data.hasVoucher = false;
    currentUser = "";
}

string MembershipSystem::getMembershipFile() {
    return currentUser + "_membership.txt";
}

void MembershipSystem::setUser(const string& username) {
    currentUser = username;
    load();
}

void MembershipSystem::resetUser() {
    currentUser = "";
    data.purchaseCount = 0;
    data.hasVoucher = false;
}

void MembershipSystem::load() {
    if (currentUser.empty()) return;
    ifstream in(getMembershipFile().c_str());
    if (!in) return;
    in >> data.purchaseCount >> data.hasVoucher;
    in.close();
}

void MembershipSystem::save() {
    if (currentUser.empty()) return;
    ofstream out(getMembershipFile().c_str());
    if (!out) return;
    out << data.purchaseCount << " " << data.hasVoucher;
    out.close();
}

void MembershipSystem::addPurchaseAndMaybeGrantVoucher() {
    data.purchaseCount++;
    if (data.purchaseCount >= 4) {
        data.purchaseCount = 0;
        data.hasVoucher = true;
        cout << "\n** SELAMAT! Anda mendapatkan VOUCHER 30% (hanya 1x pakai) karena telah membeli 4 game.\n";
    }
    save();
}

bool MembershipSystem::askUseVoucher() {
    if (!data.hasVoucher) return false;
    cout << "\nAnda memiliki voucher diskon 30%. Gunakan sekarang? (Y/N): ";
    char ch;
    cin >> ch;
    return (toupper(static_cast<unsigned char>(ch)) == 'Y');
}

void MembershipSystem::useVoucher() {
    if (!data.hasVoucher) return;
    data.hasVoucher = false;
    save();
}

bool MembershipSystem::hasVoucher() const {
    return data.hasVoucher;
}

#endif
