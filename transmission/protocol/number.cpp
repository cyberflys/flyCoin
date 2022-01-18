#include <iostream>
#include <fstream>
#include <random>
#include <unistd.h>


using namespace std;
long int randnumb(){
	long int min = 900000000000000000;
	long  double max = 999999999999999999;
	
	long int  range = max- min +1;

	long double num = rand() % range + min;

	return num;

}
int main(){

	ofstream file("number.dat");
	while(true){
		long int num = randnumb();
	
		if(file.is_open()){
			cout<<"Picked number:"<<num<<endl;
			cout<<"Write to file [y] [n] ? "<<endl;
			string answer;
			cin>>answer;
			if(answer.compare("y") == 0 || answer.compare("Y")==0){
				file<<num;
				file.close();
				cout<<"Done."<<endl;
				break;
			}
			else{
			
				cout<<"Redoing..."<<endl;

	
			}
		}
	}	
}
