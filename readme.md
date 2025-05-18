# Proyek Tim Praktikum: Aplikasi Toko Buku

## Program Pencatatan dan Manajemen Toko Buku

Program ini dibuat untuk mengelola data buku, mencatat transaksi penjualan, dan mengatur stok buku pada sebuah toko buku.

### Anggota Kelompok
1. ANDREAS ARNOL - 2802631564
2. JESSICA FREZA KUMALA - 2802642776
3. SAFIRA TAFANI CHOLISI - 2802647000
4. SHANIA ASTI MAFAZA - 2802638860
5. WAHYU ARDANI SETIAWAN - 2802634862

---

### Fitur Utama Aplikasi

Program ini memiliki beberapa fitur utama yang diakses melalui menu interaktif:

1.  **Input Data Buku (Tambah Judul Baru)**
    *   Memungkinkan pengguna untuk menambahkan buku baru ke dalam sistem.
    *   Input yang diminta: Nama Buku, Jenis Buku, Harga Buku, dan Stok Awal.
    *   Kode buku akan digenerate secara otomatis (misal: BK001, BK002, dst.).

2.  **Jual Buku** (penambahan di luar requirement soal)
    *   Memfasilitasi proses penjualan buku.
    *   Pengguna memilih buku dari daftar yang tersedia dan memasukkan jumlah yang ingin dibeli.
    *   Program akan memeriksa ketersediaan stok.
    *   Jika stok mencukupi, stok buku akan dikurangi, dan transaksi akan dicatat dalam history penjualan.

3.  **Tambah Stok Buku** (penambahan di luar requirement soal)
    *   Memungkinkan pengguna untuk menambahkan jumlah stok ke buku yang sudah ada dalam sistem.
    *   Pengguna memilih buku dan memasukkan jumlah stok yang ditambahkan.

4.  **Lihat History Penjualan**
    *   Menampilkan riwayat semua transaksi penjualan yang telah dilakukan.
    *   Detail yang ditampilkan: Nama Buku, Kode Buku, Jumlah Terjual, Total Harga, dan Sisa Stok setelah transaksi.

5.  **Lihat Data Buku**
    *   Menampilkan seluruh daftar buku yang tersimpan dalam sistem.
    *   Informasi yang ditampilkan: Nomor Urut, Kode Buku, Nama Buku, Jenis Buku, Harga, dan Stok Saat Ini.

6.  **Hapus History Penjualan**
    *   Memungkinkan pengguna untuk menghapus entri tertentu dari history penjualan berdasarkan nomor urut.

7.  **Hapus Data Buku**
    *   Memungkinkan pengguna untuk menghapus data buku tertentu dari sistem berdasarkan nomor urut.

8.  **Keluar**
    *   Menutup aplikasi.
    *   Sebelum keluar, semua data buku (termasuk perubahan stok) akan disimpan secara otomatis ke file `databuku.txt`.
    *   History penjualan disimpan setiap kali ada transaksi penjualan atau penghapusan history.

---

### File Data

Aplikasi ini menggunakan dua file teks untuk menyimpan data:

*   **`databuku.txt`**: Menyimpan daftar buku.
    *   Format per baris: `kode_buku,nama_buku,jenis_buku,harga,stok`
    *   Contoh: `BK001,Harry Potter,Fantasy,150000,20`
*   **`history.txt`**: Menyimpan riwayat transaksi penjualan.
    *   Format per baris: `Terjual: [Nama Buku] ([Kode Buku]), Jumlah: [Jumlah], Total Harga: [Harga]`
    *   Contoh: `Terjual: Harry Potter (BK001), Jumlah: 1, Total Harga: 150000`

---

### Cara Kompilasi Program (Menggunakan GCC)

Untuk mengkompilasi program C (`bookstore_app.c`), Anda dapat menggunakan compiler GCC dengan perintah berikut di terminal atau command prompt:

```bash
gcc bookstore_app.c -o bookstore_app
```

Perintah ini akan menghasilkan file executable bernama `bookstore_app` (di Linux/macOS) atau `bookstore_app.exe` (di Windows).

### Cara Menjalankan Program

Setelah berhasil dikompilasi, Anda dapat menjalankan program dengan perintah:

*   Di Linux/macOS:
    ```bash
    ./bookstore_app
    ```
*   Di Windows:
    ```bash
    bookstore_app.exe
    ```
Pastikan file `databuku.txt` berada di direktori yang sama dengan program saat dijalankan. File `history.txt` akan dibuat secara otomatis jika belum ada.

---

### Alur Umum Program

1.  Saat program dijalankan, data buku dari `databuku.txt` dan history penjualan dari `history.txt` akan dimuat.
2.  Menu utama akan ditampilkan dengan 8 pilihan (termasuk keluar).
3.  Pengguna memilih opsi menu dengan memasukkan nomor yang sesuai.
4.  Program akan menjalankan fungsi yang dipilih (misalnya, input buku, jual buku, lihat data, dll.).
5.  Setelah satu tugas selesai, menu akan muncul kembali sampai pengguna memilih opsi "Keluar".
6.  Saat keluar, data buku terbaru akan disimpan kembali ke `databuku.txt`.
