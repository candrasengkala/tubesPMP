#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shift_perdokter.h"
const char* tanggal_to_hari (int hari){
    switch(hari) {
        case 0: return "Senin";
        case 1: return "Selasa";
        case 2: return "Rabu";
        case 3: return "Kamis";
        case 4: return "Jumat";
        case 5: return "Sabtu";
        case 6: return "Minggu";
    }
}

const char* shift_to_shift (int shift){
    switch (shift)
    {
        case 1: return "Pagi";
        case 2: return "Siang";
        case 3: return "Malam";
    }
}
void jadwal_perdokter(){
    char line[128], line1[128],nama_dokter[20];
    int found=0, id_dokter=-1;
    printf("Masukkan nama dokter yang ingin dicari> ");
    fgets(nama_dokter, sizeof(nama_dokter), stdin);
    nama_dokter[strcspn(nama_dokter, "\n")] = 0; // hapus newline
    FILE *data_dokter=fopen("database/data_dokter.csv", "r");
    fgets(line1, sizeof(line1), data_dokter);//agar header tidak terbaca
    while (fgets(line1, sizeof(line1), data_dokter))
    {
        line1[strcspn(line1, "\n")]=0;
        char *id=strtok(line1, ",");
        char *nama=strtok(NULL, ",");
        if (strcmp(nama, nama_dokter) == 0){
            id_dokter=atoi(id);
            found=1;
            break;
        }
        
    }
    fclose(data_dokter);
    if (!found)
    {   
        printf("Nama dokter tidak ditemukan");
        return;
    }
    FILE *kalendar=fopen("database/kalendar.csv", "r");
    fgets(line, sizeof(line), kalendar);//agar header tidak terbaca
    int minggu=0;
    while (fgets(line, sizeof(line), kalendar))
    {
        line[strcspn(line, "\n")] = 0;
        char *hari = strtok(line, ",");
        int tanggal=atoi(hari);
        char *shift = strtok(NULL, ",");
        int shift_int=atoi(shift);
        char *dokter_list = strtok(NULL, ",");
        char *dokter_token = strtok(dokter_list, ";");
    
        while (dokter_token != NULL) {
            if (atoi(dokter_token) == id_dokter) {
                int hari_angka=((tanggal-1)%7);
                minggu=(((tanggal-1)/7)+1);
                printf("Minggu ke: %d, Hari: %s, Shift: %s\n", minggu, tanggal_to_hari (hari_angka), shift_to_shift(shift_int));
            }
            dokter_token = strtok(NULL, ";");
             
        
        }
    }
    fclose(kalendar);
}
