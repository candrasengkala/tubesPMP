#include "DataDokter.h"
#include <unistd.h>
#include <string.h>

int check_name_empty(char name[]){
    if(strlen(name) != 0) return 1;
    return 0;
}
int number_validity(int num){
    if ((num == 1) || (num == 0)) return 1;
    return 0;
}
int shift_validity(int num){
    if (num >= 0) return 1; //shift nol dianggap valid karena kalau alokasinya nol berarti gak mau kerja sama sekali.
    return 0;
}

int longest_name(){
    FILE *fptr;
    fptr = fopen("database/data_dokter.csv", "r");
    int len = 0;
    char line [100];
    while(fgets(line, 200, fptr) != NULL){
        char nama_line[100];
        strtok(line, ",");
        strcpy(nama_line, strtok(NULL, ","));
        if (strlen(nama_line) > len) len = strlen(nama_line);
    }
    fclose(fptr);
    return len;
}
void tampilkan_dokter(){
    int _longest_name = longest_name();
    if(_longest_name <= strlen("Preferensi Malam")) _longest_name = strlen("Preferensi Malam");

    
    char line[100];
    FILE *fptr;
    fptr = fopen("database/data_dokter.csv", "r");
    int i = 0;
    while(fgets(line, 200, fptr) != NULL){
        if(i == 0){
           printf("%s %*s %s %*s %s %*s %s %*s %s %*s %s %*s\n", 
                "ID", (_longest_name - strlen("ID")), "",
                "Nama", (_longest_name - strlen("Nama")), "",
                "Shift Mingguan", (_longest_name - strlen("Shift Mingguan")), "",
                "Preferensi Pagi", (_longest_name - strlen("Preferensi Pagi")), "",
                "Preferensi Siang", (_longest_name - strlen("Preferensi Siang")), "",
                "Preferensi Malam", (_longest_name - strlen("Preferensi Malam")),  "" 
                ); 
            i += 1;
        }
        else{
            char* token;
            token = strtok(line, ",");
            int col = 1; //nomor kolom, supaya space gk ada setelah new line char.
            while(token){
                printf("%s", token);
                if(col != 6){
                    for(int i = 0; i <=  abs(_longest_name - strlen(token)) + 1; i++){
                        printf(" ");
                    }
                }
                token = strtok(NULL, ",");
                col += 1;
            }
        }
    }
    fclose(fptr);
}
int check_exist(char name[], FILE *fptr){
    int returnedvalue = 1;
    char line[100];
    while(fgets(line, 200, fptr) != NULL){
        char nama_line[100];
        strtok(line, ",");
        strcpy(nama_line, strtok(NULL, ","));
        if (strcmp(name, nama_line) == 0){
            returnedvalue = 0;
        }
    }
    return returnedvalue;
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
    int found = 0;
    while(fgets(line, 200, fptr) != NULL){
        char nama_line[100];
        char line_to_be_put[100];
        strcpy(line_to_be_put, line);
        strtok(line, ",");
        strcpy(nama_line, strtok(NULL, ","));
        if (strcmp(nama_line, buffernama) == 0) found = 1;
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
    (found == 1) ? printf("Data berhasil dihapus!\n") : printf("Data tidak ditemukan!\n");
    fclose(fptr);
    sleep(2);
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
    int add_truth = 0; 
    ((check_name_empty(namebuffer) == 1) && (check_exist(namebuffer, fptr) == 1) && (shift_validity(shiftperweek) == 1) && (number_validity(preferensi1) == 1) && (number_validity(preferensi2) == 1) && (number_validity(preferensi3) == 1))? fprintf(fptr, "%d,%s,%d,%d,%d,%d\n", genereated_id, namebuffer, shiftperweek, preferensi1, preferensi2, preferensi3) : printf("Input invalid atau nama sudah ada!\n");
    sleep(2);
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