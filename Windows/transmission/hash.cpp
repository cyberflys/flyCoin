
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>

#include <stdlib.h>
#include <string.h>
#include <random>
#include <algorithm>
#include <string.h>
#include <cstring>
#include <iostream>

#include <vector>

#include <sstream>
#include <cstring>
#define IP "34.243.39.244"


#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>


#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")


using namespace std;

class SHES {

public:

	string getMyIP() {
		WSADATA data1;
		WSAStartup(MAKEWORD(2, 2), &data1);
		struct sockaddr_in addr;
		SOCKET sock;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(IP);
		addr.sin_port = htons(80);

		sock = socket(AF_INET, SOCK_STREAM, 0);
		connect(sock, (struct sockaddr*)&addr, sizeof(addr));

		string request = "GET /%s HTTP/1.1\r\nHost: checkip.amazonaws.com\r\n\r\n";
		send(sock, request.c_str(), request.length(), 0);
		string ip;
		char buffer[2048];
		memset(&buffer, 0, sizeof(buffer));
		int rc = recv(sock, buffer, sizeof(buffer), 0);

		string om = buffer;
		stringstream ss;
		ss << om;
		string input;
		vector<string>data;
		while (ss >> input) {
			data.push_back(input);
		}
		ip += data[data.size() - 1];
		closesocket(sock);
		return ip;
	}

	string encrypt(string key) {
		//PLAN for encryption:
		//mixing all the random keys from vector on the certain position
		//reversing a string
		//adding random 3 values at the end
		vector<char>hexes;
		string dat = getMyIP();
		cout << "DAT:" << dat << endl;
		dat += ":";
		dat += key;
		stringstream ss;
		for (int i = 0; i < dat.length(); i++) {
			ss << hex << (int)dat[i];
		}
		string omg = ss.str();


		vector<char>chunks;
		for (int i = 0; i < omg.length(); i++) {
			chunks.push_back(omg[i]);
		}
		//Encryption starts here
		//reversing letters

		//LETTERS
		int nextel = chunks.size() - 1;
		for (int i = 1; i < chunks.size(); i++) {

			char temp = chunks[i];
			chunks[i] = chunks[i - 1];
			chunks[i - 1] = temp;



		}
		//finished reversing letters

		stringstream ssz;
		for (int i = 0; i < chunks.size(); i++) {
			ssz << chunks[i];
		}
		string chunked = ssz.str();
		//saved letters to string 

		//chunk system
		vector<string>blocks;
		int spliz = 4;

		int j = chunked.length() / spliz;

		for (int i = 0; i < j; i++) {
			blocks.push_back(chunked.substr(i * spliz, spliz));
		}
		if (omg.length() % spliz != 0) {
			blocks.push_back(chunked.substr(spliz * j));
		}

		//splitted all letters into chunks and then into string vector
		int nextel1 = blocks.size() - 1;
		for (int i = 0; i < blocks.size(); i++) {

			string temp = blocks[i];
			blocks[i] = blocks[nextel1];
			blocks[nextel1] = temp;

		}
		//jibberishing chunks


		stringstream ret;

		for (int i = 0; i < chunks.size(); i++) {

			ret << chunks[i];
		}
		string sen1 = ret.str();
		//decrypt here

		return sen1;


	}

};


int main(int argc, char* argv[]) {

	SHES shes = SHES();
	if (argc < 2) {

		cout << "Specify a key." << endl;
		return 1;
	}
	string key = argv[1];
	
	cout << "Key:" << key << endl;
	string encrypted = shes.encrypt(key);
	cout << "HASH:" << encrypted << "\nSaved to hash.dat." << endl;
	ofstream S("hash.dat");
	if (S.is_open()) {
		S << encrypted;

	}
	S.close();



}
