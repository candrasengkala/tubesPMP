#include "DataDokter.h"

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

