all: wordlist brutexor

wordlist: 
	gcc wordlist.c -o wordlist

brutexor:
	gcc brutexor.c -o brutexor -lcrypto -lssl

clean:
	rm -rf brutexor
	rm -rf wordlist
	rm -rf *.o