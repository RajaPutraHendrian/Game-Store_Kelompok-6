#ifndef KTP_
#define KTP_
#include <iostream>
#include <string>
using namespace std;

enum Kewarganegaraan {
    WNI, 
    WNA 
};

enum Agama {
    ISLAM, 
    KRISTEN,
    KATOLIK,
    HINDU,
    BUDDHA,
    KHONGHUCU,
    LAINNYA
};

// Jenis Kelamin menggunakan boolean: true = Laki-laki, false = Perempuan

enum GolonganDarah {
    A,  
    B,  
    AB, 
    O   
};

struct Date {
    int hari;
    int bulan;
    int tahun;
};

string getAgamaString(Agama a) {
    switch (a) {
        case ISLAM:     return "ISLAM";
        case KRISTEN:   return "KRISTEN";
        case KATOLIK:   return "KATOLIK";
        case HINDU:     return "HINDU";
        case BUDDHA:    return "BUDDHA";
        case KHONGHUCU: return "KHONGHUCU";
        case LAINNYA:   return "LAINNYA";
        default:        return "TIDAK DIKETAHUI";
    }
}

string getJenisKelaminString(bool isMale) {
    return isMale ? "LAKI-LAKI" : "PEREMPUAN";
}

string getGolonganDarahString(GolonganDarah gd) {
    switch (gd) {
        case A:  return "A";
        case B:  return "B";
        case AB: return "AB";
        case O:  return "O";
        default: return "TIDAK DIKETAHUI";
    }
}

class KTP {
    friend ostream& operator<<(ostream&, const KTP&);
    friend istream& operator>>(istream&, KTP&);
public:
    KTP();
    KTP(string nik, string nama, string alamat, Date tanggalLahir,
        bool jenisKelamin, GolonganDarah golonganDarah, Agama agama, 
        bool statusPerkawinan, string pekerjaan, Kewarganegaraan kewarganegaraan);
    
    string getNIK() const {return nik;}
    string getNama() const {return nama;}
    string getAlamat() const {return alamat;}
    Date getTanggalLahir() const {return tanggalLahir;}
    bool getJenisKelamin() const {return jenisKelamin;}
    GolonganDarah getGolonganDarah() const {return golonganDarah;}
    Agama getAgama() const {return agama;}
    bool getStatusPerkawinan() const {return statusPerkawinan;}
    string getPekerjaan() const {return pekerjaan;}
    Kewarganegaraan getKewarganegaraan() const {return kewarganegaraan;}
    
    bool operator==(const KTP& k) const {return nik == k.nik;}
    bool operator!=(const KTP& k) const {return nik != k.nik;}
    
    void editData();
    
private:
    string nik;
    string nama;
    string alamat;
    Date tanggalLahir;
    bool jenisKelamin;  // true = Laki-laki, false = Perempuan
    GolonganDarah golonganDarah;
    Agama agama;
    bool statusPerkawinan;
    string pekerjaan;
    Kewarganegaraan kewarganegaraan;
};

KTP::KTP()
{
    nik = "";
    nama = "";
    alamat = "";
    tanggalLahir.hari = 0;
    tanggalLahir.bulan = 0;
    tanggalLahir.tahun = 0;
    jenisKelamin = true;  // Default: Laki-laki
    golonganDarah = O;
    agama = ISLAM;
    statusPerkawinan = false;
    pekerjaan = "";
    kewarganegaraan = WNI;
}

KTP::KTP(string nik, string nama, string alamat, Date tanggalLahir,
         bool jenisKelamin, GolonganDarah golonganDarah, Agama agama, 
         bool statusPerkawinan, string pekerjaan, Kewarganegaraan kewarganegaraan)
{
    this->nik = nik;
    this->nama = nama;
    this->alamat = alamat;
    this->tanggalLahir = tanggalLahir;
    this->jenisKelamin = jenisKelamin;
    this->golonganDarah = golonganDarah;
    this->agama = agama;
    this->statusPerkawinan = statusPerkawinan;
    this->pekerjaan = pekerjaan;
    this->kewarganegaraan = kewarganegaraan;
}

void KTP::editData()
{
    int pilihan;
    cout << "\n--- Edit Data KTP ---" << endl;
    cout << "1. Edit Nama" << endl;
    cout << "2. Edit Alamat" << endl;
    cout << "3. Edit Tanggal Lahir" << endl;
    cout << "4. Edit Jenis Kelamin" << endl;
    cout << "5. Edit Golongan Darah" << endl;
    cout << "6. Edit Agama" << endl;
    cout << "7. Edit Status Perkawinan" << endl;
    cout << "8. Edit Pekerjaan" << endl;
    cout << "9. Edit Kewarganegaraan" << endl;
    cout << "Pilihan: ";
    cin >> pilihan;
    cin.ignore();
    
    switch(pilihan) {
        case 1:
            cout << "Nama baru: ";
            getline(cin, nama);
            break;
        case 2:
            cout << "Alamat baru: ";
            getline(cin, alamat);
            break;
        case 3:
            cout << "Tanggal Lahir (DD): ";
            cin >> tanggalLahir.hari;
            cout << "Bulan Lahir (MM): ";
            cin >> tanggalLahir.bulan;
            cout << "Tahun Lahir (YYYY): ";
            cin >> tanggalLahir.tahun;
            cin.ignore();
            break;
        case 4:
            cout << "Jenis Kelamin (1=LAKI-LAKI, 2=PEREMPUAN): ";
            cin >> pilihan;
            cin.ignore();
            jenisKelamin = (pilihan == 1);  // 1=Laki-laki (true), 2=Perempuan (false)
            break;
        case 5:
            cout << "Golongan Darah (1=A, 2=B, 3=AB, 4=O): ";
            cin >> pilihan;
            cin.ignore();
            switch(pilihan) {
                case 1: golonganDarah = A; break;
                case 2: golonganDarah = B; break;
                case 3: golonganDarah = AB; break;
                case 4: golonganDarah = O; break;
            }
            break;
        case 6:
            cout << "Agama (1=ISLAM, 2=KRISTEN, 3=KATOLIK, 4=HINDU, 5=BUDDHA, 6=KHONGHUCU, 7=LAINNYA): ";
            cin >> pilihan;
            cin.ignore();
            switch(pilihan) {
                case 1: agama = ISLAM; break;
                case 2: agama = KRISTEN; break;
                case 3: agama = KATOLIK; break;
                case 4: agama = HINDU; break;
                case 5: agama = BUDDHA; break;
                case 6: agama = KHONGHUCU; break;
                default: agama = LAINNYA; break;
            }
            break;
        case 7:
            cout << "Status Perkawinan (1=KAWIN, 2=BELUM KAWIN): ";
            cin >> pilihan;
            cin.ignore();
            statusPerkawinan = (pilihan == 1);
            break;
        case 8:
            cout << "Pekerjaan baru: ";
            getline(cin, pekerjaan);
            break;
        case 9:
            cout << "Kewarganegaraan (1=WNI, 2=WNA): ";
            cin >> pilihan;
            cin.ignore();
            kewarganegaraan = (pilihan == 1) ? WNI : WNA;
            break;
        default:
            cout << "Pilihan tidak valid!" << endl;
            return;
    }
    
    cout << "Data berhasil diubah!" << endl;
}

ostream& operator<<(ostream& out, const KTP& k)
{
    out << "NIK               : " << k.nik << endl;
    out << "Nama              : " << k.nama << endl;
    out << "Alamat            : " << k.alamat << endl;
    out << "Tanggal Lahir     : " << k.tanggalLahir.hari << "-" 
                                  << k.tanggalLahir.bulan << "-"
                                  << k.tanggalLahir.tahun << endl;
    out << "Jenis Kelamin     : " << getJenisKelaminString(k.jenisKelamin) << endl;
    out << "Golongan Darah    : " << getGolonganDarahString(k.golonganDarah) << endl;
    out << "Agama             : " << getAgamaString(k.agama) << endl;
    out << "Status Perkawinan : " << (k.statusPerkawinan ? "KAWIN" : "BELUM KAWIN") << endl;
    out << "Pekerjaan         : " << k.pekerjaan << endl;
    out << "Kewarganegaraan   : " << (k.kewarganegaraan == WNI ? "WNI" : "WNA") << endl;
    return out;
}

istream& operator>>(istream& in, KTP& k)
{
    int pilihan;
    
    // Clear any leftover newline from previous input
    if (in.peek() == '\n') {
        in.ignore();
    }
    
    cout << "Masukkan NIK: ";
    getline(in, k.nik);
    cout << "Masukkan Nama: ";
    getline(in, k.nama);
    cout << "Masukkan Alamat: ";
    getline(in, k.alamat);
    
    cout << "Masukkan Tanggal Lahir (DD): ";
    in >> k.tanggalLahir.hari;
    cout << "Masukkan Bulan Lahir (MM): ";
    in >> k.tanggalLahir.bulan;
    cout << "Masukkan Tahun Lahir (YYYY): ";
    in >> k.tanggalLahir.tahun;
    in.ignore();
    
    cout << "Jenis Kelamin (1=LAKI-LAKI, 2=PEREMPUAN): ";
    in >> pilihan;
    in.ignore();
    k.jenisKelamin = (pilihan == 1);  // 1=Laki-laki (true), 2=Perempuan (false)
    
    cout << "Pilih Golongan Darah:" << endl;
    cout << "  1. A\n  2. B\n  3. AB\n  4. O" << endl;
    cout << "Pilihan Golongan Darah (1-4): ";
    in >> pilihan;
    in.ignore();
    switch (pilihan) {
        case 1: k.golonganDarah = A; break;
        case 2: k.golonganDarah = B; break;
        case 3: k.golonganDarah = AB; break;
        case 4: k.golonganDarah = O; break;
        default: k.golonganDarah = O; break;
    }
    
    cout << "Pilih Agama:" << endl;
    cout << "  1. ISLAM\n  2. KRISTEN\n  3. KATOLIK\n  4. HINDU\n  5. BUDDHA\n  6. KHONGHUCU\n  7. LAINNYA" << endl;
    cout << "Pilihan Agama (1-7): ";
    in >> pilihan;
    in.ignore();
    switch (pilihan) {
        case 1: k.agama = ISLAM; break;
        case 2: k.agama = KRISTEN; break;
        case 3: k.agama = KATOLIK; break;
        case 4: k.agama = HINDU; break;
        case 5: k.agama = BUDDHA; break;
        case 6: k.agama = KHONGHUCU; break;
        default: k.agama = LAINNYA; break;
    }
    
    cout << "Status Perkawinan (1=KAWIN, 2=BELUM KAWIN): ";
    in >> pilihan;
    k.statusPerkawinan = (pilihan == 1);
    in.ignore();
    
    cout << "Masukkan Pekerjaan: ";
    getline(in, k.pekerjaan);
    
    cout << "Kewarganegaraan (1=WNI, 2=WNA): ";
    in >> pilihan;
    k.kewarganegaraan = (pilihan == 1) ? WNI : WNA;
    in.ignore();
    
    return in;
}

#endif
