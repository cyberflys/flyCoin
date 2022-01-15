/*
 *	flyCoin Daemon Listener
 *		by suibex	
 *			http://cyfly.rs
 *
 *			11 Jan 21.
 */
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include <netdb.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <vector>
#include <regex>
#include <thread>
#include <unistd.h>
#include "access.hpp"
#define MAXCONN 65535

#define PORT "9701"

using namespace std;
vector<string> verify(string usr,string passwd){
	vector<string> omg = flyCoinProtocol(usr,passwd,"a","em");
	return omg;
}

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
      	return conv;  
}

void UpdateMoney(string sender,string amount,string username,string password){

	vector<string>data = verify(username,password);
	string curr = data[0];
	string usr = data[2];

	string passwd = data[3];
	FILE *pipe;
	char buffer[2048];
	amount = amount.substr(amount.find(":")+1);
	cout<<"Received:"<<amount<<" flyCoin/s"<<endl;
	memset(&buffer,0,sizeof(buffer));
	stringstream ss;
	float curf ;
	ss<<curr;
	ss>>curf;
	stringstream sz;
	sz<<amount;
	float recvam;
	sz>>recvam;
	if(recvam < 0){
		cout<<"You cannot receive a payment in - digits."<<endl;
		
		abort();
	}
	float calculated = curf+recvam;
	stringstream zo;
	zo<<calculated;
	string finalamm;
	zo>>finalamm;
	flyCoinProtocol(username,password,"w",finalamm);
	

}

string myIP(){
	FILE *pipe;
	pipe = popen("python3 ip.py","r");
	char buffer[2048];
	string lom;
	memset(&buffer,0,sizeof(buffer));
	while(fgets(buffer,sizeof(buffer),pipe) != NULL){
		lom+=buffer;
	}
	return lom;
}
void Listener(string username,string password){
	
	vector<string>datas;
	
	struct addrinfo *ptr,*result,hints;
	int sock,cli;
	int rv;
	struct sockaddr_storage theiraddr;
	socklen_t size;
	char ip [INET6_ADDRSTRLEN];
	
	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype=SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	//socket , bind , freeaddr , listen , while accept new sock , fork , boom;
	getaddrinfo(NULL,PORT,&hints,&result);
	int yes =1;
	for(ptr= result;ptr!=NULL;ptr=ptr->ai_next){
		
		if((sock = socket(ptr->ai_family,ptr->ai_socktype,ptr->ai_protocol)) <0){
			cout<<"Could not create socket."<<endl;	
			exit(1);
		}
		setsockopt(sock,SOL_SOCKET,SO_REUSEADDR ,&yes,sizeof(int));
		if((bind(sock,ptr->ai_addr,ptr->ai_addrlen)) <0){
			cout<<"Could not bind address ."<<endl;
			exit(1);
		}

		break;
	}
	freeaddrinfo(result);

	if(listen(sock,55) < 0){
		cout<<"Could not listen."<<endl;
		exit(1);
	}
	size = sizeof(theiraddr);
	char buffer[2048];
	while((cli = accept(sock,(struct sockaddr*)&theiraddr,&size)) >0){
		memset(&buffer,0,sizeof(buffer));
		int recvsize = recv(cli,buffer,sizeof(buffer),0);
		
		if(fork() == 0){
			close(sock);
			string fixed = buffer;
			stringstream ss;
			ss<<fixed;
			string input;
			while(ss>>input){
				datas.push_back(input);
			}
			string hash = datas[0];
			string sender = datas[1];
			string amount = datas[2];
			
			string unhashed = unHash(hash);
			string password = unhashed.substr(unhashed.find(":")+1);
			cout<<password<<endl;
			cout<<sender<<endl;
			vector<string>verifyuser = verify(sender,password);
			
			if(verifyuser[0].compare("404") == 0){
				cout<<"Unauthorized transaction."<<endl;
				break;
			}
			if(sender.find(username)!= string::npos){
				cout<<"Cannot send to yourself.";
				break;
			}
			cout<<"Sender:"<<sender<<endl;
	
			UpdateMoney(sender,amount,username,password);

				
			close(cli);
			exit(1);
		
		}
		close(cli);
	}


}

int main(int argc,char *argv[]){
	
	if(argc < 2){
		cout<<"./executable {username} {password} "<<endl;
		return 1;
	}
	string username = argv[1];
	//username.erase(remove(username.begin(),username.end(),'\n'),username.end());
	
	string passwd= argv[2];
	//passwd.erase(remove(passwd.begin(),passwd.end(),'\n'),passwd.end());
	vector<string>verification = verify(username,passwd);
	if(verification.size() > 0){
		cout<<"Listening..."<<endl;
		Listener(username,passwd);


	}
	else{
		exit(1);
	}
}

