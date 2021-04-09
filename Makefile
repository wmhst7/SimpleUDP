all:
	g++ server.cpp -std=c++14 -O2 -o server
	g++ client.cpp -std=c++14 -O2 -o client
clean:
	rm server client