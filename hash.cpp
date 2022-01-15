
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <random>
#include <algorithm>
#include <string.h>
#include <cstring>

#include <sstream>

using namespace std;

class SHES{

	public:
		
		
		string PIPENZI(){
			
			FILE *ls = popen("python3 ip.py","r");
			char buff[2048];
			memset(&buff,0,sizeof(buff));
			string ip;
			while(fgets(buff,sizeof(buff),ls) !=0){
				ip+=buff;
			}

			cout<<ip;
			return ip;
		}
		string encrypt(string key){
			//PLAN for encryption:
			//mixing all the random keys from vector on the certain position
			//reversing a string
			//adding random 3 values at the end
			vector<char>hexes;
			string dat = "178.148.206.252";
			cout<<"DAT:"<<dat<<endl;	
			stringstream ss;
			dat+=":";
			dat+=key;

		
			for(int i=0;i<dat.length();i++){
				ss<<hex<<(int)dat[i];
			}
			string omg = ss.str();
		

			vector<char>chunks;
			for(int i =0;i<omg.length();i++){
				chunks.push_back(omg[i]);
			}
			//Encryption starts here
			//reversing letters

			//LETTERS
			int nextel = chunks.size()-1;
			for(int i =1;i<chunks.size();i++){

				char temp = chunks[i];
				chunks[i] = chunks[i-1];
				chunks[i-1]= temp;

				

			}
			//finished reversing letters

			stringstream ssz;
			for(int i =0;i<chunks.size();i++){
				ssz<<chunks[i];
			}
			string chunked= ssz.str();
			//saved letters to string 
			
			//chunk system
			vector<string>blocks;
			int spliz = 4;

			int j = chunked.length()/spliz;

			for(int i =0;i<j;i++){
				blocks.push_back(chunked.substr(i*spliz,spliz));
			}
			if(omg.length()% spliz !=0){
				blocks.push_back(chunked.substr(spliz*j));
			}

			//splitted all letters into chunks and then into string vector
			int nextel1 = blocks.size()-1;
			for(int i =0;i<blocks.size();i++){

				string temp = blocks[i];
				blocks[i] = blocks[nextel1];
				blocks[nextel1]= temp;

			}
			//jibberishing chunks

			
			stringstream ret;

			for(int i =0;i<chunks.size();i++){
			
				ret<<chunks[i];
			}
			string sen1 = ret.str();
			//decrypt here
	
			return sen1;


		}

};


int main(int argc, char *argv[]){
	
	SHES shes = SHES();
	if(argc < 2){
			
		cout<<"Specify a key."<<endl;
		return 1;
	}
	string key=argv[1];
	cout<<"Key:"<<key<<endl;
	string encrypted = shes.encrypt(key);
	cout<<"HASH:"<<encrypted<<"\nSaved to hash.dat."<<endl;
	ofstream S("hash.dat");
	if(S.is_open()){
		S<<encrypted;

	}
	S.close();
	

	
}
