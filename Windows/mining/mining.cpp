

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <direct.h>
#include <windows.h>
#include <string.h>
#include <fstream>
#include <vector>


// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")

using namespace std;
//add constant writing and checking , if correct , break and celebrate :) 

void Miner() {
	FILE* a;
	a = fopen("mine.dat", "ab");
	while (true) {

		long int min = 69;
		long  double max = 999999999999999999;

		long int range = max - min + 1;

		long int num = rand() % range + min;
		cout << "Number:" << num << endl;
		string write;
		stringstream ss;
		ss << num;
		ss >> write;
		write += "\n";
		fwrite(write.c_str(), write.length(), 1, a);


	}
	fclose(a);
}
vector<string> Response(string username, string password, string amount) {

	vector<string>data = flyCoinProtocol(username, password, "m", amount);

	return data;

}
void Check(string username, string password) {

	vector<string>check = flyCoinProtocol(username, password, "a", "em");

	if (check.size() <= 0) {
		cout << "Fatal error." << endl;
		exit(1);
	}
	vector<string>comb;

	fstream re("mine.dat");

	string input;

	while (re >> input) {
		comb.push_back(input);
	}
	cout << "size of data:" << comb.size() << endl;
	cout << "Connecting to flyCoin servers..." << endl;
	cout << "***********************************" << endl;

	for (int i = 0; i < comb.size(); i++) {
		vector<string>resp = Response(username, password, comb[i]);
		cout << "TRYING:" << comb[i] << endl;
		for (int i = 0; i < resp.size(); i++) {
			cout << resp[i] << endl;

		}

	}



}
int main(int argc, char* argv[]) {

	if (argc < 2) {
		cout << "./executable {arg} {username} {password}\nArguments: -mine - used for mining the data \n-check - used for checking the data and maybe winning the prize!" << endl;
		return 1;
	}

	string username = argv[2];
	string password = argv[3];
	//	mkdir("flycoin-mine",0777);
	vector<string> dat = flyCoinProtocol(username, password, "a", "em");


	if (_chdir("flycoin-mine") < 0) {
		cout << "Could not change directory to flycoin-mine." << endl;
		exit(1);
	}
	string arg = argv[1];

	if (arg.compare("-mine") == 0) {
		Miner();
	}
	if (arg.compare("-check") == 0) {
		cout << "Checking process can take a long time.\nYou need to leave your computer turned on.\nAre you sure? [Y] [N]" << endl;
		string answer;
		cin >> answer;
		if (answer.compare("y") == 0 || answer.compare("y") == 0) {
			Check(username, password);

		}
		else {
			cout << "Cancelled." << endl;
			exit(1);
		}
	}
	else {
		cout << "Provide correct argument to start the operation." << endl;
	}




}