CC = iverilog

main:
	$(CC) -o CPU.out *.v

vvp:
	vvp CPU.out

diff1:
	diff -w ../testdata/output.txt ../testdata/output_1.txt

diff2:
	diff -w ../testdata/output.txt ../testdata/output_2.txt

gtk:
	gtkwave CPU.vcd

clean:
	rm -f CPU.out CPU.vcd ../testdata/output.txt
