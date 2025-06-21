#include "DataDokter.h"

void tampilkan_dokter(){
    char line[100];
    FILE *fptr;
    fptr = fopen("database/data_dokter.csv", "r");

    int i = 0;
    while(fgets(line, 200, fptr) != NULL){
        if(i == 0){
            printf("ID               Nama             Shift Mingguan   Preferensi Pagi  Preferensi Siang Preferensi Malam \n");  
            i += 1;
        }
        else{
            char* token;
            token = strtok(line, ",");
            int col = 1; //nomor kolom, supaya space gk ada setelah new line char.
            while(token){
                printf("%s", token);
                if(col != 6){
                    for(int i = 0; i <  strlen("ID               ") - strlen(token); i++){
                        printf(" ");
                    }
                }
                token = strtok(NULL, ",");
                col += 1;
            }
            //if (token == NULL){
            //    printf("\n");
            //}
        }
    }
    fclose(fptr);
}

void hapus_dokter(){
    char buffernama[100];
    char listdokter[100][100];
    char line[100];

    FILE *fptr;
    fptr = fopen("database/data_dokter.csv", "r");
    
    printf("Nama dokter yang dihapus: ");
    fgets(buffernama, 100, stdin);
    buffernama[strcspn(buffernama, "\n")] = 0;

    int i = 0;
    while(fgets(line, 200, fptr) != NULL){
        char nama_line[100];
        char line_to_be_put[100];

        strcpy(line_to_be_put, line);
        strtok(line, ",");
        strcpy(nama_line, strtok(NULL, ","));
        //printf("%s %d\n", nama_line, strcmp(buffernama, nama_line));

        if(strcmp(nama_line, buffernama) != 0){
            strcpy(listdokter[i], line_to_be_put);
            i += 1;
        }
    }
    fclose(fptr);

    fptr = fopen("database/data_dokter.csv", "w");
    int baru = 0;
    while(baru < i){
        fprintf(fptr, listdokter[baru]);
        baru += 1;
    }
    fclose(fptr);
}



int generate_id(){
    int generatedid = 0;
    FILE *fptr;
    fptr = fopen("database/data_dokter.csv", "r");
    char line[200];
    int i = 0;
    int maxval = 0;
    while(fgets(line, 200, fptr) != NULL){
        int nilainow = atoi(strtok(line, ","));
        if(nilainow > maxval){
            maxval = nilainow;
        }
    }
    generatedid = maxval + 1;
    return generatedid;
}

void tambah_dokter(){
    char namebuffer[100];
    char shiftbuffer[20];

    char preferensibuffer1[100];
    char preferensibuffer2[100];
    char preferensibuffer3[100];

    int preferensi1;
    int preferensi2;
    int preferensi3;

    int shiftperweek = 0;
    
    FILE *fptr;
    
    fptr = fopen("database/data_dokter.csv", "a+");
    printf("Nama Dokter: ");
    fgets(namebuffer, 100, stdin);
    namebuffer[strcspn(namebuffer, "\n")] = 0;
    printf("Jumlah shift mingguan: ");
    fgets(shiftbuffer, 20, stdin);
    shiftperweek = atoi(shiftbuffer);
    
    printf("Preferensi Pagi (1/0): ");
    fgets(preferensibuffer1, 100, stdin);
    printf("Preferensi Siang (1/0): ");
    fgets(preferensibuffer2, 100, stdin);
    printf("Preferensi Malam (1/0): ");
    fgets(preferensibuffer3, 100, stdin);

    preferensi1 = atoi(preferensibuffer1);
    preferensi2 = atoi(preferensibuffer2);
    preferensi3 = atoi(preferensibuffer3);

    int genereated_id = generate_id();
    fprintf(fptr, "%d,%s,%d,%d,%d,%d\n", genereated_id, namebuffer, shiftperweek, preferensi1, preferensi2, preferensi3);
    fclose(fptr);
}

int jumlah_dokter(){
    char line[100];
    FILE *fptr;
    fptr = fopen("database/data_dokter.csv", "r");
    int i = -1;
    while(fgets(line, 200, fptr) != NULL){
        i += 1;
    }
    fclose(fptr);
    return i;
}

int jumlah_kalendar(){
    char line[100];
    FILE *fptr;
    fptr = fopen("database/kalendar.csv", "r");
    int i = -1;
    while(fgets(line, 200, fptr) != NULL){
        i += 1;
    }
    fclose(fptr);
    return i;
}