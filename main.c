#include <stdio.h>
#include "src/library.h"

int main() {
    Dokter daftar_dokter[MAX_DOKTER];
    int jumlah_dokter = 0;
    int jadwal[SHIFT_PER_HARI][MAX_HARI];

    // 1. Baca data dokter dari CSV
    baca_data_dokter("database/data_dokter.csv", daftar_dokter, &jumlah_dokter);

    // 2. Buat jadwal otomatis
    generate_jadwal(daftar_dokter, jumlah_dokter, jadwal);

    // 3. Simpan ke kalendar.csv dan kalendar_dokter.csv
    simpan_kalendar_csv("database/kalendar.csv", jadwal, daftar_dokter, jumlah_dokter);
    simpan_kalendar_dokter_csv("database/kalender_dokter.csv", daftar_dokter, jumlah_dokter);

    // 4. Notifikasi selesai
    printf("Jadwal berhasil dibuat dan disimpan!\n");
    return 0;
}