#include "Library/DataDokter/DataDokter.h"
#include "Library/JadwalGeneration/JadwalGeneration.h"
#include "Library/JadwalPerWaktu/JadwalPerWaktu.h"
#include "Library/jumlah_pelanggaran/jumlah_pelanggaran.h"
#include "Library/shift_perdokter/shift_perdokter.h"
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#define PATH_DOKTER "database/data_dokter.csv"
#define PATH_KALENDAR "database/kalendar.csv"
int main(void){
    char buffermode[100];
    char mode;
    for(;;){
        //system("cls");
        printf("%s %s %s %s %s %s %s %s %s %s", 
                "Selamat Datang!\n",
                "Menu                           input (lowercase) \n",
                "Hapus Data Dokter              [h]\n",
                "Tampilkan Data Dokter          [t]\n",
                "Tambah data                    [a]\n",
                "Generate Jadwal                [g]\n",
                "Lihat Pelanggaran              [p]\n",
                "Shift Per-Dokter               [s]\n",
                "Jadwal Perwaktu                [j]\n",
                "Exit                           [e]\n>");
        fgets(buffermode, 5, stdin);
        mode = buffermode[0];
        if(mode == 'h'){
            hapus_dokter();       
        }
        else if(mode == 't'){
            tampilkan_dokter();
            printf("Press any key to escape\n");
            getchar();
        }
        else if(mode == 'a'){
            tambah_dokter();
        }
        else if(mode == 'e'){
            break;
        }
        else if(mode == 'g'){
            Dokter daftar_dokter[MAX_DOKTER];
            int jumlahdokter = jumlah_dokter();
            int jadwal[SHIFT_PER_HARI][MAX_HARI];
            baca_data_dokter("database/data_dokter.csv", daftar_dokter, &jumlahdokter);
            generate_jadwal(daftar_dokter, jumlahdokter, jadwal);
            simpan_kalendar_csv("database/kalendar.csv", jadwal, daftar_dokter, jumlahdokter);
            simpan_kalendar_dokter_csv("database/kalender_dokter.csv", daftar_dokter, jumlahdokter);
            printf("Jadwal berhasil dibuat dan disimpan!\n");
            sleep(2);
        }
        else if(mode == 'p'){    
            char buffermode[100];
            for(;;){
                printf("Input Lihat Pelanggaran: \nPelanggaran Perdokter [d]\nPelanggaran Keseluruhan[k]\nExit[e]\n>");
                fgets(buffermode, 5, stdin);
                char mode = buffermode[0];
                if(mode == 'd'){
                    pelanggaran_person();
                    printf("Press any key to escape\n");
                    getchar();
                    break;
                }
                else if(mode == 'k'){
                    pelanggaran_all();
                    printf("Press any key to escape\n");
                    getchar();
                    break;
                }
                else if(mode == 'e'){
                    break;
                }
            }
        }
        else if(mode == 's'){
            jadwal_perdokter();
            printf("Press any key to escape\n");
            getchar();
        }
        else if(mode == 'j'){
            for(;;){
                printf("Lihat Jadwal: \nJadwal Per-Bulan [b]\nJadwal Per-Minggu[m]\nJadwal Per-Hari[h]\nExit[e]\n>");
                fgets(buffermode, 5, stdin);
                char mode = buffermode[0];

                Dokter_JadwalPerWaktu data_dokter[MAX_DOCTORS];
                JadwalEntry data_jadwal[MAX_JADWAL_ENTRIES];
                readfile_data_dokter("database/data_dokter.csv", data_dokter, MAX_DOCTORS);
                readfile_kalendar("database/kalendar.csv", data_jadwal, MAX_JADWAL_ENTRIES);
                int panjang_kalendar = jumlah_kalendar();
                int panjang_dokter = jumlah_dokter();
                if(mode == 'h'){
                    jadwal_harian(data_jadwal, panjang_kalendar, data_dokter, panjang_dokter);
                }
                else if(mode == 'm'){
                    jadwal_mingguan(data_jadwal, panjang_kalendar, data_dokter, panjang_dokter);
                }
                else if(mode == 'b'){
                    jadwal_bulanan(data_jadwal, panjang_kalendar, data_dokter, panjang_dokter);
                }
                else if(mode == 'e'){
                    break;
                }
            }
        }
        system("cls");
    }
    return 0;
}