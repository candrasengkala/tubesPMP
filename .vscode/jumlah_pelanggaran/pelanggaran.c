#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pelanggaran.h"

void pelanggaran_all() {
    FILE *data_dokter = fopen("../database/data_dokter.csv", "r");
    FILE *kalender_dokter = fopen("../database/kalender_dokter.csv", "r");
    if (!data_dokter || !kalender_dokter) {
        printf("Gagal membuka file.\n");
        return;
    }
    char line[128], line1[128];
    fgets(line, sizeof(line), data_dokter); // skip header
    fgets(line1, sizeof(line1), kalender_dokter); // skip header

    while (fgets(line, sizeof(line), data_dokter)) {
        line[strcspn(line, "\n")] = 0;
        char *id = strtok(line, ",");
        char *nama = strtok(NULL, ",");
        int id_dokter = atoi(id);

        // cari jumlah_pelanggaran di file kalender_dokter.csv
        int jumlah_pelanggaran = 0;
        while (fgets(line1, sizeof(line1), kalender_dokter)) {
            line1[strcspn(line1, "\n")] = 0;
            char *id_kal = strtok(line1, ",");
            for (int i = 0; i < 7; i++) strtok(NULL, ",");//skip kolom lain
            char *pel = strtok(NULL, ",");
            if (atoi(id_kal) == id_dokter) {
                jumlah_pelanggaran = atoi(pel);
                break;
            }
        }
        printf("%d. %s: %d pelanggaran shift\n", id_dokter, nama, jumlah_pelanggaran);
    }
    fclose(data_dokter);
    fclose(kalender_dokter);
}

void pelanggaran_person() {
    char line[128], line1[128], nama_dokter[64];
    int found = 0, id_dokter = -1;
    printf("Masukkan nama dokter yang ingin dicari> ");
    fgets(nama_dokter, sizeof(nama_dokter), stdin);
    nama_dokter[strcspn(nama_dokter, "\n")] = 0;
    FILE *data_dokter = fopen("../database/data_dokter.csv", "r");
    fgets(line1, sizeof(line1), data_dokter);
    while (fgets(line1, sizeof(line1), data_dokter)) {
        line1[strcspn(line1, "\n")] = 0;
        char *id = strtok(line1, ",");
        char *nama = strtok(NULL, ",");
        if (strcmp(nama, nama_dokter) == 0) {
            id_dokter = atoi(id);
            found = 1;
            break;
        }
    }
    fclose(data_dokter);
    if (!found) {
        printf("Nama dokter tidak ditemukan\n");
        return;
    }
    FILE *kalender_dokter = fopen("../database/kalender_dokter.csv", "r");
    fgets(line, sizeof(line), kalender_dokter);
    int jumlah_pelanggaran = 0;
    while (fgets(line, sizeof(line), kalender_dokter)) {
        line[strcspn(line, "\n")] = 0;
        char *id = strtok(line, ",");
        for (int i = 0; i < 7; i++) strtok(NULL, ",");
        char *pel = strtok(NULL, ",");
        if (atoi(id) == id_dokter) {
            jumlah_pelanggaran = atoi(pel);
            break;
        }
    }
    fclose(kalender_dokter);
    printf("%d. %s: %d pelanggaran shift\n", id_dokter, nama_dokter, jumlah_pelanggaran);
}
