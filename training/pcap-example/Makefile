pcap: pcap.o
	gcc -o pcap pcap.o -lpcap
sniff: sniff.o
	gcc -o sniff sniff.o -lpcap
all: pcap sniff
clean:
	rm pcap
	rm sniff
	rm *.o
