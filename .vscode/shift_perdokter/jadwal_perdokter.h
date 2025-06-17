#ifndef shift_perdokter
#define shift_perdokter

void jadwal_perdokter();
    /*
    Input       : kalendar.csv, nama dokter
    Output      : jadwal si nama dokter
    Deskripsi   : 
    1. Membaca file kalendar.csv
    2. Meminta nama dokter yang ingin ditampilkan jadwalnya
    3. Mencari semua baris pada file yang sesuai dengan nama dokter tersebut
    4. Menampilkan semua shift, hari, dan minggu hari tersebut
    5. Format tampilan:
    Hari:[hari], shift[shift]
    */
const char* hari_to_hari(int hari);
    /*
    ganti hari dari keterangan angka ke senin selasa dst
    */
const char* shift_to_shift (int shift);
    /*
    ganti shift dari keterangan angka ke keterangan waktu pagi siang malam
    */
#endif
