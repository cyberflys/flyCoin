/*
 *	flyCoin Daemon Listener
 *		by suibex	
 *			http://cyfly.rs
 *
 *			18 Jan 21.
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


/*
 *		PACKET SCHEME:
 *
 *			sender\nhash\nreceiver\namount
 */
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
      	string passwd = conv.substr(conv.find(":")+1);

	return passwd; 
}

void UpdateMoney(string sender,string password,string receiver,string recvhash,string amount){

	//add money to the receiver 
	//remove from the sender
	vector<string>data = verify(sender,password);
	string senderwallet = data[0];
	//cout<<"WALLET:"<<senderwallet;
	//cout<<"PASSWORD:"<<password;

	cout<<receiver<<" received "<<amount<<" flyCoins."<<endl;
	//cout<<receiver<<endl;
//	cout<<recvhash<<endl;

	float recvamount;
	float senderamount;
	float calc;

	stringstream ss;
	ss<<amount;
	ss>>recvamount;
	ss.clear();
	ss<<senderwallet;
	ss>>senderamount;
	ss.clear();
	calc = senderamount - recvamount;
	string dignity;
	ss<<calc;
	ss>>dignity;
	ss.clear();

	//removing money from senders account 
	flyCoinProtocol(sender,password,"w",dignity);

	//getting money from receiver and adding money 
	
	string passrec=unHash(recvhash);
	vector<string>receiverdata = verify(receiver,passrec);
	string recvwallet = receiverdata[0];
	float wallet;
	stringstream recs;

	recs<<recvwallet;
	recs>>wallet;
	recs.clear();
	float calculated = wallet + recvamount;

	string finalwrite;
	recs<<calculated;
	recs>>finalwrite;
	recs.clear();
	flyCoinProtocol(receiver,passrec,"w",finalwrite);



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



/*
 *		PACKET SCHEME:
 *
 *			sender\nhash\nreceiver\namount
 */
void Listener(){
	
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
			
/*
 *			PACKET SCHEME:
 *			sender\nhash\nreceiver\nhash2\namount
 */
			string sender = datas[0];
			string hash = datas[1];
			string receiver = datas[2];
			string hishash = datas[3];
			string amount = datas[4];


			string unhashed = unHash(hash);
			string password = unhashed.substr(unhashed.find(":")+1);
			vector<string>verifyuser = verify(sender,password);
			string username = verifyuser[3];

			if(verifyuser[0].compare("404") == 0){
				cout<<"Unauthorized transaction."<<endl;
				break;
			}
			if(sender.find(username)!= string::npos){
				cout<<"Cannot send to yourself.";
				break;
			}
			cout<<"Sender:"<<sender<<endl;
	
			UpdateMoney(sender,password,receiver,hishash,amount);
				
			close(cli);
			exit(1);
		
		}
		close(cli);
	}


}

int main(int argc,char *argv[]){
	
	
	//passwd.erase(remove(passwd.begin(),passwd.end(),'\n'),passwd.end());
	cout<<"Listening..."<<endl;
	Listener();

}

