airSche1.exe:
	g++ -o airSche1.exe airSche1.cpp fordfulkerson.cpp edge.cpp -std=c++11
clean:
	rm *.exe
	rm *.o
