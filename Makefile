build:
	gcc tema1.c functions.c -g -o tema

clean:
	rm tema

checker:
	python3 checker.py
