#include "JadwalPerWaktu.h"

// Membaca data dokter dari file CSV
int readfile_data_dokter(const char* filename, Dokter_JadwalPerWaktu dokters[], int max_dokter) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "File tidak bisa dibuka\n", filename);
        return 0;
    }
    char buffer[MAX_LINE];
    int jumlah_terbaca = 0;
    fgets(buffer, sizeof(buffer), file); // Lewati header
    while (fgets(buffer, sizeof(buffer), file) && jumlah_terbaca < max_dokter) {
        sscanf(buffer, "%d,%99[^,],%d,%d,%d,%d",
            &dokters[jumlah_terbaca].id,
            dokters[jumlah_terbaca].nama,
            &dokters[jumlah_terbaca].shift_mingguan,
            &dokters[jumlah_terbaca].preferensi_pagi,
            &dokters[jumlah_terbaca].preferensi_siang,
            &dokters[jumlah_terbaca].preferensi_malam);
        jumlah_terbaca++;
    }
    fclose(file);
    return jumlah_terbaca;
}

// Memuat jadwal shift dokter dari file
int readfile_kalendar(const char* filename, JadwalEntry jadwal[], int max_entries) {
    FILE* input = fopen(filename, "r");
    if (!input) {
        fprintf(stderr, "Tidak dapat membuka: %s\n", filename);
        return 0;
    }
    char baris[MAX_LINE];
    int entri_ke = 0;
    fgets(baris, sizeof(baris), input); // Abaikan header
    while (fgets(baris, sizeof(baris), input) && entri_ke < max_entries) {
        char* ptr_hari = strtok(baris, ",");
        char* ptr_shift = strtok(NULL, ",");
        char* ptr_daftar = strtok(NULL, "\n");
        if (ptr_hari) jadwal[entri_ke].hari = atoi(ptr_hari);
        if (ptr_shift) jadwal[entri_ke].shift = atoi(ptr_shift);
        jadwal[entri_ke].jumlah_dokter = 0;
        if (ptr_daftar) {
            char* token = strtok(ptr_daftar, ";");
            while (token && jadwal[entri_ke].jumlah_dokter < MAX_DOCTORS) {
                jadwal[entri_ke].dokter_ids[jadwal[entri_ke].jumlah_dokter++] = atoi(token);
                token = strtok(NULL, ";");
            }
        }
        entri_ke++;
    }
    fclose(input);
    return entri_ke;
}

// Mencari nama dokter berdasarkan ID
const char* cari_dokter(int id, const Dokter_JadwalPerWaktu dokters[], int jumlah_dokter) {
    for (int i = 0; i < jumlah_dokter; i++) {
        if (dokters[i].id == id) {
            return dokters[i].nama;
        }
    }
    return "Tidak Ditemukan";
}

// Konversi hari numerik ke nama hari
const char* nama_hari(int hari) {
    const char* daftar_hari[] = {
        "Tidak Valid", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu", "Minggu"
    };
    return (hari >= 1 && hari <= 7) ? daftar_hari[hari] : daftar_hari[0];
}

// Konversi angka shift ke label
const char* nama_shift(int shift) {
    switch (shift) {
        case 1: return "Pagi";
        case 2: return "Siang";
        case 3: return "Malam";
        default: return "Shift Tidak Dikenal";
    }
}

// Menampilkan jadwal untuk satu hari tertentu
void tampil_jadwal_harian(int hari_ke, const JadwalEntry jadwal[], int total_jadwal, const Dokter_JadwalPerWaktu dokters[], int total_dokter) {
    int ditemukan = 0;
    for (int i = 0; i < total_jadwal; i++) {
        if (jadwal[i].hari == hari_ke) {
            printf("  Shift %-6s - Dokter_JadwalPerWaktu: ", nama_shift(jadwal[i].shift));
            for (int j = 0; j < jadwal[i].jumlah_dokter; j++) {
                const char* nama = cari_dokter(jadwal[i].dokter_ids[j], dokters, total_dokter);
                printf("%s%s", nama, (j < jadwal[i].jumlah_dokter - 1) ? ", " : "");
            }
            printf("\n");
            ditemukan = 1;
        }
    }
    if (!ditemukan) {
        printf("  Tidak ada jadwal untuk hari ini.\n");
    }
}

// Jadwal satu hari berdasarkan input pengguna
void jadwal_harian(const JadwalEntry jadwal[], int total_jadwal, const Dokter_JadwalPerWaktu dokters[], int total_dokter) {
    int hari_input;
    printf("Masukkan hari (1-30): ");
    scanf("%d", &hari_input);
    if (hari_input < 1 || hari_input > 30) {
        printf("Hari tidak valid. Harus antara 1–30.\n");
        return;
    }
    int indeks_hari = (hari_input - 1) % 7 + 1;
    printf("\n=== JADWAL HARI KE-%d (%s) ===\n", hari_input, nama_hari(indeks_hari));
    tampil_jadwal_harian(hari_input, jadwal, total_jadwal, dokters, total_dokter);
}

// Jadwal dalam seminggu berdasarkan minggu ke-n
void jadwal_mingguan(const JadwalEntry jadwal[], int total_jadwal, const Dokter_JadwalPerWaktu dokters[], int total_dokter) {
    int minggu_input;
    printf("Masukkan minggu ke (1-5): ");
    scanf("%d", &minggu_input);
    if (minggu_input < 1 || minggu_input > 5) {
        printf("Input tidak valid. Harus antara 1–5.\n");
        return;
    }
    printf("\n=== JADWAL MINGGU KE-%d ===\n", minggu_input);
    int jumlah_hari = (minggu_input == 5) ? 2 : 7;
    int mulai_dari = (minggu_input - 1) * 7 + 1;
    for (int i = 0; i < jumlah_hari; i++) {
        int hari_ke = mulai_dari + i;
        int indeks_hari = (hari_ke - 1) % 7 + 1;
        printf("\n--- Hari ke-%d (%s) ---\n", hari_ke, nama_hari(indeks_hari));
        tampil_jadwal_harian(hari_ke, jadwal, total_jadwal, dokters, total_dokter);
    }
}

// Menampilkan seluruh jadwal selama 30 hari
void jadwal_bulanan(const JadwalEntry jadwal[], int total_jadwal, const Dokter_JadwalPerWaktu dokters[], int total_dokter) {
    printf("\n=== JADWAL BULANAN ===\n");
    for (int hari_ke = 1; hari_ke <= 30; hari_ke++) {
        int minggu_ke = (hari_ke - 1) / 7 + 1;
        int indeks_hari = (hari_ke - 1) % 7 + 1;
        if (indeks_hari == 1) {
            printf("\n===== MINGGU KE-%d =====\n", minggu_ke);
        }
        printf("\n--- Hari ke-%d (%s) ---\n", hari_ke, nama_hari(indeks_hari));
        tampil_jadwal_harian(hari_ke, jadwal, total_jadwal, dokters, total_dokter);
    }
}
