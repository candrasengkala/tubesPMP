#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

void baca_data_dokter(const char* nama_file, Dokter dokter[], int* jumlah_dokter) {
    FILE* file = fopen(nama_file, "r");
    if (!file) {
        printf("Gagal membuka file %s\n", nama_file);
        return;
    }

    char baris[200];
    fgets(baris, sizeof(baris), file); // Lewati header

    *jumlah_dokter = 0;
    while (fgets(baris, sizeof(baris), file)) {
        Dokter* d = &dokter[*jumlah_dokter];
        sscanf(baris, "%d,%49[^,],%d,%d,%d,%d",
               &d->id, d->nama, &d->max_shift_per_minggu,
               &d->preferensi[PAGI], &d->preferensi[SIANG], &d->preferensi[MALAM]);

        d->total_shift = 0;
        d->pelanggaran = 0;
        memset(d->jumlah_shift_per_jenis, 0, sizeof(d->jumlah_shift_per_jenis));
        memset(d->jadwal, 0, sizeof(d->jadwal));

        (*jumlah_dokter)++;
    }

    fclose(file);
}

void generate_jadwal(Dokter dokter[], int jumlah_dokter, int jadwal[SHIFT_PER_HARI][MAX_HARI]) {
        
}

void simpan_kalendar_csv(const char* nama_file, int jadwal[SHIFT_PER_HARI][MAX_HARI], Dokter dokter[], int jumlah_dokter) {
    FILE* file = fopen(nama_file, "w");
    if (!file) {
        printf("Gagal menulis ke file %s\n", nama_file);
        return;
    }
    fprintf(file, "hari,shift,dokter\n");
    for (int hari = 0; hari < MAX_HARI; hari++) {
        for (int shift = 0; shift < SHIFT_PER_HARI; shift++) {
            fprintf(file, "%d,%d,", hari + 1, shift + 1);
            int id = jadwal[shift][hari];
            if (id != -1) {
                fprintf(file, "%d", id);
            }
            fprintf(file, "\n");
        }
    }

    fclose(file);
}

void simpan_kalendar_dokter_csv(const char* nama_file, Dokter dokter[], int jumlah_dokter) {
    FILE* file = fopen(nama_file, "w");
    if (!file) {
        printf("Gagal menulis ke file %s\n", nama_file);
        return;
    }

    fprintf(file, "id,jumlah_pagi,jumlah_siang,jumlah_malam,preferensi_pagi,preferensi_siang,preferensi_malam,shift_mingguan,pelanggaran\n");
    for (int i = 0; i < jumlah_dokter; i++) {
        Dokter* d = &dokter[i];
        fprintf(file, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                d->id,
                d->jumlah_shift_per_jenis[PAGI],
                d->jumlah_shift_per_jenis[SIANG],
                d->jumlah_shift_per_jenis[MALAM],
                d->preferensi[PAGI],
                d->preferensi[SIANG],
                d->preferensi[MALAM],
                d->max_shift_per_minggu,
                d->pelanggaran);
    }

    fclose(file);
}