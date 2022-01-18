#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sstream>
#include <netdb.h>
#include <thread>
#include <unistd.h>
#include <string.h>
#include <vector>
//DEFAULT VARIABLES
#define PORTCLI 1907 // birth date of my mother , because i love her the most
#define IP "192.168.0.18"
using namespace std;
/*
 * 		flyCoin Transfer protocol , client observer
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
vector<string> flyCoinProtocol(string username,string password,string mode,string amount){

	struct sockaddr_in addr;
	int s;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(IP);
	addr.sin_port =htons(PORTCLI);
	vector<string>received;
	s = socket(AF_INET,SOCK_STREAM,0);
	connect(s,(struct sockaddr*)&addr,sizeof(addr));
	if(amount == "em" && mode.compare("a") == 0){
		
		string comb = username+":"+password+":"+mode;
		send(s,comb.c_str(),comb.length(),0);
		char resp[2048];
		memset(&resp,0,sizeof(resp));
		recv(s,resp,sizeof(resp),0);
		stringstream ss;
		ss<<resp;
		string input;
		while(ss>>input){
			received.push_back(input);

		}	



	}
	if(amount != "em"  && mode.compare("w") ==0){
		string comb = username+":"+password+":"+mode+":"+amount;
		send(s,comb.c_str(),comb.length(),0);
		char resp[2048];
		memset(&resp,0,sizeof(resp));
		recv(s,resp,sizeof(resp),0);

		stringstream ss;
		ss<<resp;
		string input;
		while(ss>>input){
			received.push_back(input);

		}	


	}
	if(amount != "em" && mode.compare("m")==0){
	
		string comb = username+":"+password+":"+mode+":"+amount;
	
		send(s,comb.c_str(),comb.length(),0);
		
		char resp[2048];
		memset(&resp,0,sizeof(resp));
		recv(s,resp,sizeof(resp),0);
		string response = resp;
		stringstream ss;
		ss<<resp;
		string input;
		while(ss>>input){
			
			received.push_back(input);

		}
		if(response.find("Correct") != string::npos){
		    cout<<"Congratulations!\nYou won.\n-suibex , have a great day :)"<<endl;
		    cout<<response<<endl;
			exit(1);
		}
		
	}
	close(s);
	return received;	
	

}

