CC = gcc
LDFLAGS = -L Library/jumlah_pelanggaran -L Library/DataDokter -L Library/shift_perdokter
LIBS = -lDataDokter -;

main: test.c Library/DataDokter/libDataDokter.a
	gcc test.c -o test -L Library/DataDokter -lDataDokter
Library/DataDokter/libDataDokter.a: Library/DataDokter/DataDokter.o
	ar rcs $@ $^
Library/DataDokter/DataDokter.o: Library/DataDokter/DataDokter.c
	gcc -c Library/DataDokter/DataDokter.c -o $@
Library/jumlah_pelang