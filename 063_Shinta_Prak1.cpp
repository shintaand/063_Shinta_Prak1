#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct servis {
    string model, merek, kendala, montir;
    servis* next;
};

struct pelanggan {
    string nama, gender, alamat, nomor;
    int umur;
    servis* riwayatservis; 
    pelanggan *next, *prev;
};

pelanggan* headpel = NULL;
pelanggan* tailpel = NULL;


pelanggan* caripelanggan(string nama) {
    pelanggan* temp = headpel;
    while (temp != NULL) {
        if (temp->nama == nama) return temp;
        temp = temp->next;
    }
    return NULL;
}

pelanggan* tambahpelanggan(string nama, string nomor = "", string alamat = "", int umur = 0, string gender = "") {
    pelanggan* baru = new pelanggan;
    baru->nama = nama;
    baru->nomor = nomor;
    baru->alamat = alamat;
    baru->umur = umur;
    baru->gender = gender;
    baru->riwayatservis = NULL;
    baru->next = NULL;
    baru->prev = tailpel;

    if (headpel == NULL) {
        headpel = tailpel = baru;
    } else {
        tailpel->next = baru;
        tailpel = baru;
    }
    return baru;
}

void tambahservis(pelanggan* pel, string mod, string mer, string ken, string mon) {
    servis* baru = new servis;
    baru->model = mod;
    baru->merek = mer;
    baru->kendala = ken;
    baru->montir = mon;
    baru->next = NULL;

    if (pel->riwayatservis == NULL) {
        pel->riwayatservis = baru;
    } else {
        servis* temp = pel->riwayatservis;
        while (temp->next != NULL) temp = temp->next;
        temp->next = baru;
    }
}

void simpan() {
    ofstream file("bengkel.csv");
    pelanggan* p = headpel;
    while (p != NULL) {
        servis* s = p->riwayatservis;
        while (s != NULL) {
            file << p->nama << "," << p->nomor << "," << p->alamat << "," 
                 << p->umur << "," << p->gender << "," << s->model << "," 
                 << s->merek << "," << s->kendala << "," << s->montir << "\n";
            s = s->next;
        }
        p = p->next;
    }
    file.close();
}

void loading() {
    ifstream file("bengkel.csv");
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string nama, nomor, alamat, umurStr, gender, mod, mer, ken, mon;

        getline(ss, nama, ',');
        getline(ss, nomor, ',');
        getline(ss, alamat, ',');
        getline(ss, umurStr, ',');
        getline(ss, gender, ',');
        getline(ss, mod, ',');
        getline(ss, mer, ',');
        getline(ss, ken, ',');
        getline(ss, mon, ',');

        if (nama == "") continue;

        pelanggan* pel = caripelanggan(nama);
        if (pel == NULL) {
            pel = tambahpelanggan(nama, nomor, alamat, stoi(umurStr), gender);
        }
        tambahservis(pel, mod, mer, ken, mon);
    }
    file.close();
}

void menuservisbaru() {
    string nama, mod, mer, ken, mon;
    cout << "\n=== SERVIS ===\n";
    cout << "Model Mobil: "; getline(cin, mod);
    cout << "Merek Mobil: "; getline(cin, mer);
    cout << "Kendala: "; getline(cin, ken);
    cout << "Montir: "; getline(cin, mon);
    cout << "Nama Pelanggan: "; getline(cin, nama);

    pelanggan* pel = caripelanggan(nama);
    if (pel != NULL) {
        tambahservis(pel, mod, mer, ken, mon);
        cout << "Data servis sudah ditambahkan ke data pelanggan lama.\n";
    } else {
        pelanggan* baru = tambahpelanggan(nama);
        cout << "No Telp Pelanggan: "; getline(cin, baru->nomor);
        cout << "Alamat: "; getline(cin, baru->alamat);
        cout << "Umur: "; cin >> baru->umur; cin.ignore();
        cout << "Gender: "; getline(cin, baru->gender);
        tambahservis(baru, mod, mer, ken, mon);
        cout << "\nPelanggan baru sudah terdaftar.\n";
    }
    simpan();
    cout << "Tekan Enter untuk kembali..."; cin.get();
}

void tampilsemuaservis() {
    cout << "\n=== SEMUA SERVIS ===\n";
    pelanggan* p = headpel;
    bool ada = false;
    while (p != NULL) {
        servis* s = p->riwayatservis;
        while (s != NULL) {
            cout << "-----------------------\n";
            cout << "Pelanggan: " << p->nama << "\nNo Telp: " << p->nomor << endl;
            cout << "Model Mobil: " << s->model << "\nMerek: " << s->merek << endl;
            cout << "Kendala: " << s->kendala << "\nMontir: " << s->montir << endl;
            s = s->next;
            ada = true;
        }
        p = p->next;
    }
    if(!ada) cout << "Belum ada data servis.\n";
    cout << "\nTekan Enter untuk kembali..."; cin.get();
}

void riwayatmontir() {
    string montirlist[] = {"Suby", "Dimas, "Farhan", "Aldo"};
    cout << "\n=== RIWAYAT PEKERJAAN MONTIR ===\n";
    cout << "Pilih montir?\n";
    for(int i=0; i<4; i++) cout << i+1 << ". " << montirlist[i] << endl;
    int pilih; cout << "Pilihan: "; cin >> pilih; cin.ignore();
    
    if(pilih < 1 || pilih > 4) return;

    string target = montirlist[pilih-1];
    cout << "\n=== RIWAYAT PEKERJAAN " << target << " ===\n";
    pelanggan* p = headpel;
    while (p != NULL) {
        servis* s = p->riwayatservis;
        while (s != NULL) {
            if (s->montir == target) {
                cout << "-----------------------\n";
                cout << "Pelanggan: " << p->nama << "\n";
                cout << "Mobil: " << s->model << " (" << s->merek << ")\n";
                cout << "Kendala: " << s->kendala << endl;
            }
            s = s->next;
        }
        p = p->next;
    }
    cout << "Tekan Enter untuk kembali..."; 
    cin.get();
}

void menusemuapelanggan() {
    cout << "\n=== SEMUA PELANGGAN ===\n";
    pelanggan* p = headpel;
    if (p == NULL) cout << "Data pelanggan kosong.\n";
    while (p != NULL) {
        cout << "-----------------------\n";
        cout << "Nama: " << p->nama << "\nTelepon: " << p->nomor << "\nAlamat: " << p->alamat << endl;
        
        servis* s = p->riwayatservis;
        if (s != NULL) {
            while (s->next != NULL) s = s->next;
            cout << "--- Servis Terakhir ---\nMobil: " << s->model << "\nMontir: " << s->montir << endl;
        }
        p = p->next;
    }
    cout << "\nTekan Enter untuk kembali..."; 
    cin.get();
}

void menudatapelanggan() {
    if (headpel == NULL) {
        cout << "\nData pelanggan masih kosong!\n";
        cout << "Tekan Enter..."; 
        cin.get();
        return;
    }
    pelanggan* curr = headpel;
    string pilih;
    do {
        cout << "\n=== Data Pelanggan ===\n";
        cout << "Nama: " << curr->nama << "\nTelepon: " << curr->nomor << "\nUmur: " << curr->umur << endl;
        cout << "Gender: " << curr->gender << "\nAlamat: " << curr->alamat << endl;
        cout << "-----------------------\nRiwayat Servis:\n";
        servis* s = curr->riwayatservis;
        while (s != NULL) {
            cout << "- " << s->model << " | " << s->kendala << " | Montir: " << s->montir << endl;
            s = s->next;
        }
        cout << "-----------------------\n[N]ext, [P]revious, [E]xit\nPilihan: ";
        getline(cin, pilih);
        
        if ((pilih == "n" || pilih == "N") && curr->next != NULL) curr = curr->next;
        else if ((pilih == "p" || pilih == "P") && curr->prev != NULL) curr = curr->prev;
        
    } while (pilih != "e" && pilih != "E");
}

int main() {
    loading();
    int pilihan;
    do {
        cout << "\n=== SELAMAT DATANG DI UNOSERVIS ===\n";
        cout << "1. Servis\n";
        cout << "2. Semua Data Pelanggan\n";
        cout << "3. Data Pelanggan\n";
        cout << "4. Keluar\n";
        cout << "Pilihan: ";
        if (!(cin >> pilihan)) { cin.clear(); cin.ignore(1000, '\n'); continue; }
        cin.ignore();

        if (pilihan == 1) {
            cout << "\n=== SUB-MENU SERVIS ===\n";
            cout << "1. Semua Servis\n";
            cout << "2. Servis Baru\n";
            cout << "3. Riwayat Montir\n";
            cout << "Pilihan: ";
            int p; cin >> p; cin.ignore();

            if (p == 1) tampilsemuaservis();
            else if (p == 2) menuservisbaru();
            else if (p == 3) riwayatmontir();

        } else if (pilihan == 2) {
            menusemuapelanggan();

        } else if (pilihan == 3) {
            menudatapelanggan();
        }
    } while (pilihan != 4);

    return 0;
}
