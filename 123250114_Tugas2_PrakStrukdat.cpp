
#include <iostream>
#include <string>
using namespace std;

struct Antrian {
    string nama;
    Antrian* next;
};

struct Menu {
    string nama, kategori;
    int harga;
    bool tersedia;

    Antrian *front, *rear;
    Menu *left, *right;
};

struct Riwayat {
    int jenis;
    string menu, pelanggan;
    bool antrian;
    string diberikan;
    Riwayat* next;
};

Menu* root = nullptr;
Riwayat* top = nullptr;

void enqueue(Menu* m, string nama) {
    Antrian* baru = new Antrian;
    baru->nama = nama;
    baru->next = nullptr;

    if (m->rear == nullptr)
        m->front = m->rear = baru;
    else {
        m->rear->next = baru;
        m->rear = baru;
    }
}

string dequeue(Menu* m) {
    if (m->front == nullptr) return "";

    Antrian* hapus = m->front;
    string nama = hapus->nama;
    m->front = m->front->next;

    if (m->front == nullptr) m->rear = nullptr;

    delete hapus;
    return nama;
}

void enqueueDepan(Menu* m, string nama) {
    Antrian* baru = new Antrian;
    baru->nama = nama;
    baru->next = m->front;
    m->front = baru;

    if (m->rear == nullptr) m->rear = baru;
}

void hapusTerakhir(Menu* m) {
    if (m->front == nullptr) return;

    if (m->front == m->rear) {
        delete m->front;
        m->front = m->rear = nullptr;
        return;
    }

    Antrian* bantu = m->front;
    while (bantu->next != m->rear)
        bantu = bantu->next;

    delete m->rear;
    m->rear = bantu;
    m->rear->next = nullptr;
}

void tampilAntrian(Menu* m) {
    if (m->front == nullptr) {
        cout << "Antrian kosong\n";
        return;
    }

    Antrian* bantu = m->front;
    while (bantu != nullptr) {
        cout << bantu->nama << " menunggu menu "
             << m->nama << endl;
        bantu = bantu->next;
    }
}

void push(int jenis, string menu, string pelanggan,
          bool antrian, string diberikan) {
    Riwayat* baru = new Riwayat;

    baru->jenis = jenis;
    baru->menu = menu;
    baru->pelanggan = pelanggan;
    baru->antrian = antrian;
    baru->diberikan = diberikan;

    baru->next = top;
    top = baru;
}

Menu* cari(Menu* p, string nama) {
    if (p == nullptr || p->nama == nama)
        return p;

    if (nama < p->nama)
        return cari(p->left, nama);

    return cari(p->right, nama);
}

Menu* buatMenu(string nama, string kategori, int harga) {
    Menu* baru = new Menu;

    baru->nama = nama;
    baru->kategori = kategori;
    baru->harga = harga;
    baru->tersedia = true;

    baru->front = baru->rear = nullptr;
    baru->left = baru->right = nullptr;

    return baru;
}

Menu* tambah(Menu* p, string nama,
             string kategori, int harga) {
    if (p == nullptr)
        return buatMenu(nama, kategori, harga);

    if (nama < p->nama)
        p->left = tambah(p->left, nama, kategori, harga);
    else if (nama > p->nama)
        p->right = tambah(p->right, nama, kategori, harga);

    return p;
}

void inorder(Menu* p) {
    if (p == nullptr) return;

    inorder(p->left);

    cout << "\nNama     : " << p->nama;
    cout << "\nKategori : " << p->kategori;
    cout << "\nHarga    : Rp" << p->harga;
    cout << "\nStatus   : "
         << (p->tersedia ? "Tersedia" : "Habis");
    cout << "\n----------------------\n";

    inorder(p->right);
}

void hapusQueue(Menu* p) {
    while (p->front != nullptr)
        dequeue(p);
}

Menu* minimum(Menu* p) {
    while (p->left != nullptr)
        p = p->left;
    return p;
}

Menu* hapus(Menu* p, string nama) {
    if (p == nullptr) return nullptr;

    if (nama < p->nama)
        p->left = hapus(p->left, nama);
    else if (nama > p->nama)
        p->right = hapus(p->right, nama);
    else {
        if (p->left == nullptr || p->right == nullptr) {
            Menu* temp = (p->left != nullptr) ? p->left : p->right;
            hapusQueue(p);
            delete p;
            return temp;
        }

        Menu* temp = minimum(p->right);

        p->nama = temp->nama;
        p->kategori = temp->kategori;
        p->harga = temp->harga;
        p->tersedia = temp->tersedia;

        hapusQueue(p);
        p->front = temp->front;
        p->rear = temp->rear;
        temp->front = temp->rear = nullptr;

        p->right = hapus(p->right, temp->nama);
    }

    return p;
}

void tambahMenu() {
    string nama, kategori;
    int harga;

    cout << "Nama menu: ";
    getline(cin, nama);
    cout << "Kategori: ";
    getline(cin, kategori);
    cout << "Harga: ";
    cin >> harga;
    cin.ignore(1000, '\n');

    if (cari(root, nama) != nullptr) {
        cout << "Menu sudah ada!\n";
        return;
    }

    root = tambah(root, nama, kategori, harga);
    cout << "Menu berhasil ditambahkan\n";
}

void lihatMenu() {
    if (root == nullptr)
        cout << "Belum ada menu\n";
    else
        inorder(root);
}


void pesanMenu() {
    string pelanggan, nama;

    cout << "Nama pelanggan: ";
    getline(cin, pelanggan);
    cout << "Nama menu: ";
    getline(cin, nama);

    Menu* m = cari(root, nama);

    if (m == nullptr) {
        cout << "Menu tidak ditemukan\n";
        return;
    }

    if (m->tersedia) {
        cout << "Pesanan " << pelanggan
             << " berhasil dibuat\n";

        m->tersedia = false;

        push(1, nama, pelanggan, false, "");
    }
    else {
        enqueue(m, pelanggan);

        cout << "Menu sedang habis, " << pelanggan
             << " masuk ke dalam waiting list\n";

        push(1, nama, pelanggan, true, "");
    }
}


void menuTersedia() {
    string nama;

    cout << "Nama menu: ";
    getline(cin, nama);

    Menu* m = cari(root, nama);

    if (m == nullptr) {
        cout << "Menu tidak ditemukan\n";
        return;
    }

    if (m->tersedia) {
        cout << "Menu sudah tersedia\n";
        return;
    }

    m->tersedia = true;
    string pelanggan = "";

    cout << "Menu tersedia kembali\n";

    if (m->front != nullptr) {
        pelanggan = dequeue(m);
        cout << "Pesanan langsung diberikan kepada "
             << pelanggan << " dari waiting list\n";
    }

    push(2, nama, "", false, pelanggan);
}

void hapusMenu() {
    string nama;

    cout << "Nama menu: ";
    getline(cin, nama);

    if (cari(root, nama) == nullptr) {
        cout << "Menu tidak ditemukan\n";
        return;
    }

    root = hapus(root, nama);
    cout << "Menu berhasil dihapus\n";
}

void undo() {
    if (top == nullptr) {
        cout << "Tidak ada aksi untuk di-undo\n";
        return;
    }

    Riwayat* bantu = top;
    top = top->next;

    Menu* m = cari(root, bantu->menu);

    if (m == nullptr) {
        cout << "Menu tidak ditemukan\n";
        delete bantu;
        return;
    }

    if (bantu->jenis == 1) {
        if (bantu->antrian)
            hapusTerakhir(m);

        cout << "Undo: Pesanan menu " << bantu->menu
             << " dibatalkan\n";
    }
    else {
        m->tersedia = false;

        if (bantu->diberikan != "")
            enqueueDepan(m, bantu->diberikan);

        cout << "Undo: Menu " << bantu->menu
             << " kembali habis\n";
    }

    delete bantu;
}

void lihatAntrian() {
    string nama;

    cout << "Nama menu: ";
    getline(cin, nama);

    Menu* m = cari(root, nama);

    if (m == nullptr) {
        cout << "Menu tidak ditemukan\n";
        return;
    }

    tampilAntrian(m);
}

int main() {
    int pilih;

    do {
        cout << "\n+===== SISTEM PESANAN RESTORAN =====+\n";
        cout << "| 1. Tambah Menu\n";
        cout << "| 2. Lihat Daftar Menu\n";
        cout << "| 3. Pesan Menu\n";
        cout << "| 4. Menu Tersedia Kembali\n";
        cout << "| 5. Hapus Menu\n";
        cout << "| 6. Undo Aktivitas\n";
        cout << "| 7. Lihat Antrian\n";
        cout << "| 8. Keluar\n";
        cout << "+======================================+";
        cout << "| Pilihan: ";

        cin >> pilih;
        cin.ignore(1000, '\n');

        switch (pilih) {
            case 1: tambahMenu(); break;
            case 2: lihatMenu(); break;
            case 3: pesanMenu(); break;
            case 4: menuTersedia(); break;
            case 5: hapusMenu(); break;
            case 6: undo(); break;
            case 7: lihatAntrian(); break;
            case 0: cout << "Program selesai\n"; break;
            default: cout << "Pilihan tidak valid\n";
        }

    } while (pilih != 0);

    return 0;
}
