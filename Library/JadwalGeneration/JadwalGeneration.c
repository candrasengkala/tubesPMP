#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "JadwalGeneration.h"

void baca_data_dokter(const char* nama_file, Dokter dokter[], int* jumlah_dokter) {
    FILE* file = fopen(nama_file, "r");
    if (!file) {
        printf("Gagal membuka file %s\n", nama_file);
        return;
    }

    char baris[200];
    fgets(baris, sizeof(baris), file); // Lewati header

    *jumlah_dokter = 0;
    while (fgets(baris, sizeof(baris), file)) {
        Dokter* d = &dokter[*jumlah_dokter];
        sscanf(baris, "%d,%49[^,],%d,%d,%d,%d",
               &d->id, d->nama, &d->max_shift_per_minggu,
               &d->preferensi[PAGI], &d->preferensi[SIANG], &d->preferensi[MALAM]);

        d->total_shift = 0;
        d->pelanggaran = 0;
        memset(d->jumlah_shift_per_jenis, 0, sizeof(d->jumlah_shift_per_jenis));
        memset(d->jadwal, 0, sizeof(d->jadwal));

        (*jumlah_dokter)++;
    }

    fclose(file);
}

void generate_jadwal(Dokter dokter[], int jumlah_dokter, int jadwal[SHIFT_PER_HARI][MAX_HARI]) {
    // Inisialisasi jadwal dengan 0 (tidak ada dokter yang dijadwalkan)
    for (int shift = 0; shift < SHIFT_PER_HARI; shift++) {
        for (int hari = 0; hari < MAX_HARI; hari++) {
            jadwal[shift][hari] = 0; // 0 berarti tidak ada dokter yang dijadwalkan
        }
    }
    
    // Inisialisasi data dokter
    for (int i = 0; i < jumlah_dokter; i++) {
        dokter[i].total_shift = 0;
        dokter[i].pelanggaran = 0;
        dokter[i].shift_mingguan_current = 0;
        for (int j = 0; j < 3; j++) {
            dokter[i].jumlah_shift_per_jenis[j] = 0;
        }
        for (int j = 0; j < SHIFT_PER_HARI * MAX_HARI; j++) {
            dokter[i].jadwal[j] = 0;
        }
        
        //  Hitung jumlah preferensi
        dokter[i].jumlah_preferensi = dokter[i].preferensi[PAGI] + dokter[i].preferensi[SIANG] + dokter[i].preferensi[MALAM];
    }
    
    // Step 1: Isi jadwal berdasarkan preferensi dokter
    // Prioritaskan dokter dengan preferensi yang lebih sedikit
    for (int shift_type = PAGI; shift_type <= MALAM; shift_type++) {
        for (int hari = 0; hari < MAX_HARI; hari++) { // Proses semua 30 hari
            
            // Hitung shift mingguan untuk minggu berjalan
            int minggu_mulai = (hari / 7) * 7;
            int minggu_akhir = minggu_mulai + 7;
            if (minggu_akhir > MAX_HARI) minggu_akhir = MAX_HARI;
            for (int i = 0; i < jumlah_dokter; i++) {
                dokter[i].shift_mingguan_current = 0;
                for (int d = minggu_mulai; d < minggu_akhir; d++) {
                    for (int s = 0; s < SHIFT_PER_HARI; s++) {
                        int idx = s * MAX_HARI + d;
                        if (dokter[i].jadwal[idx] == 1) { //kalau udah pernah ngambil
                            dokter[i].shift_mingguan_current++; //shift_mingguan_current ditambah karena dokternya sudah pernah di-assign.
                        }
                    }
                }
            }
            
            // Prioritas pengisian dokter berdasarkan preferensi
            // Prioritaskan dokter dengan preferensi yang lebih sedikit
            for (int pref_priority = 1; pref_priority <= 3; pref_priority++) {
                if (jadwal[shift_type][hari] != 0) break; // Jika shift sudah berisikan minimal 1 dokter
                
                int best_doctor = -1;
                int min_shifts_this_week = 999;
                
                // Cari dokter yang sesuai dengan preferensi dan batas mingguan
                for (int i = 0; i < jumlah_dokter; i++) {
                    // Cek apakah dokter memiliki preferensi yang sesuai
                    // Skip dokter yang tidak memiliki jumlah_preferensi sesuai dengan prioritas saat ini
                    if (dokter[i].jumlah_preferensi != pref_priority) continue;
                    
                    // Cek apakah dokter memiliki preferensi untuk shift ini
                    if (dokter[i].preferensi[shift_type] != 1) continue;
                    
                    // Cek apakah dokter sudah mencapai batas shift mingguan
                    // Skip dokter yang sudah mencapai batas shift mingguan
                    if (dokter[i].shift_mingguan_current >= dokter[i].max_shift_per_minggu) continue;
                    
                    // Pilih dokter dengan jumlah shift mingguan terkecil
                    // Ini untuk memastikan dokter yang paling sedikit bertugas di minggu ini dipilih
                    if (dokter[i].shift_mingguan_current < min_shifts_this_week) {
                        min_shifts_this_week = dokter[i].shift_mingguan_current;
                        best_doctor = i;
                    }
                }
                
                //  Jika ada dokter yang ditemukan sesuai dengan preferensi
                //  Assign dokter ke jadwal
                if (best_doctor != -1) {
                    jadwal[shift_type][hari] = dokter[best_doctor].id;
                    
                    // Update data dokter tersebut
                    int day_shift_index = shift_type * MAX_HARI + hari;
                    dokter[best_doctor].jadwal[day_shift_index] = 1; //
                    dokter[best_doctor].total_shift++;
                    dokter[best_doctor].jumlah_shift_per_jenis[shift_type]++;
                    dokter[best_doctor].shift_mingguan_current++; // Increment running count for the week
                    break; // Lanjut ke hari berikutnya setelah menemukan dokter untuk shift ini
                }
            }
        }
    }
    
    // Step 2: Isi jadwal untuk shift yang belum terisi (value = 0)
    // Jika masih ada shift yang belum terisi, cari dokter yang tersedia
    for (int shift_type = PAGI; shift_type <= MALAM; shift_type++) {
        for (int hari = 0; hari < MAX_HARI; hari++) { //  Proses semua 30 hari
            if (jadwal[shift_type][hari] == 0) { // Shift ini belum terisi
                
                // Hitung shift mingguan untuk minggu berjalan
                int minggu_mulai = (hari / 7) * 7;
                int minggu_akhir = minggu_mulai + 7;
                if (minggu_akhir > MAX_HARI) minggu_akhir = MAX_HARI;

                for (int i = 0; i < jumlah_dokter; i++) {
                    dokter[i].shift_mingguan_current = 0;
                    for (int d = minggu_mulai; d < minggu_akhir; d++) {
                        for (int s = 0; s < SHIFT_PER_HARI; s++) {
                            int idx = s * MAX_HARI + d;
                            if (dokter[i].jadwal[idx] == 1) {
                                dokter[i].shift_mingguan_current++; //untuk mengetahui di minggu tersebut *sudah berapa shift*
                            }
                        }
                    }
                }
                
                int best_doctor = -1;
                int min_current_shifts = 999;
                
                // Cari dokter yang tersedia (belum melebihi batas mingguan)
                for (int i = 0; i < jumlah_dokter; i++) {
                    // Cek apakah dokter sudah mencapai batas shift mingguan
                    // Skip dokter yang sudah mencapai batas shift mingguan
                    if (dokter[i].shift_mingguan_current >= dokter[i].max_shift_per_minggu) continue;
                    
                    // Pilih dokter dengan jumlah shift mingguan terkecil
                    // Ini untuk memastikan dokter yang paling sedikit bertugas di minggu ini dipilih
                    if (dokter[i].shift_mingguan_current < min_current_shifts) {
                        min_current_shifts = dokter[i].shift_mingguan_current;
                        best_doctor = i;
                    }
                }
                
                // Jika ada dokter yang ditemukan, assign ke jadwal
                if (best_doctor != -1) {
                    jadwal[shift_type][hari] = dokter[best_doctor].id;
                    
                    // Update data dokter tersebut
                    int day_shift_index = shift_type * MAX_HARI + hari;
                    dokter[best_doctor].jadwal[day_shift_index] = 1;
                    dokter[best_doctor].total_shift++;
                    dokter[best_doctor].jumlah_shift_per_jenis[shift_type]++;
                    dokter[best_doctor].shift_mingguan_current++;
                    
                    // Cek apakah dokter tersebut melanggar preferensi
                    // Jika dokter tidak memiliki preferensi untuk shift ini, tambahkan pelanggaran
                    if (dokter[best_doctor].preferensi[shift_type] == 0) {
                        dokter[best_doctor].pelanggaran++;
                    }
                }
                // Kalau tidak ada dokter yang tersedia, shift ini tetap kosong
            }
        }
    }

    // Step 3: Tambahkan dokter tambahan ke shift yang sudah ada
    // Prioritaskan dokter yang belum mencapai shift maksimal dan punya preferensi
    for (int shift_type = PAGI; shift_type <= MALAM; shift_type++) {
        for (int hari = 0; hari < MAX_HARI; hari++) {
            
            // Reset shift mingguan di awal minggu
            if (hari % 7 == 0) {
                for (int i = 0; i < jumlah_dokter; i++) {
                    dokter[i].shift_mingguan_current = 0;
                    // Hitung ulang shift untuk minggu ini berdasarkan jadwal yang sudah ada
                    for (int d = hari; d < hari + 7 && d < MAX_HARI; d++) {
                        for (int s = 0; s < SHIFT_PER_HARI; s++) {
                            int day_shift_index = s * MAX_HARI + d;
                            if (dokter[i].jadwal[day_shift_index] == 1) {
                                dokter[i].shift_mingguan_current++;
                            }
                        }
                    }
                }
            }
            
            // Cari dokter yang bisa ditambahkan ke shift ini
            // Urutkan berdasarkan prioritas: yang paling sedikit shift dulu
            int dokter_tersedia[MAX_DOKTER];
            int jumlah_tersedia = 0;
            
            // Kumpulkan dokter yang memenuhi syarat
            for (int i = 0; i < jumlah_dokter; i++) {
                // Syarat untuk Step 3:
                // 1. Harus punya preferensi untuk shift ini (tidak boleh pelanggaran)
                if (dokter[i].preferensi[shift_type] != 1) continue;
                
                // 2. Belum mencapai batas shift mingguan
                if (dokter[i].shift_mingguan_current >= dokter[i].max_shift_per_minggu) continue;
                
                // 3. Belum terjadwal di shift dan hari ini
                int day_shift_index = shift_type * MAX_HARI + hari;
                if (dokter[i].jadwal[day_shift_index] == 1) continue;
                
                dokter_tersedia[jumlah_tersedia++] = i;
            }
            
            // Sorting dokter tersedia berdasarkan jumlah shift mingguan (ascending)
            for (int i = 0; i < jumlah_tersedia - 1; i++) {
                for (int j = i + 1; j < jumlah_tersedia; j++) {
                    if (dokter[dokter_tersedia[i]].shift_mingguan_current > 
                        dokter[dokter_tersedia[j]].shift_mingguan_current) {
                        // Swap bubble sort
                        int temp = dokter_tersedia[i];
                        dokter_tersedia[i] = dokter_tersedia[j];
                        dokter_tersedia[j] = temp;
                    }
                }
            }
            
            // Tambahkan dokter ke shift (bisa lebih dari satu dokter per shift)
            for (int idx = 0; idx < jumlah_tersedia; idx++) {
                int i = dokter_tersedia[idx];
                
                // Double check kondisi karena mungkin berubah setelah assignment sebelumnya
                if (dokter[i].shift_mingguan_current >= dokter[i].max_shift_per_minggu) continue;
                
                // Assign dokter ke shift ini
                int day_shift_index = shift_type * MAX_HARI + hari;
                dokter[i].jadwal[day_shift_index] = 1;
                dokter[i].total_shift++;
                dokter[i].jumlah_shift_per_jenis[shift_type]++;
                dokter[i].shift_mingguan_current++;
                
                // Tidak ada pelanggaran di Step 3 karena kita sudah cek preferensi
            }
        }
    }
}

void simpan_kalendar_csv(const char* nama_file, int jadwal[SHIFT_PER_HARI][MAX_HARI], Dokter dokter[], int jumlah_dokter) {
    FILE* file = fopen(nama_file, "w");
    if (!file) {
        printf("Gagal menulis ke file %s\n", nama_file);
        return;
    }
    fprintf(file, "hari,shift,dokter\n");
    
    for (int hari = 0; hari < MAX_HARI; hari++) {
        for (int shift = 0; shift < SHIFT_PER_HARI; shift++) {
            fprintf(file, "%d,%d,", hari + 1, shift + 1);
            
            // Cari semua dokter yang dijadwalkan di shift dan hari ini
            int first_doctor = 1;
            for (int i = 0; i < jumlah_dokter; i++) {
                int day_shift_index = shift * MAX_HARI + hari;
                if (dokter[i].jadwal[day_shift_index] == 1) {
                    if (!first_doctor) {
                        fprintf(file, ";"); // Separator untuk multiple dokter
                    }
                    fprintf(file, "%d", dokter[i].id);
                    first_doctor = 0;
                }
            }
            
            fprintf(file, "\n");
        }
    }

    fclose(file);
}

void simpan_kalendar_dokter_csv(const char* nama_file, Dokter dokter[], int jumlah_dokter) {
    FILE* file = fopen(nama_file, "w");
    if (!file) {
        printf("Gagal menulis ke file %s\n", nama_file);
        return;
    }

    fprintf(file, "id,jumlah_pagi,jumlah_siang,jumlah_malam,preferensi_pagi,preferensi_siang,preferensi_malam,shift_mingguan,pelanggaran\n");
    for (int i = 0; i < jumlah_dokter; i++) {
        Dokter* d = &dokter[i];
        fprintf(file, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                d->id,
                d->jumlah_shift_per_jenis[PAGI],
                d->jumlah_shift_per_jenis[SIANG],
                d->jumlah_shift_per_jenis[MALAM],
                d->preferensi[PAGI],
                d->preferensi[SIANG],
                d->preferensi[MALAM],
                d->max_shift_per_minggu,
                d->pelanggaran);
    }

    fclose(file);
}