#ifndef PaymentSystem_
#define PaymentSystem_

#include <iostream>
#include <string>
using namespace std;

enum PaymentCategory {
    EWALLET = 1,
    BANK_TRANSFER = 2,
    CARD = 3,
    VIRTUAL_ACCOUNT = 4,
    QRIS = 5
};

class PaymentSystem {
private:
    string selectedMethod;
    PaymentCategory selectedCategory;
    
    void showEWalletOptions() {
        cout << "\n=== E-WALLET ===" << endl;
        cout << "1. GoPay" << endl;
        cout << "2. OVO" << endl;
        cout << "3. DANA" << endl;
        cout << "4. ShopeePay" << endl;
        cout << "Pilih e-wallet: ";
    }
    
    void showBankOptions() {
        cout << "\n=== BANK TRANSFER ===" << endl;
        cout << "1. BCA" << endl;
        cout << "2. Mandiri" << endl;
        cout << "3. BNI" << endl;
        cout << "4. BRI" << endl;
        cout << "Pilih bank: ";
    }
    
    void showCardOptions() {
        cout << "\n=== CREDIT/DEBIT CARD ===" << endl;
        cout << "1. Visa" << endl;
        cout << "2. Mastercard" << endl;
        cout << "3. JCB" << endl;
        cout << "Pilih card: ";
    }
    
    void showVAOptions() {
        cout << "\n=== VIRTUAL ACCOUNT ===" << endl;
        cout << "1. BCA Virtual Account" << endl;
        cout << "2. Mandiri Virtual Account" << endl;
        cout << "3. BNI Virtual Account" << endl;
        cout << "Pilih VA: ";
    }
    
    bool processPayment(int amount) {
        cout << "\n========================================" << endl;
        cout << "         MEMPROSES PEMBAYARAN..." << endl;
        cout << "========================================" << endl;
        cout << "Metode     : " << selectedMethod << endl;
        cout << "Total      : Rp " << amount << endl;
        cout << "----------------------------------------" << endl;
        
        if (selectedCategory == EWALLET) {
            cout << "\nSilakan buka aplikasi " << selectedMethod << endl;
            cout << "dan konfirmasi pembayaran." << endl;
        } else if (selectedCategory == BANK_TRANSFER || selectedCategory == VIRTUAL_ACCOUNT) {
            cout << "\nTransfer ke rekening berikut:" << endl;
            cout << "No. Rekening: 1234567890" << endl;
            cout << "Atas Nama: GAME STORE" << endl;
        } else if (selectedCategory == CARD) {
            cout << "\nMasukkan detail kartu Anda..." << endl;
            cout << "(Simulasi - langsung berhasil)" << endl;
        } else if (selectedCategory == QRIS) {
            cout << "\nScan QR Code untuk membayar:" << endl;
            cout << "[QR CODE PLACEHOLDER]" << endl;
        }
        
        cout << "\nTekan Enter untuk melanjutkan...";
        cin.ignore(1000, '\n');
        cin.get();
        
        cout << "\n========================================" << endl;
        cout << "       PEMBAYARAN BERHASIL!" << endl;
        cout << "========================================\n" << endl;
        return true;
    }

public:
    PaymentSystem() : selectedMethod(""), selectedCategory(EWALLET) {}
    
    bool selectPaymentMethod() {
        cout << "\n========================================" << endl;
        cout << "       PILIH METODE PEMBAYARAN" << endl;
        cout << "========================================" << endl;
        cout << "1. E-Wallet (GoPay, OVO, DANA, dll)" << endl;
        cout << "2. Bank Transfer (BCA, Mandiri, dll)" << endl;
        cout << "3. Credit/Debit Card" << endl;
        cout << "4. Virtual Account" << endl;
        cout << "5. QRIS" << endl;
        cout << "0. Batal" << endl;
        cout << "Pilih kategori: ";
        
        int category;
        if (!(cin >> category)) {
            cin.clear();
            cin.ignore(1000, '\n');
            return false;
        }
        
        if (category == 0) return false;
        if (category < 1 || category > 5) {
            cout << "Pilihan tidak valid!" << endl;
            return false;
        }
        
        selectedCategory = static_cast<PaymentCategory>(category);
        int choice;
        
        switch (selectedCategory) {
            case EWALLET:
                showEWalletOptions();
                if (!(cin >> choice) || choice < 1 || choice > 4) return false;
                switch (choice) {
                    case 1: selectedMethod = "GoPay"; break;
                    case 2: selectedMethod = "OVO"; break;
                    case 3: selectedMethod = "DANA"; break;
                    case 4: selectedMethod = "ShopeePay"; break;
                }
                break;
                
            case BANK_TRANSFER:
                showBankOptions();
                if (!(cin >> choice) || choice < 1 || choice > 4) return false;
                switch (choice) {
                    case 1: selectedMethod = "BCA Transfer"; break;
                    case 2: selectedMethod = "Mandiri Transfer"; break;
                    case 3: selectedMethod = "BNI Transfer"; break;
                    case 4: selectedMethod = "BRI Transfer"; break;
                }
                break;
                
            case CARD:
                showCardOptions();
                if (!(cin >> choice) || choice < 1 || choice > 3) return false;
                switch (choice) {
                    case 1: selectedMethod = "Visa Card"; break;
                    case 2: selectedMethod = "Mastercard"; break;
                    case 3: selectedMethod = "JCB Card"; break;
                }
                break;
                
            case VIRTUAL_ACCOUNT:
                showVAOptions();
                if (!(cin >> choice) || choice < 1 || choice > 3) return false;
                switch (choice) {
                    case 1: selectedMethod = "BCA Virtual Account"; break;
                    case 2: selectedMethod = "Mandiri Virtual Account"; break;
                    case 3: selectedMethod = "BNI Virtual Account"; break;
                }
                break;
                
            case QRIS:
                selectedMethod = "QRIS";
                break;
        }
        
        return !selectedMethod.empty();
    }
    
    string getPaymentMethodName() const {
        return selectedMethod;
    }
    
    void displayPaymentInfo(int amount) {
        if (!processPayment(amount)) {
            cout << "Pembayaran gagal!" << endl;
        }
    }
};

#endif
