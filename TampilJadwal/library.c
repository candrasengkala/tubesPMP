#include "library.h"

int baca_data_dokter(const char* filename, Dokter dokters[], int max_dokter) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Tidak dapat membuka file %s\n", filename);
        return 0;
    }
    char line[MAX_LINE];
    int count = 0;
    fgets(line, sizeof(line), file); // Skip header
    while (fgets(line, sizeof(line), file) && count < max_dokter) {
        sscanf(line, "%d,%99[^,],%d,%d,%d,%d", 
            &dokters[count].id, 
            dokters[count].nama,
            &dokters[count].shift_mingguan, 
            &dokters[count].preferensi_pagi, 
            &dokters[count].preferensi_siang, 
            &dokters[count].preferensi_malam);
        count++;
    }
    fclose(file);
    return count;
}

int baca_data_kalendar(const char* filename, JadwalEntry jadwal[], int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Tidak dapat membuka file %s\n", filename);
        return 0;
    }
    char line[MAX_LINE];
    int count = 0;
    fgets(line, sizeof(line), file); // Skip header
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        char* token_hari = strtok(line, ",");
        char* token_shift = strtok(NULL, ",");
        char* token_dokter = strtok(NULL, "\n");

        if (token_hari) jadwal[count].hari = atoi(token_hari);
        if (token_shift) jadwal[count].shift = atoi(token_shift);
        
        if (token_dokter) {
            jadwal[count].jumlah_dokter = 0;
            char* id_str = strtok(token_dokter, ";");
            while (id_str && jadwal[count].jumlah_dokter < MAX_DOCTORS) {
                jadwal[count].dokter_ids[jadwal[count].jumlah_dokter++] = atoi(id_str);
                id_str = strtok(NULL, ";");
            }
        }
        count++;
    }
    fclose(file);
    return count;
}


const char* cari_nama_dokter(int id, const Dokter dokters[], int jumlah_dokter) {
    for (int i = 0; i < jumlah_dokter; i++) {
        if (dokters[i].id == id) {
            return dokters[i].nama;
        }
    }
    return "N/A";
}

const char* nama_hari(int hari) { // hari 1-7
    switch (hari) {
        case 1: return "Senin";
        case 2: return "Selasa";
        case 3: return "Rabu";
        case 4: return "Kamis";
        case 5: return "Jumat";
        case 6: return "Sabtu";
        case 7: return "Minggu";
        default: return "Hari Tidak Valid";
    }
}

const char* nama_shift(int shift) {
    switch (shift) {
        case 1: return "Pagi";
        case 2: return "Siang";
        case 3: return "Malam";
        default: return "Shift Tidak Valid";
    }
}

// FUNGSI HELPER BARU untuk menghindari duplikasi kode
void tampilkan_jadwal_untuk_hari(int hari_mingguan, const JadwalEntry jadwal[], int total_jadwal, const Dokter dokters[], int total_dokter) {
    int jadwal_ditemukan = 0;
    for (int i = 0; i < total_jadwal; i++) {
        if (jadwal[i].hari == hari_mingguan) {
            printf("  Shift %-6s - Dokter: ", nama_shift(jadwal[i].shift));
            for (int j = 0; j < jadwal[i].jumlah_dokter; j++) {
                const char* nama = cari_nama_dokter(jadwal[i].dokter_ids[j], dokters, total_dokter);
                printf("%s%s", nama, (j < jadwal[i].jumlah_dokter - 1) ? ", " : "");
            }
            printf("\n");
            jadwal_ditemukan = 1;
        }
    }
    if (!jadwal_ditemukan) {
        printf("  Tidak ada jadwal.\n");
    }
}

// IMPLEMENTASI FUNGSI UTAMA

// Menampilkan jadwal untuk hari ke 1-30
void jadwal_harian(const JadwalEntry jadwal[], int total_jadwal, const Dokter dokters[], int total_dokter) {
    int hari_input;
    printf("Masukkan hari dalam sebulan (1-30): ");
    scanf("%d", &hari_input);

    if (hari_input < 1 || hari_input > 30) {
        printf("Error: Hari harus antara 1-30\n");
        return;
    }

    // Konversi hari 1-30 menjadi hari 1-7 (Senin-Minggu)
    int hari_mingguan = (hari_input - 1) % 7 + 1;
    
    printf("\n=== JADWAL HARI KE-%d (%s) ===\n", hari_input, nama_hari(hari_mingguan));
    tampilkan_jadwal_untuk_hari(hari_mingguan, jadwal, total_jadwal, dokters, total_dokter);
}

// Menampilkan jadwal untuk minggu ke 1-5
void jadwal_mingguan(const JadwalEntry jadwal[], int total_jadwal, const Dokter dokters[], int total_dokter) {
    int minggu_input;
    printf("Masukkan minggu (1-5): ");
    scanf("%d", &minggu_input);
    if (minggu_input < 1 || minggu_input > 5) {
        printf("Error: Minggu harus antara 1-5\n");
        return;
    }
    printf("\n=== JADWAL MINGGU KE-%d ===\n", minggu_input);
    
    int jumlah_hari_dalam_minggu = (minggu_input == 5) ? 2 : 7;
    for (int i = 1; i <= jumlah_hari_dalam_minggu; i++) {
        printf("\n--- %s ---\n", nama_hari(i));
        tampilkan_jadwal_untuk_hari(i, jadwal, total_jadwal, dokters, total_dokter);
    }
}

// Menampilkan jadwal sebulan penuh (30 hari)
void jadwal_bulanan(const JadwalEntry jadwal[], int total_jadwal, const Dokter dokters[], int total_dokter) {
    printf("\n=== JADWAL BULANAN (30 Hari) ===\n");
    for (int hari_ke = 1; hari_ke <= 30; ++hari_ke) {
        int minggu = (hari_ke - 1) / 7 + 1;
        int hari_mingguan = (hari_ke - 1) % 7 + 1;

        if (hari_mingguan == 1) {
            printf("\n========== MINGGU KE-%d ==========\n", minggu);
        }
        printf("\n--- Hari ke-%d (%s) ---\n", hari_ke, nama_hari(hari_mingguan));
        tampilkan_jadwal_untuk_hari(hari_mingguan, jadwal, total_jadwal, dokters, total_dokter);
    }
}
