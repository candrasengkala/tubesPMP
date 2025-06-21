CC = gcc
LDFLAGS = -LLibrary/DataDokter -LLibrary/JadwalGeneration -LLibrary/JadwalPerWaktu -LLibrary/jumlah_pelanggaran -LLibrary/shift_perdokter
LIBS = -lDataDokter -lJadwalGeneration -lJadwalPerWaktu -ljumlah_pelanggaran -lshift_perdokter

main: main.c Library/DataDokter/libDataDokter.a Library/JadwalGeneration/libJadwalGeneration.a Library/JadwalPerWaktu/libJadwalPerWaktu.a Library/jumlah_pelanggaran/libjumlah_pelanggaran.a Library/shift_perdokter/libshift_perdokter.a
	$(CC) main.c -o main $(LDFLAGS) $(LIBS) 

Library/DataDokter/libDataDokter.a: Library/DataDokter/DataDokter.o
	ar rcs $@ $^
Library/DataDokter/DataDokter.o: Library/DataDokter/DataDokter.c
	$(CC) -c Library/DataDokter/DataDokter.c -o $@

Library/JadwalGeneration/libJadwalGeneration.a: Library/JadwalGeneration/JadwalGeneration.o
	ar rcs $@ $^
Library/JadwalGeneration/JadwalGeneration.o: Library/JadwalGeneration/JadwalGeneration.c
	$(CC) -c Library/JadwalGeneration/JadwalGeneration.c -o $@

Library/JadwalPerWaktu/libJadwalPerWaktu.a: Library/JadwalPerWaktu/JadwalPerWaktu.o
	ar rcs $@ $^
Library/JadwalPerWaktu/JadwalPerWaktu.o: Library/JadwalPerWaktu/JadwalPerWaktu.c
	$(CC) -c Library/JadwalPerWaktu/JadwalPerWaktu.c -o $@

Library/jumlah_pelanggaran/libjumlah_pelanggaran.a: Library/jumlah_pelanggaran/jumlah_pelanggaran.o
	ar rcs $@ $^
Library/jumlah_pelanggaran/jumlah_pelanggaran.o: Library/jumlah_pelanggaran/jumlah_pelanggaran.c
	$(CC) -c Library/jumlah_pelanggaran/jumlah_pelanggaran.c -o $@

Library/shift_perdokter/libshift_perdokter.a: Library/shift_perdokter/shift_perdokter.o
	ar rcs $@ $^
Library/shift_perdokter/shift_perdokter.o: Library/shift_perdokter/shift_perdokter.c
	$(CC) -c Library/shift_perdokter/shift_perdokter.c -o $@