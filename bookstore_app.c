#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // For isspace

#define MAX_LINE_LENGTH 256
#define MAX_BOOKS 100 // Initial capacity, can be made dynamic
#define MAX_HISTORY 200 // Initial capacity for history

// Struct untuk Buku
typedef struct {
  char kode[10];        // Kode unik buku
  char nama[100];       // Nama atau judul buku
  char jenis[50];       // Jenis atau kategori buku
  int harga;            // Harga buku
  int stok;             // Jumlah stok buku
} Book;

// Struct untuk Entri History (disimpan sebagai string)
typedef struct {
  char entry[MAX_LINE_LENGTH]; // Detail entri history
} HistoryEntry;

// Array global untuk menyimpan data buku dan history, beserta jumlahnya
Book books[MAX_BOOKS];         // Array untuk menyimpan objek buku
int book_count = 0;            // Jumlah buku saat ini

HistoryEntry history[MAX_HISTORY]; // Array untuk menyimpan entri history
int history_count = 0;             // Jumlah entri history saat ini

// Deklarasi fungsi-fungsi (prototipe)
void load_books(const char* filename);      // Fungsi untuk memuat data buku dari file
void save_books(const char* filename);      // Fungsi untuk menyimpan data buku ke file
void load_history(const char* filename);    // Fungsi untuk memuat history penjualan dari file
void save_history(const char* filename);    // Fungsi untuk menyimpan history penjualan ke file
void generate_book_code(char* new_code);  // Fungsi untuk menghasilkan kode buku baru
void input_buku();                        // Fungsi untuk menginput data buku baru
void jual_buku();                         // Fungsi untuk menjual buku
void view_buku();                         // Fungsi untuk menampilkan semua data buku
void view_history();                      // Fungsi untuk menampilkan history penjualan
void delete_history_entry();              // Fungsi untuk menghapus entri history
void delete_buku_entry();                 // Fungsi untuk menghapus data buku
void tambah_stok_buku();                  // Fungsi untuk menambah stok buku
void trim_whitespace(char *str);          // Fungsi utilitas untuk menghapus spasi di awal/akhir string


// Fungsi untuk menghapus spasi di awal dan akhir string
void trim_whitespace(char *str) {
  if (str == NULL || *str == '\0') {
    return;
  }
  char *start = str;
  while (isspace((unsigned char)*start)) {
    start++;
  }
  char *end = str + strlen(str) - 1;
  while (end > start && isspace((unsigned char)*end)) {
    end--;
  }
  *(end + 1) = '\0';
  if (start != str) {
    memmove(str, start, strlen(start) + 1);
  }
}


// Fungsi untuk memuat data buku dari file "databuku.txt"
void load_books(const char* filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    // Pesan jika file tidak ditemukan
    printf("Peringatan: File '%s' tidak ditemukan. Memulai dengan daftar buku kosong.\n", filename);
    return;
  }

  char line[MAX_LINE_LENGTH];
  book_count = 0; // Reset jumlah buku sebelum memuat
  // Membaca file baris per baris
  while (fgets(line, sizeof(line), file) != NULL && book_count < MAX_BOOKS) {
    // Menghapus karakter newline jika ada
    line[strcspn(line, "\n")] = 0;
    trim_whitespace(line);
    if (strlen(line) == 0) continue; // Lewati baris kosong

    char *token; // Untuk menyimpan token hasil pemisahan string
    Book b;      // Variabel sementara untuk buku

    // Memisahkan baris berdasarkan koma (,)
    token = strtok(line, ",");
    if (token == NULL) { printf("Melewati baris dengan format salah (kode): %s\n", line); continue; }
    trim_whitespace(token);
    strncpy(b.kode, token, sizeof(b.kode)-1);
    b.kode[sizeof(b.kode)-1] = '\0'; // Pastikan null-terminated

    token = strtok(NULL, ",");
    if (token == NULL) { printf("Melewati baris dengan format salah (nama): %s\n", line); continue; }
    trim_whitespace(token);
    strncpy(b.nama, token, sizeof(b.nama)-1);
    b.nama[sizeof(b.nama)-1] = '\0';

    token = strtok(NULL, ",");
    if (token == NULL) { printf("Melewati baris dengan format salah (jenis): %s\n", line); continue; }
    trim_whitespace(token);
    strncpy(b.jenis, token, sizeof(b.jenis)-1);
    b.jenis[sizeof(b.jenis)-1] = '\0';
    
    token = strtok(NULL, ",");
    if (token == NULL) { printf("Melewati baris dengan format salah (harga): %s\n", line); continue; }
    trim_whitespace(token);
    b.harga = atoi(token); // Konversi harga ke integer
    
    token = strtok(NULL, ","); // Ambil token untuk stok
    if (token == NULL) { 
      printf("Melewati baris dengan format salah (stok tidak ada): %s. Asumsi stok 0.\n", line); 
      b.stok = 0; // Asumsi stok 0 jika tidak ada atau format salah
    } else {
      trim_whitespace(token);
      b.stok = atoi(token); // Konversi stok ke integer
      if (b.stok < 0) {
        printf("Peringatan: Stok negatif (%d) untuk buku %s. Diatur menjadi 0.\n", b.stok, b.nama);
        b.stok = 0;
      }
    }

    // Menambahkan buku yang sudah diparsing ke array books
    books[book_count++] = b;
  }
  fclose(file); // Menutup file
}

// Fungsi untuk menyimpan data buku ke file "databuku.txt"
void save_books(const char* filename) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    perror("Kesalahan saat membuka file untuk menyimpan buku");
    return;
  }
  // Menulis setiap buku ke file
  for (int i = 0; i < book_count; i++) {
    fprintf(file, "%s,%s,%s,%d,%d\n", books[i].kode, books[i].nama, books[i].jenis, books[i].harga, books[i].stok);
  }
  fclose(file);
  printf("Data buku berhasil disimpan ke %s\n", filename);
}

// Fungsi untuk memuat history penjualan dari file "history.txt"
void load_history(const char* filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Peringatan: File '%s' tidak ditemukan. Memulai dengan history kosong.\n", filename);
    return;
  }

  history_count = 0; // Reset jumlah history
  // Membaca file history baris per baris
  while (fgets(history[history_count].entry, MAX_LINE_LENGTH, file) != NULL && history_count < MAX_HISTORY) {
    history[history_count].entry[strcspn(history[history_count].entry, "\n")] = 0; // Hapus newline
    trim_whitespace(history[history_count].entry);
    if(strlen(history[history_count].entry) > 0) { // Hanya proses jika baris tidak kosong
      history_count++;
    }
  }
  fclose(file);
}

// Fungsi untuk menyimpan history penjualan ke file "history.txt"
void save_history(const char* filename) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    perror("Kesalahan saat membuka file untuk menyimpan history");
    return;
  }
  // Menulis setiap entri history ke file
  for (int i = 0; i < history_count; i++) {
    fprintf(file, "%s\n", history[i].entry);
  }
  fclose(file);
  printf("History berhasil disimpan ke %s\n", filename);
}

// Fungsi untuk menghasilkan kode buku baru secara otomatis
void generate_book_code(char* new_code) {
  if (book_count == 0) {
    // Kode default jika belum ada buku
    strcpy(new_code, "BK001");
    return;
  }
  // Mendapatkan kode dari buku terakhir
  const char* last_code_str = books[book_count - 1].kode;
  // Pengecekan format kode buku terakhir
  if (strlen(last_code_str) < 3 || last_code_str[0] != 'B' || last_code_str[1] != 'K') {
    // Fallback jika format kode terakhir tidak sesuai
    sprintf(new_code, "BK%03d", book_count + 1); 
    return;
  }
  int last_number = atoi(last_code_str + 2); // Ambil nomor dari kode terakhir
  sprintf(new_code, "BK%03d", last_number + 1); // Buat kode baru
}

// Fungsi untuk menginput data buku baru
void input_buku() {
  if (book_count >= MAX_BOOKS) {
    printf("Kapasitas buku penuh. Tidak bisa menambah buku baru.\n");
    return;
  }

  printf("\n-- Input Buku Baru --\n");
  Book new_book; // Variabel untuk menyimpan data buku baru
  char buffer[MAX_LINE_LENGTH]; // Buffer untuk input pengguna

  printf("Masukkan Nama Buku: ");
  fgets(buffer, sizeof(buffer), stdin);
  buffer[strcspn(buffer, "\n")] = 0; // Hapus newline dari input
  trim_whitespace(buffer);
  strncpy(new_book.nama, buffer, sizeof(new_book.nama)-1);
  new_book.nama[sizeof(new_book.nama)-1] = '\0'; // Pastikan null-terminated

  printf("Masukkan Jenis Buku: ");
  fgets(buffer, sizeof(buffer), stdin);
  buffer[strcspn(buffer, "\n")] = 0;
  trim_whitespace(buffer);
  strncpy(new_book.jenis, buffer, sizeof(new_book.jenis)-1);
  new_book.jenis[sizeof(new_book.jenis)-1] = '\0';

  // Loop untuk validasi input harga
  while (1) {
    printf("Masukkan Harga Buku: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    trim_whitespace(buffer);
    if (sscanf(buffer, "%d", &new_book.harga) == 1 && new_book.harga > 0) {
      break; // Harga valid, keluar dari loop
    } else {
      printf("Harga harus positif dan berupa angka.\n");
    }
  }

  // Loop untuk validasi input stok awal
  while (1) {
    printf("Masukkan Stok Awal Buku: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    trim_whitespace(buffer);
    if (sscanf(buffer, "%d", &new_book.stok) == 1 && new_book.stok >= 0) {
      break; // Stok valid (boleh 0), keluar dari loop
    } else {
      printf("Stok harus berupa angka non-negatif.\n");
    }
  }
  
  generate_book_code(new_book.kode); // Buat kode buku baru
  books[book_count++] = new_book;    // Tambah buku baru ke array
  printf("Buku '%s' (Stok: %d) berhasil ditambahkan dengan kode %s.\n", new_book.nama, new_book.stok, new_book.kode);
}

// Fungsi untuk menampilkan semua data buku
void view_buku() {
  printf("\n-- Daftar Semua Buku --\n");
  if (book_count == 0) {
    printf("Tidak ada data buku.\n");
    return;
  }
  // Header tabel
  printf("-------------------------------------------------------------------------------------\n");
  printf("%-3s | %-6s | %-30s | %-15s | %-10s | %-5s\n", "No", "Kode", "Nama Buku", "Jenis Buku", "Harga", "Stok");
  printf("-------------------------------------------------------------------------------------\n");
  // Menampilkan setiap buku dalam format tabel
  for (int i = 0; i < book_count; i++) {
    printf("%-3d | %-6s | %-30s | %-15s | %-10d | %-5d\n", i + 1, books[i].kode, books[i].nama, books[i].jenis, books[i].harga, books[i].stok);
  }
  printf("-------------------------------------------------------------------------------------\n");
}

// Fungsi untuk menampilkan history penjualan
void view_history() {
  printf("\n-- History Penjualan --\n");
  if (history_count == 0) {
    printf("Tidak ada history penjualan.\n");
    return;
  }
  // Header tabel history
  printf("-------------------------------------\n");
  printf("%-3s | %s\n", "No", "Detail Transaksi");
  printf("-------------------------------------\n");
  // Menampilkan setiap entri history
  for (int i = 0; i < history_count; i++) {
    printf("%-3d | %s\n", i + 1, history[i].entry);
  }
  printf("-------------------------------------\n");
}

// Fungsi untuk menghapus entri history penjualan
void delete_history_entry() {
  printf("\n-- Hapus History Penjualan --\n");
  if (history_count == 0) {
    printf("Tidak ada history untuk dihapus.\n");
    return;
  }

  view_history(); // Tampilkan history terlebih dahulu
  if (history_count == 0) return; // Jika setelah view_history ternyata kosong

  char buffer[MAX_LINE_LENGTH]; // Buffer untuk input pengguna
  int index_to_delete;          // Index history yang akan dihapus

  // Loop untuk validasi input index
  while (1) {
    printf("Masukkan nomor history yang akan dihapus (1-%d): ", history_count);
    fgets(buffer, sizeof(buffer), stdin);
    trim_whitespace(buffer);
    // Coba konversi input index ke integer
    if (sscanf(buffer, "%d", &index_to_delete) == 1 && index_to_delete >= 1 && index_to_delete <= history_count) {
      break; // Index valid, keluar loop
    } else {
      printf("Input tidak valid. Harap masukkan angka antara 1 dan %d.\n", history_count);
    }
  }

  // Salin entri yang akan dihapus untuk ditampilkan di pesan konfirmasi
  char deleted_entry_copy[MAX_LINE_LENGTH];
  strncpy(deleted_entry_copy, history[index_to_delete - 1].entry, MAX_LINE_LENGTH -1);
  deleted_entry_copy[MAX_LINE_LENGTH-1] = '\0'; // Pastikan null-terminated

  // Geser elemen-elemen setelah index yang dihapus
  for (int i = index_to_delete - 1; i < history_count - 1; i++) {
    history[i] = history[i + 1];
  }
  history_count--; // Kurangi jumlah history
  printf("Data history '%s' berhasil dihapus.\n", deleted_entry_copy);
  save_history("history.txt"); // Simpan perubahan ke file history
}

// Fungsi untuk menghapus data buku
void delete_buku_entry() {
  printf("\n-- Hapus Data Buku --\n");
  if (book_count == 0) {
    printf("Tidak ada data buku untuk dihapus.\n");
    return;
  }

  view_buku(); // Tampilkan daftar buku terlebih dahulu
  if (book_count == 0) return; // Jika setelah view_buku ternyata kosong

  char buffer[MAX_LINE_LENGTH]; // Buffer untuk input pengguna
  int index_to_delete;          // Index buku yang akan dihapus

  // Loop untuk validasi input index
  while (1) {
    printf("Masukkan nomor buku yang akan dihapus (1-%d): ", book_count);
    fgets(buffer, sizeof(buffer), stdin);
    trim_whitespace(buffer);
    // Coba konversi input index ke integer
    if (sscanf(buffer, "%d", &index_to_delete) == 1 && index_to_delete >= 1 && index_to_delete <= book_count) {
      break; // Index valid, keluar loop
    } else {
      printf("Input tidak valid. Harap masukkan angka antara 1 dan %d.\n", book_count);
    }
  }
  
  // Salin nama buku yang akan dihapus untuk pesan konfirmasi
  char deleted_book_name[100];
  strncpy(deleted_book_name, books[index_to_delete - 1].nama, sizeof(deleted_book_name)-1);
  deleted_book_name[sizeof(deleted_book_name)-1] = '\0'; // Pastikan null-terminated

  // Geser elemen-elemen setelah index yang dihapus
  for (int i = index_to_delete - 1; i < book_count - 1; i++) {
    books[i] = books[i + 1];
  }
  book_count--; // Kurangi jumlah buku
  printf("Buku '%s' berhasil dihapus.\n", deleted_book_name);
  // Data buku akan disimpan saat keluar dari aplikasi
}

// Fungsi untuk menambah stok buku yang sudah ada
void tambah_stok_buku() {
  printf("\n-- Tambah Stok Buku --\n");
  if (book_count == 0) {
    printf("Tidak ada data buku untuk ditambahkan stoknya.\n");
    return;
  }

  view_buku(); // Tampilkan daftar buku
  if (book_count == 0) return;

  char buffer[MAX_LINE_LENGTH];
  int book_index_to_update;
  int stock_to_add;

  // Meminta input nomor buku dari pengguna
  while (1) {
    printf("Masukkan nomor buku yang akan ditambah stoknya (1-%d): ", book_count);
    fgets(buffer, sizeof(buffer), stdin);
    trim_whitespace(buffer);
    if (sscanf(buffer, "%d", &book_index_to_update) == 1 && book_index_to_update >= 1 && book_index_to_update <= book_count) {
      break;
    } else {
      printf("Input tidak valid. Harap masukkan nomor buku yang benar antara 1 dan %d.\n", book_count);
    }
  }

  // Meminta jumlah stok yang akan ditambahkan
  while (1) {
    printf("Masukkan jumlah stok yang akan ditambahkan untuk '%s' (Stok saat ini: %d): ", 
           books[book_index_to_update - 1].nama, books[book_index_to_update - 1].stok);
    fgets(buffer, sizeof(buffer), stdin);
    trim_whitespace(buffer);
    if (sscanf(buffer, "%d", &stock_to_add) == 1 && stock_to_add > 0) {
      break;
    } else {
      printf("Jumlah stok tambahan tidak valid. Harap masukkan angka positif.\n");
    }
  }

  // Tambah stok
  books[book_index_to_update - 1].stok += stock_to_add;
  printf("Stok untuk buku '%s' berhasil ditambahkan. Stok baru: %d.\n", 
         books[book_index_to_update - 1].nama, books[book_index_to_update - 1].stok);
}

// Fungsi untuk menjual buku
void jual_buku() {
  printf("\n-- Jual Buku --\n");
  if (book_count == 0) {
    printf("Tidak ada buku yang tersedia untuk dijual.\n");
    return;
  }

  view_buku(); // Tampilkan daftar buku yang tersedia
  if (book_count == 0) { 
    return;
  }

  char buffer[MAX_LINE_LENGTH];
  int book_index_to_sell;
  int quantity;

  // Meminta input nomor buku dari pengguna
  while (1) {
    printf("Masukkan nomor buku yang akan dijual (1-%d): ", book_count);
    fgets(buffer, sizeof(buffer), stdin);
    trim_whitespace(buffer);
    if (sscanf(buffer, "%d", &book_index_to_sell) == 1 && book_index_to_sell >= 1 && book_index_to_sell <= book_count) {
      break;
    } else {
      printf("Input tidak valid. Harap masukkan nomor buku yang benar antara 1 dan %d.\n", book_count);
    }
  }
  
  // Cek stok awal buku yang dipilih
  if (books[book_index_to_sell - 1].stok <= 0) {
    printf("Maaf, stok buku '%s' sudah habis.\n", books[book_index_to_sell - 1].nama);
    return;
  }

  // Meminta input jumlah buku
  while (1) {
    printf("Masukkan jumlah '%s' yang akan dijual (Stok saat ini: %d): ", books[book_index_to_sell - 1].nama, books[book_index_to_sell - 1].stok);
    fgets(buffer, sizeof(buffer), stdin);
    trim_whitespace(buffer);
    if (sscanf(buffer, "%d", &quantity) == 1 && quantity > 0) {
      if (quantity <= books[book_index_to_sell - 1].stok) {
        break; // Jumlah valid dan stok mencukupi
      } else {
        printf("Jumlah yang diminta (%d) melebihi stok yang tersedia (%d).\n", quantity, books[book_index_to_sell - 1].stok);
      }
    } else {
      printf("Jumlah tidak valid. Harap masukkan angka positif.\n");
    }
  }

  // Kurangi stok buku
  books[book_index_to_sell - 1].stok -= quantity;

  // Hitung total harga
  int total_harga = books[book_index_to_sell - 1].harga * quantity;

  // Catat transaksi ke history
  if (history_count < MAX_HISTORY) {
    char history_entry_str[MAX_LINE_LENGTH];
    snprintf(history_entry_str, MAX_LINE_LENGTH, "Terjual: %s (%s), Jumlah: %d, Total Harga: %d", 
             books[book_index_to_sell - 1].nama, books[book_index_to_sell - 1].kode, quantity, total_harga);
    
    strncpy(history[history_count].entry, history_entry_str, MAX_LINE_LENGTH - 1);
    history[history_count].entry[MAX_LINE_LENGTH - 1] = '\0'; 
    history_count++;
    
    save_history("history.txt"); 
    printf("Transaksi berhasil dicatat: %s\n", history_entry_str);
  } else {
    printf("Kapasitas history penuh. Transaksi tidak dapat dicatat.\n");
  }
}

// Fungsi utama program
int main() {
  // Muat data buku dan history saat program dimulai
  load_books("databuku.txt");
  load_history("history.txt");

  char choice_buffer[10]; // Buffer untuk input pilihan menu
  int choice;             // Pilihan menu pengguna

  // Loop utama aplikasi
  while (1) {
    printf("\nSelamat Datang di Aplikasi Toko Buku\n");
    printf("Menu:\n");
    printf("1. Input Data Buku (Tambah Judul Baru)\n");
    printf("2. Jual Buku\n");
    printf("3. Tambah Stok Buku\n");
    printf("4. Lihat History Penjualan\n");
    printf("5. Lihat Data Buku\n");
    printf("6. Hapus History Penjualan\n");
    printf("7. Hapus Data Buku\n");
    printf("8. Keluar\n");
    printf("Pilih menu (1-8): ");

    fgets(choice_buffer, sizeof(choice_buffer), stdin);
    trim_whitespace(choice_buffer); // Hapus spasi berlebih dari input

    // Coba konversi input pilihan ke integer
    if (sscanf(choice_buffer, "%d", &choice) != 1) {
      choice = -1; // Jika input bukan angka, set ke nilai tidak valid
    }
    
    // Struktur switch untuk menangani pilihan menu
    switch (choice) {
      case 1:
        input_buku();
        break;
      case 2:
        jual_buku();
        break;
      case 3:
        tambah_stok_buku();
        break;
      case 4:
        view_history();
        break;
      case 5:
        view_buku();
        break;
      case 6:
        delete_history_entry();
        break;
      case 7:
        delete_buku_entry();
        break;
      case 8:
        save_books("databuku.txt");
        printf("Terima kasih telah menggunakan aplikasi. Data telah disimpan.\n");
        return 0; // Keluar dari program
      default:
        // Pesan jika pilihan tidak valid
        printf("Pilihan tidak valid. Silakan coba lagi.\n");
    }
  }

  return 0; // Seharusnya tidak pernah tercapai
} 