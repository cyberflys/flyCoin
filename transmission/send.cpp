/*
 *
 *		flyCoin Wallet Module 1
 			Send money module
			
 *			by suibex	
 *
 *			18 Jan 21.
 *				http://cyfly.rs
 *
 */
#include <sstream>
#include <netdb.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <regex>
#include "access.hpp"
#define PORT "9701"
#define IP "178.148.206.252" // IP address of the main server branch
using namespace std;
//implement access.hpp function !!
string convertBack(string s){
	string converted;

	for(int i =0;i<s.length();i+=2){
		string part = s.substr(i,2);
		char ch = stoul(part,NULL,16);
		converted+=ch;
	}

	return converted;
}


string decrypt(string data){
	//Plan for decryption
	//
	//REVERSE ALL ENCRYPTION STEPS!!!!


	//breaking string into chunks 4 * 4
	vector<string>blocks;
	int spliz = 4;

	int j = data.length()/spliz;
	
	for(int i =0;i<j;i++){
		blocks.push_back(data.substr(i*spliz,spliz));
	}
	if(data.length()% spliz !=0){
		blocks.push_back(data.substr(spliz*j));
	}

	int nextel1 = blocks.size()-1;
	for(int i =0;i<blocks.size();i++){

		string temp = blocks[nextel1];
		blocks[nextel1] = blocks[i];
		blocks[i]= temp;

	}

	
	stringstream szy;
	for(int i=0;i<blocks.size();i++){
		szy<<blocks[i];
	}
	string ended= szy.str();
	vector<char>chunks;
	for(int i =0;i<ended.length();i++){
		chunks.push_back(ended[i]);
	}

	//LETTERS
	int nextel2 = chunks.size()-1;
	for(int i =1;i<chunks.size();i++){

		char temp = chunks[i];
		chunks[i] = chunks[i-1];
		chunks[i-1]= temp;		

	}
	//printing it 
	stringstream lolz;
	for(int i =0;i<chunks.size();i++){
		lolz<<chunks[i];
	}
	string om = lolz.str();
	return om;

}	

string unHash(string hash){
	
	string decr= decrypt(hash);
	string conv = convertBack(decr);
	string ip = conv.substr(conv.find(":")+1);
	return ip;

}
vector<string>myInfo(string username,string password){
	vector<string> omg = flyCoinProtocol(username,password,"a","em");
	
	return omg;

}
int main(int argc , char *argv[]){
	
	if(argc < 2){

		cout<<"Specify all info needed."<<endl;
		
		cout<<"./executable {sender} {receiver} {hishash} {amount}"<<endl;

		return 1;
	}
	string sender = argv[1];
	string receiver = argv[2];
	string hishash = argv[3];
	string amount = argv[4];
	string myhash;
	string input;
	fstream rhash("hash.dat");
	while(rhash>>input){
		myhash+=input;
	}	
	if(myhash.length() <=0){
		cout<<"Please create your hash before sending the money."<<endl;
		exit(1);
	}
	
	string mypassword = unHash(myhash);
	vector<string>info = myInfo(sender,mypassword);
	
	cout<<"\r\nflyCoin Transmission protocol started.\r\n\r\n"<<endl;
	
	cout<<"My Username:"<<info[2]<<endl;
	cout<<"My password:"<<mypassword<<endl;
	struct addrinfo *ptr=NULL,*result = NULL,hints;

	int sock;
	memset(&hints,0,sizeof(hints));
	
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol=IPPROTO_TCP;
	
	getaddrinfo(IP,PORT,&hints,&result);
	for(ptr = result;ptr!=NULL;ptr=ptr->ai_next){
		sock = socket(ptr->ai_family,ptr->ai_socktype,ptr->ai_protocol);
		connect(sock,ptr->ai_addr,(int)ptr->ai_addrlen);
	}
	freeaddrinfo(result);
	//crafting the send packet.
	//yea , im actually right now starting to comment stuff.
	// II understood everything i was writing , but tought this , crafting the packet would sound cool. :)
	// an	string choice;
	
/*
 *              PACKET SCHEME:
 *
 *                      sender\nhash\nreceiver\nhishash\namount
 */



	string packet = sender+"\n"+myhash+"\n"+receiver+"\n"+hishash+"\n"+amount;

	string choice;
	cout<<"Are you sure you want to process this payment?\r\nThis process cannot be undone."<<endl;
	cin>>choice;
	if(choice.compare("yes")==0 || choice.compare("Yes")==0 || choice.compare("Y") ==0 || choice.compare("y") == 0){
		
		stringstream ss;
		ss<<info[0];
		float curr;
		ss>>curr;
		stringstream z;
		z<<amount;
		float amoun;
		z>>amoun;


		float calculated = curr -amoun;
		if(calculated < 0){
			cout<<"You don't have enough funds to process the this payment.";
			close(sock);
			exit(1);

			return 0;


		}
		if(curr <= 0){
			cout<<"You don't have enough funds to process the this payment.";
			close(sock);
			exit(1);

			return 0;

		}
		else{
		
			send(sock,packet.c_str(),packet.length(),0);

			cout<<"Transmission completed."<<endl;
		}
		return 0;
	
	}
	else{
	
		cout<<"Cancelled."<<endl;
		exit(1);
	}
	
}	
