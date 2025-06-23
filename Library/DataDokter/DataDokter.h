#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int longest_name();

void tambah_dokter();
/*
Input       : File data_dokter.csv
Output      : File data_dokter.csv
Deskripsi   :
Menambah data dokter dengan validasi input yang dilakukan oleh fungsi helper jumlah_k
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
Fail data_dokter.csv
Output      :
Jumlah dokter pada suatu sistem.
Deskripsi   :
Digunakan untuk mengetahui jumlah dokter pada file data_dokter.csv. Fungsi ini dipanggil bila dibutuhkan oleh fungsi-fungsi lain.
Perhitungan dilakukan dengan menghitung line.
*/
int jumlah_kalendar();
/*
Input       :
Fail kalendar.csv
Output      :
Jumlah hari pada kalendar.
Deskripsi   :
Digunakan untuk mengetahui jumlah hari dan sesi pada file kelendar.csv. Fungsi ini dipanggil bila dibutuhkan oleh fungsi-fungsi lain.
Perhitungan dilakukan dengan menghitung line.
*/