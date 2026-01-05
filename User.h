#ifndef User_
#define User_

#include <string>
#include "ktp.h"
using namespace std;

struct User {
    KTP ktpData;
    string password;
    string email;
};

struct Publisher {
    KTP ktpData;
    string password;
    string email;
    string namaStudio;
    string negara;
    string tahunBerdiri;
};

#endif
