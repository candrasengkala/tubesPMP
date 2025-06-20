#ifndef jadwal_dokter_h
#define jadwal_dokter_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256
#define MAX_NAME 100
#define MAX_DOCTORS 10
#define MAX_JADWAL_ENTRIES 100

// Struktur untuk menyimpan data dokter
typedef struct {
    int id;
    char nama[MAX_NAME];
    int shift_mingguan;
    int preferensi_pagi;
    int preferensi_siang;
    int preferensi_malam;
} Dokter;

// Struktur untuk menyimpan data jadwal
typedef struct {
    int hari; // Hari ke-1 sampai 30
    int shift; // 1: Pagi, 2: Siang, 3: Malam
    int dokter_ids[MAX_DOCTORS];
    int jumlah_dokter;
} JadwalEntry;

// --- FUNGSI HELPER ---
int readfile_data_dokter(const char* filename, Dokter dokters[], int max_dokter);
int readfile_kalendar(const char* filename, JadwalEntry jadwal[], int max_entries);
const char* cari_dokter(int id, const Dokter dokters[], int jumlah_dokter);
const char* nama_hari(int hari);
const char* nama_shift(int shift);
void tampil_jadwal_harian(int hari_ke, const JadwalEntry jadwal[], int total_jadwal, const Dokter dokters[], int total_dokter);

// --- FUNGSI UTAMA ---
void jadwal_harian(const JadwalEntry jadwal[], int total_jadwal, const Dokter dokters[], int total_dokter);
void jadwal_mingguan(const JadwalEntry jadwal[], int total_jadwal, const Dokter dokters[], int total_dokter);
void jadwal_bulanan(const JadwalEntry jadwal[], int total_jadwal, const Dokter dokters[], int total_dokter);

#endif
