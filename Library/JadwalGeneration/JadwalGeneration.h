#define MAX_DOKTER 100      // Maksimal jumlah dokter, bisa disesuaikan
#define MAX_HARI 30         // Maksimal jumlah hari dalam sebulan (fix untuk kalendar)
#define SHIFT_PER_HARI 3    // Jumlah shift per hari (Pagi, Siang, Malam)

typedef enum { PAGI, SIANG, MALAM } Shift;  // enum untuk jenis shift (Pagi = 0, Siang = 1, Malam = 2)

typedef struct {
    int id;                                 // ID dokter
    char nama[50];                          // Nama dokter
    int max_shift_per_minggu;               // Maksimal shift per minggu
    int preferensi[3];                      // Preferensi shift: 1 = mau, 0 = tidak
    int total_shift;                        // Total shift yang sudah dijadwalkan
    int jumlah_shift_per_jenis[3];          // Jumlah shift PAGI, SIANG, MALAM
    int pelanggaran;                        // Jumlah pelanggaran preferensi
    int jadwal[SHIFT_PER_HARI * MAX_HARI];  // Penandaan shift (1 jika bertugas)
    int jumlah_preferensi;                  // Jumlah shift yang disukai (1, 2, atau 3)
    int shift_mingguan_current;             // Shift yang sudah dijadwalkan minggu ini
} Dokter;


void baca_data_dokter(const char* nama_file, Dokter dokter[], int* jumlah_dokter);
/**
 * baca_data_dokter
 * ----------------
 * Membaca file CSV data dokter dan mengisi array Dokter.
 * 
 * Input:
 *   - nama_file: nama file CSV, misalnya "data_dokter.csv"
 *   - dokter[]: array untuk menyimpan hasil pembacaan data dokter
 *   - jumlah_dokter: pointer ke integer untuk menyimpan jumlah dokter yang dibaca
 * 
 * Output:
 *   - dokter[] yang sudah berisikan data dokter
 *   - jumlah_dokter diupdate dengan jumlah dokter yang berhasil dibaca
 * 
 * Langkah kerja:
 *   1. Buka file dengan fopen().
 *   2. Lewati header dengan fgets() pertama. (misalnya "id,nama,shift_mingguan,preferensi_pagi,preferensi_siang,preferensi_malam").
 *   3. Loop baca setiap baris dengan fgets().
 *   4. Parse data dengan sscanf(): id,nama,max_shift_per_minggu,preferensi_pagi,preferensi_siang,preferensi_malam ke dalam struct Dokter.
 *   5. Inisialisasi sub-data lain: total_shift=0, pelanggaran=0, clear arrays jadwal dan jumlah_shift_per_jenis.
 *   6. Increment jumlah_dokter.
 *   7. Tutup file dengan fclose().
 */

void generate_jadwal(Dokter dokter[], int jumlah_dokter, int jadwal[SHIFT_PER_HARI][MAX_HARI]);
/**
 * generate_jadwal
 * ---------------
 * Menyusun jadwal otomatis menggunakan algoritma greedy 3-step dengan prioritas preferensi.
 * 
 * Input:
 *   - dokter[]: array dokter
 *   - jumlah_dokter: banyaknya dokter
 *   - jadwal[SHIFT_PER_HARI][MAX_HARI]: array 2d/matriks penjadwalan
 * 
 * Output:
 *   - dokter[] diupdate (bagian total_shift, pelanggaran, dan jumlah_shift_per_jenis)
 *   - jadwal[][] array 3d/matriks terisi indeks dokter yang dijadwalkan
 * 
 * Langkah kerja (3-step greedy algorithm):
 *   STEP 0 - Inisialisasi:
 *   - Reset semua jadwal ke 0, inisialisasi data dokter
 *   - Hitung jumlah_preferensi untuk setiap dokter
 *   
 *   STEP 1 - Prioritas Preferensi: (Tanpa pelanggaran)
 *   - Loop untuk setiap shift dan hari (30 hari)
 *   - Reset shift_mingguan_current setiap awal minggu (hari % 7 == 0)
 *   - Prioritaskan dokter berdasarkan jumlah preferensi (1->2->3)
 *   - Pilih dokter dengan preferensi sesuai dan shift mingguan terkecil
 *   - Update jadwal dan data dokter
 *   
 *   STEP 2 - Isi Shift Kosong: (Pelanggaran jika perlu)
 *   - Cari shift yang masih kosong (jadwal == 0)
 *   - Recalculate shift_mingguan_current setiap awal minggu
 *   - Pilih dokter tersedia dengan shift mingguan terkecil (tanpa memperhatikan preferensi)
 *   - Tambahkan pelanggaran jika dokter tidak punya preferensi untuk shift tersebut
 *   
 *   STEP 3 - Tambah Dokter ke Shift Existing: (Tanpa pelanggaran)
 *   - Untuk setiap shift yang sudah ada dokternya
 *   - Kumpulkan dokter yang memenuhi syarat (ada preferensi, belum max shift mingguan)
 *   - Sort berdasarkan shift_mingguan_current (ascending)
 *   - Assign multiple dokter ke shift yang sama jika memungkinkan
 */

void simpan_kalendar_csv(const char* nama_file, int jadwal[SHIFT_PER_HARI][MAX_HARI], Dokter dokter[], int jumlah_dokter);
/**
 * simpan_kalendar_csv
 * -------------------
 * Menulis hasil jadwal ke file CSV dengan format multiple dokter per shift dengan format:
 *   hari,shift,dokter_id;dokter_id;...
 * 
 * Input:
 *   - nama_file: nama file output
 *   - jadwal: matriks penjadwalan
 *   - dokter[]: array dokter
 *   - jumlah_dokter: jumlah dokter
 *
 * Output:
 *   - File CSV berisi jadwal shift dokter.
 *
 * Langkah kerja:
 *   1. Buka file untuk ditulis dengan fopen()
 *   2. Tulis header: "hari,shift,dokter"
 *   3. Loop untuk setiap hari (1-30) dan shift (1-3):
 *      - Tulis hari dan shift
 *      - Cari semua dokter yang dijadwalkan di shift tersebut (cek dokter[i].jadwal[day_shift_index])
 *      - Tulis ID dokter dengan separator ";" untuk multiple dokter
 *   4. Tutup file dengan fclose()
 */

void simpan_kalendar_dokter_csv(const char* nama_file, Dokter dokter[], int jumlah_dokter);
/**
 * simpan_kalendar_dokter_csv
 * --------------------------
 * Menulis statistik shift masing-masing dokter ke file kalendar_dokter.csv dengan format:
 *   id,jumlah_pagi,jumlah_siang,jumlah_malam,preferensi_pagi,preferensi_siang,preferensi_malam,shift_mingguan,pelanggaran
 *
 * Input:
 *   - nama_file: nama file output
 *   - dokter[]: array dokter
 *   - jumlah_dokter: jumlah dokter
 *
 * Output:
 *   - File CSV berisi statistik shift dokter.
 *
 * Langkah kerja:
 *   1. Buka file untuk ditulis dengan fopen()
 *   2. Tulis header: "id,jumlah_pagi,jumlah_siang,jumlah_malam,preferensi_pagi,preferensi_siang,preferensi_malam,shift_mingguan,pelanggaran"
 *   3. Loop untuk setiap dokter:
 *      - Tulis semua data: ID, jumlah shift per jenis, preferensi, max shift mingguan, dan pelanggaran
 *   4. Tutup file dengan fclose()
 */