#include "Library/DataDokter/DataDokter.h"
#include "Library/JadwalGeneration/JadwalGeneration.h"
#include "Library/JadwalPerWaktu/JadwalPerWaktu.h"
#include "Library/jumlah_pelanggaran/jumlah_pelanggaran.h"
#include "Library/shift_perdokter/shift_perdokter.h"


#define PATH_DOKTER "database/data_dokter.csv"
#define PATH_KALENDAR "database/kalendar.csv"
int main(void){
    char buffermode[100];
    char mode;
    for(;;){
        printf("Input mode: \nHapus Data Dokter [h]\nTampilkan Data Dokter[t]\nTambah data[a]\nGenerate Jadwal[g]\nLihat Pelanggaran[p]\nShift Per-Dokter[s]\nJadwal Perwaktu[j]\nExit [e]\n>");
        fgets(buffermode, 5, stdin);
        mode = buffermode[0];
        if(mode == 'h'){
            hapus_dokter();       
        }
        else if(mode == 't'){
            tampilkan_dokter();
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
        }
        else if(mode == 'p'){    
            char buffermode[100];
            for(;;){
                printf("Input Lihat Pelanggaran: \nPelanggaran Perdokter [d]\nPelanggaran Keseluruhan[k]\nExit[e]\n");
                fgets(buffermode, 5, stdin);
                char mode = buffermode[0];
                if(mode == 'd'){
                    pelanggaran_person();
                }
                else if(mode == 'k'){
                    pelanggaran_all();
                }
                else if(mode == 'e'){
                    break;
                }
            }
        }
        else if(mode == 's'){
            jadwal_perdokter();
        }
        else if(mode == 'j'){
            for(;;){
                printf("Lihat Jadwal: \nJadwal Per-Bulan [h]\nJadwal Per-Minggu[m]\nJadwal Per-Hari[h]\nExit[e]\n");
                fgets(buffermode, 5, stdin);
                char mode = buffermode[0];

                Dokter_JadwalPerWaktu* data_dokter;
                JadwalEntry* data_jadwal;
                readfile_data_dokter(PATH_DOKTER, data_dokter, MAX_DOCTORS);
                readfile_kalendar(PATH_KALENDAR, data_jadwal, MAX_JADWAL_ENTRIES);
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
    }
    return 0;
}