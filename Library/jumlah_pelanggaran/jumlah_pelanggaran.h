void pelanggaran_all();
    /*
    Input       : kalender_dokter.csv, data_dokter.csv
    Output      : menampilkan semua pelanggaran shift dokter
    Deskripsi   :
    1. Membaca file data_dokter.csv
    2. Ambil id dan nama dokter dari file tersebut
    3. Membaca file kalender_dokter.csv
    4. Ambil data pelanggaran setiap id dokter
    5. Menampilkan semua pelanggaran shift dokter dengan format
    "[id]. [nama dokter]: [jumlah pelanggaran] pelanggaran shift"
    */

void pelanggaran_person();
/*
    Input       : kalender_dokter.csv, data_dokter.csv, nama dokter
    Output      : menampilkan jumlah pelanggaran shift milik dokter yang ingin dicari
    Deskripsi   : 
    1. Menerima input nama dokter yang ingin dicari
    2. Membaca file data_dokter.csv dan mendapatkan ID dokter tersebut
    3. Mencari data pelanggaran shift di kalender_dokter.csv
    4. Menampilkan data pelanggaran shift milik dokter tersebut.
*/

