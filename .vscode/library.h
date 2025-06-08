void jadwal_harian();
/*
Input           : kalendar.csv, input hari (1-7)
Output          : jadwal dokter per hari yang diprint
Deskripsi:
1. Membaca file kalendar.csv
2. Mencari semua baris dengan hari yang sesuai
3. Menampilkan shift dan dokter di hari tersebut
4. Format: "Shift [nama] - Dokter: [nama dokter]" atau disesuaikan aja formatnya
*/

void jadwal_mingguan();
/*
Input           : kalendar.csv
Output          : jadwal dokter per minggu yang diprint
Deskripsi:
1. Membaca file kalendar.csv
2. Mengurutkan data berdasarkan hari (1-7)
3. Untuk setiap hari, tampilkan semua shift dan dokternya
4. Format tampilan per hari dengan header nama hari
5. Menampilkan lengkap Senin-Minggu dalam satu tampilan
*/

void jadwal_bulanan();
/*
Input           : kalendar.csv
Output          : jadwal dokter per bulan yang diprint
Deskripsi:
1. Membaca file kalendar.csv
2. Menampilkan jadwal mingguan sebanyak 4 kali (4 minggu)
3. Memberikan header "Minggu ke-1", "Minggu ke-2", dst
4. Setiap minggu menampilkan pola hari 1-7 yang sama
5. Total menampilkan 30 hari (4 minggu - 7 hari + 1 minggu - 2 hari)
*/