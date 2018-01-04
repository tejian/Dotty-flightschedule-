airSche1.exe:
	g++ -o airSche1.exe airSche1.cpp fordfulkerson.cpp edge.cpp -std=c++11

airSche2.exe:
	g++ -o airSche2.exe airSche2.cpp fordfulkerson.cpp edge.cpp -std=c++11

clean:
	rm *.exe
	rm *.o
