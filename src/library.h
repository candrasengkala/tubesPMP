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
 *   1. Buka file.
 *   2. Lewati header. (misalnya "id,nama,shift_mingguan,preferensi_pagi,preferensi_siang,preferensi_malam")
 *   3. Baca tiap baris: id,nama,shift_mingguan,preferensi_pagi,preferensi_siang,preferensi_malam
 *   4. Simpan ke struct.
 *   5. Update jumlah_dokter.
 *   6. Tutup file.
 */

void generate_jadwal(Dokter dokter[], int jumlah_dokter, int jadwal[SHIFT_PER_HARI][MAX_HARI]);
/**
 * generate_jadwal
 * ---------------
 * Menyusun jadwal otomatis berdasarkan preferensi dan batas shift mingguan.
 * 
 * Input:
 *   - dokter[]: array dokter
 *   - jumlah_dokter: banyaknya dokter
 *   - jadwal[SHIFT_PER_HARI][MAX_HARI]: array 3d/matriks penjadwalan
 * 
 * Output:
 *   - dokter[] diupdate (bagian total_shift, pelanggaran, dan jumlah_shift_per_jenis)
 *   - jadwal[][] array 3d/matriks terisi indeks dokter yang dijadwalkan
 * 
 * Langkah kerja:
 *   1. Untuk tiap hari dan shift, cari dokter yang cocok: (menggunaakn algo greedy)
 *      - Dokter belum terjadwal di hari itu
 *      - Belum melebihi batas shift mingguan (max_shift_per_minggu)
 *      - Preferensi shift sesuai (sebisanya)
 *   2. Update total shift, pelanggaran (jika terjadi), dan jenis shift.
 *   3. Simpan ID dokter ke jadwal[shift][hari].
 *   4. Jika tidak ada dokter yang cocok, tandai dengan -1. (belum tau mau diapakan, libur aja kali ya)
 */

void simpan_kalendar_csv(const char* nama_file, int jadwal[SHIFT_PER_HARI][MAX_HARI], Dokter dokter[], int jumlah_dokter);
/**
 * simpan_kalendar_csv
 * -------------------
 * Menulis hasil jadwal ke file kalendar.csv dengan format:
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
 *   1. Buka file untuk ditulis.
 *   2. Tulis header: "hari,shift,dokter"
 *   3. Untuk tiap hari dan tiap shift:
 *      - Ambil ID dokter dari jadwal[shift][hari].
 *      - Jika ada dokter, tulis ke file dengan format "hari,shift,dokter_id;dokter_id;..."
 *   4. Tutup file.
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
 *   1. Buka file untuk ditulis.   
 *   2. Tulis header: "id,jumlah_pagi,jumlah_siang,jumlah_malam,preferensi_pagi,preferensi_siang,preferensi_malam,shift_mingguan,pelanggaran"
 *   3. Untuk tiap dokter:
 *      - Tulis ID, jumlah shift pagi, siang, malam, preferensi, shift mingguan, dan pelanggaran.
 *   4. Tutup file.
 */