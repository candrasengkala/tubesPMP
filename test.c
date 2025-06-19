#include "Library/DataDokter/DataDokter.h"

int main(void){
    char buffermode[100];
    char mode;
    for(;;){
        printf("Input mode: \nHapus Data Dokter [h]\nTampilkan data[t]\nTambah data[a]\n");
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
    }
    return 0;
}