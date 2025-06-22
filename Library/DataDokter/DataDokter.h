#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void tambah_dokter();
/*
Input       :

Output      :

Deskripsi   :

*/
void hapus_dokter();
/*
Input       :

Output      :

Deskripsi   :

*/
void tampilkan_dokter();
/*
Input       :

Output      :

Deskripsi   :

*/

//Fungsi Helper
int generate_id();
/*
Input       :

Output      :

Deskripsi   :

*/
int check_exist(char name[], FILE *fptr);
/*
Input       :

Output      :

Deskripsi   :

*/
int check_name_empty(char name[]);
/*
Input       :

Output      :

Deskripsi   :

*/
int number_validity(int num);
/*
Input       :

Output      :

Deskripsi   :

*/

//Fungsi Lain Untuk Integrasi.
int jumlah_dokter();
/*
Input       :

Output      :

Deskripsi   :

*/
int jumlah_kalendar();
/*
Input       :

Output      :

Deskripsi   :

*/