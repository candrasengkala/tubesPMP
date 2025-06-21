#include "JadwalPerWaktu.h" 

// --- FUNGSI UTILITAS  ---
int readfile_data_dokter(const char* filename, Dokter_JadwalPerWaktu dokters[], int max_dokter) {
    FILE* file = fopen(filename, "r"); // Membuka file untuk dibaca
    if (file == NULL) {
        fprintf(stderr, "File data dokter tidak bisa diakses!\n", filename);
        return 0; // Gagal membaca
    }
    char lineBuffer[MAX_LINE]; // Buffer untuk membaca setiap baris
    int docsReadCount = 0;     // Penghitung jumlah dokter yang sudah terbaca
    // Lewati baris pertama, biasanya ini header CSV. Kita gak butuh data headernya.
    fgets(lineBuffer, sizeof(lineBuffer), file);

    // Baca setiap baris sampai akhir file atau sampai array dokters penuh
    while (fgets(lineBuffer, sizeof(lineBuffer), file) != NULL && docsReadCount < max_dokter) {
        // Gunakan sscanf untuk mem-parsing data dari baris.
        // Pastikan formatnya sesuai: ID,Nama,ShiftMingguan,PrefPagi,PrefSiang,PrefMalam
        int result = sscanf(lineBuffer, "%d,%99[^,],%d,%d,%d,%d",
                            &dokters[docsReadCount].id,
                            dokters[docsReadCount].nama,
                            &dokters[docsReadCount].shift_mingguan,
                            &dokters[docsReadCount].preferensi_pagi,
                            &dokters[docsReadCount].preferensi_siang,
                            &dokters[docsReadCount].preferensi_malam);

        // Pastikan semua 6 item berhasil dibaca
        if (result == 6) {
            docsReadCount++; // Lanjut ke dokter berikutnya
        } else {
            fprintf(stderr, "Baris data dokter tidak lengkap atau format salah: %s", lineBuffer);
        }
    }

    fclose(file); 
    return docsReadCount; // berapa banyak dokter yang berhasil dibaca
}

int readfile_kalendar(const char* filename, JadwalEntry jadwal[], int max_entries) {
    FILE* calendarFile = fopen(filename, "r"); // Buka file kalender
    if (!calendarFile) {
        fprintf(stderr, "Tidak bisa membuka file kalender!\n", filename);
        return 0; 
    }
    char rowBuffer[MAX_LINE]; // Buffer untuk menampung satu baris dari file
    int currentEntryIndex = 0; // Indeks untuk menunjuk ke entri jadwal saat ini
    // Abaikan baris pertama (header) karena biasanya cuma judul kolom
    fgets(rowBuffer, sizeof(rowBuffer), calendarFile);
    // Loop selama masih ada baris yang bisa dibaca dan belum melebihi kapasitas array
    while (fgets(rowBuffer, sizeof(rowBuffer), calendarFile) != NULL && currentEntryIndex < max_entries) {
        // strtok memisahkan string berdasarkan delimiter (koma atau newline).
        // Hati-hati dengan strtok, karena dia memodifikasi string aslinya.
        char* dayToken = strtok(rowBuffer, ",");
        char* shiftToken = strtok(NULL, ",");
        char* doctorListToken = strtok(NULL, "\n"); // Ambil sisa baris sampai newline
        // Pastikan token hari dan shift ada sebelum dikonversi
        if (dayToken) {
            jadwal[currentEntryIndex].hari = atoi(dayToken); // Konversi string hari ke integer
        }
        if (shiftToken) {
            jadwal[currentEntryIndex].shift = atoi(shiftToken); // Konversi string shift ke integer
        }
        jadwal[currentEntryIndex].jumlah_dokter = 0; // Reset jumlah dokter untuk entri ini
        // Jika ada daftar dokter, parsing ID-IDnya
        if (doctorListToken) {
            char* doctorIdToken = strtok(doctorListToken, ";"); // Dokter dipisahkan dengan semicolon
            while (doctorIdToken != NULL && jadwal[currentEntryIndex].jumlah_dokter < MAX_DOCTORS) {
                // Konversi ID dokter dari string ke integer dan simpan
                jadwal[currentEntryIndex].dokter_ids[jadwal[currentEntryIndex].jumlah_dokter++] = atoi(doctorIdToken);
                doctorIdToken = strtok(NULL, ";"); // Lanjut ke ID dokter berikutnya
            }
        }
        currentEntryIndex++; // Pindah ke entri jadwal berikutnya
    }
    fclose(calendarFile); 
    return currentEntryIndex; // total entri yang berhasil diisi
}

const char* cari_dokter(int id, const Dokter_JadwalPerWaktu dokters[], int jumlah_dokter) {
    for (int i = 0; i < jumlah_dokter; ++i) {
        if (dokters[i].id == id) {
            return dokters[i].nama; // Kembalikan namanya.
        }
    }
    return "Tidak Ditemukan"; 
}

const char* nama_hari(int hari) {
    const char* daftarHariLengkap[] = {
        "Tidak Valid", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu", "Minggu"};
    if (hari >= 1 && hari <= 7) {
        // Jika angka hari valid (antara 1 sampai 7)
        return daftarHariLengkap[hari];
    } else {
        // Jika angka hari tidak valid (di luar 1-7)
        return daftarHariLengkap[0]; // Mengembalikan "Tidak Valid"
    }
}

const char* nama_shift(int shift){
    switch (shift) {
        case 1: return "Pagi";
        case 2: return "Siang";
        case 3: return "Malam";
        default: return "Shift Tidak Dikenal"; 
    }
}

// --- FUNGSI UNTUK MENAMPILKAN JADWAL ---
void tampil_jadwal_harian(int hari_ke, const JadwalEntry jadwal[], int total_jadwal, const Dokter_JadwalPerWaktu dokters[], int total_dokter) {
    int foundScheduleForDay = 0; 
    // Loop melalui semua entri jadwal yang ada
    for (int i = 0; i < total_jadwal; ++i) {
        if (jadwal[i].hari == hari_ke) {
            // Kita menemukan entri jadwal untuk hari yang diminta!
            printf("  Shift %-6s - Dokter_JadwalPerWaktu: ", nama_shift(jadwal[i].shift));
            for (int j = 0; j < jadwal[i].jumlah_dokter; ++j) {
                // Cari nama dokter berdasarkan ID dan tampilkan
                const char* doctorName = cari_dokter(jadwal[i].dokter_ids[j], dokters, total_dokter);
                printf("%s", doctorName); 
                // Cek apakah ini bukan dokter terakhir dalam daftar
                if (j < jadwal[i].jumlah_dokter - 1) {
                    printf(", "); // Jika bukan, cetak koma dan spasi setelah namanya
                }
            }
            printf("\n"); // Baris baru setelah daftar dokter untuk shift ini
            foundScheduleForDay = 1; 
        }
    }
    if (!foundScheduleForDay) {
        printf("  Tidak ada jadwal yang terdaftar untuk hari ini.\n");
    }
}

void jadwal_harian(const JadwalEntry jadwal[], int total_jadwal, const Dokter_JadwalPerWaktu dokters[], int total_dokter) {
    int requestedDay;
    printf("Mau lihat jadwal hari ke berapa (1-30)? ");
    if (scanf("%d", &requestedDay) != 1) {
        // Penanganan error kalau inputnya bukan angka
        printf("Input tidak valid. Mohon masukkan angka hari yang benar.\n");
        // Hapus sisa buffer input untuk mencegah masalah di scanf berikutnya
        while (getchar() != '\n');
        return;
    }
    if (requestedDay < 1 || requestedDay > 30) {
        printf("Nomor hari tidak valid. Harusnya antara 1 sampai 30 saja, ya.\n");
        return;
    }
    // Hitung nama hari dalam seminggu berdasarkan nomor hari dalam bulan..
    int dayOfWeekIndex = (requestedDay - 1) % 7 + 1;
    printf("\n=== JADWAL HARI KE-%d (%s) ===\n", requestedDay, nama_hari(dayOfWeekIndex));
    tampil_jadwal_harian(requestedDay, jadwal, total_jadwal, dokters, total_dokter);
}

void jadwal_mingguan(const JadwalEntry jadwal[], int total_jadwal, const Dokter_JadwalPerWaktu dokters[], int total_dokter) {
    int requestedWeek;
    printf("Mau lihat jadwal minggu ke berapa (1-5)? ");
    if (scanf("%d", &requestedWeek) != 1) {
        printf("Input tidak valid. Mohon masukkan angka minggu yang benar.\n");
        while (getchar() != '\n');
        return;
    }
    if (requestedWeek < 1 || requestedWeek > 5) {
        printf("Input minggu tidak valid. Harusnya antara 1 sampai 5 saja.\n");
        return;
    }
    printf("\n=== JADWAL MINGGU KE-%d ===\n", requestedWeek);
    // Minggu ke-5  hanya 2 hari (untuk total 30 hari dalam sebulan).
    // Minggu lainnya 7 hari.
    int daysInWeek; // Deklarasikan variabelnya dulu
    if (requestedWeek == 5) {
        // Jika minggu yang diminta adalah minggu ke-5
        daysInWeek = 2; // Berarti hanya ada 2 hari dalam minggu itu (untuk total 30 hari dalam sebulan)
    } 
    else {
        // Untuk minggu lainnya (minggu 1, 2, 3, 4)
        daysInWeek = 7; // Berarti ada 7 hari penuh
    }
    // Tentukan hari awal untuk minggu yang diminta.
    int startDay = (requestedWeek - 1) * 7 + 1;
    for (int i = 0; i < daysInWeek; ++i) {
        int currentDay = startDay + i;
        // Kembali hitung indeks hari dalam seminggu (Senin-Minggu)
        int dayOfWeekIndex = (currentDay - 1) % 7 + 1;
        printf("\n--- Hari ke-%d (%s) ---\n", currentDay, nama_hari(dayOfWeekIndex));
        tampil_jadwal_harian(currentDay, jadwal, total_jadwal, dokters, total_dokter);
    }
}

void jadwal_bulanan(const JadwalEntry jadwal[], int total_jadwal, const Dokter_JadwalPerWaktu dokters[], int total_dokter) {
    printf("\n=== SELURUH JADWAL BULANAN ===\n"); // Judul besar untuk seluruh bulan
    for (int currentDay = 1; currentDay <= 30; ++currentDay) {
        // Hitung minggu ke berapa hari ini jatuh (untuk pengelompokan visual)
        int currentWeek = (currentDay - 1) / 7 + 1;
        // Hitung indeks hari dalam seminggu (1=Senin, 7=Minggu)
        int dayOfWeekIndex = (currentDay - 1) % 7 + 1;
        // Jika ini awal minggu, cetak header minggu.
        if (dayOfWeekIndex == 1) {
            printf("\n===== MINGGU KE-%d =====\n", currentWeek);
        }
        // Tampilkan header untuk hari ini
        printf("\n--- Hari ke-%d (%s) ---\n", currentDay, nama_hari(dayOfWeekIndex));
        // Panggil fungsi pembantu untuk menampilkan detail jadwal hari ini
        tampil_jadwal_harian(currentDay, jadwal, total_jadwal, dokters, total_dokter);
    }
}
