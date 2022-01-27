#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <windows.h>
#include <string.h>
#include <vector>
//DEFAULT VARIABLES
#define PORTCLI 1907 // birth date of my mother , because i love her the most
#define IP "178.148.206.252"
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")

using namespace std;
/*
 * 		flyCoin Transfer protocol , client observer
 * 
 * 			12 Jan 21.
 *
 *		SCHEME :
 *
 *		USER: SEND: DATA-> HOST RECEIVES -> GETS SQL DATA -> SENDS SQL DATA BACK TO USER -
 *		^									   |
 *		|									   |
 *		|									   |
 *		|									   |
 *		|									   |
 *		|--------------------------------------------------------------------------
 */
vector<string> flyCoinProtocol(string username, string password, string mode, string amount) {

	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);

	struct sockaddr_in addr;
	SOCKET s;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(IP);
	addr.sin_port = htons(PORTCLI);
	vector<string>received;
	s = socket(AF_INET, SOCK_STREAM, 0);
	connect(s, (struct sockaddr*)&addr, sizeof(addr));
	if (amount == "em" && mode.compare("a") == 0) {

		string comb = username + ":" + password + ":" + mode;
		send(s, comb.c_str(), comb.length(), 0);
		char resp[2048];
		memset(&resp, 0, sizeof(resp));
		recv(s, resp, sizeof(resp), 0);
		stringstream ss;
		ss << resp;
		string input;
		while (ss >> input) {
			received.push_back(input);

		}



	}
	if (amount != "em" && mode.compare("w") == 0) {
		string comb = username + ":" + password + ":" + mode + ":" + amount;
		send(s, comb.c_str(), comb.length(), 0);
		char resp[2048];
		memset(&resp, 0, sizeof(resp));
		recv(s, resp, sizeof(resp), 0);

		stringstream ss;
		ss << resp;
		string input;
		while (ss >> input) {
			received.push_back(input);

		}


	}
	if (amount != "em" && mode.compare("m") == 0) {

		string comb = username + ":" + password + ":" + mode + ":" + amount;

		send(s, comb.c_str(), comb.length(), 0);

		char resp[2048];
		memset(&resp, 0, sizeof(resp));
		recv(s, resp, sizeof(resp), 0);
		string response = resp;
		stringstream ss;
		ss << resp;
		string input;
		while (ss >> input) {

			received.push_back(input);

		}
		if (response.find("Correct") != string::npos) {
			cout << "Congratulations!\nYou won.\n-suibex , have a great day :)" << endl;
			cout << response << endl;
			exit(1);
		}

	}
	closesocket(s);
	WSACleanup();
	return received;


}

