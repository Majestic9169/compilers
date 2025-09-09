all:
	flex ass1.l
	g++ lex.yy.c
	@echo "[test]: running sample test case 1"
	./a.out 1.in
	@echo "[test]: running sample test case 2"
	./a.out 2.in
	@echo "[test]: running sample test case 3"
	./a.out ./A1-Guidelines/input.txt

clean: 
	rm -rf *.out lex.yy.c
