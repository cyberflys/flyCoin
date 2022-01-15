package main

import (
    "fmt"
    "database/sql"
    _ "mysql"
    "os"

//   vector "container/vector"  

)
var database string = "flycoin"
var comb string = "username:password@tcp(127.0.0.1:3306)/"+database

type Tag struct{
	amount float64  `"json:amount"`
	hash string `"json:hash"`
	username string `"json:username"`
	password string `"json:password"`
}

func accountInfo(username string ,password string) [4]string{
	db,err:= sql.Open("mysql",comb)
	if(err != nil){
		panic(err.Error())
	}
	query:= "SELECT * from Wallet where username='"+username+"' and password='"+password+"';"
	results,err:= db.Query(query)
	
	var data[4] string
	var money float64;
	for results.Next(){
		err = results.Scan(&money,&data[1],&data[2],&data[3])
		if(err != nil){
			panic(err.Error())
		}

		var converted =fmt.Sprintf("%f",money);
		data[0]+=converted;
		
	}	
	defer db.Close()

	return data

}
func accountChange(username string,password string ,amount string){
	
	
	checker:= accountInfo(username,password)

	var current int = checker[0];

	if(checker[2] != username && checker[3] != password){
		fmt.Println("Invalid credidentials.");
		os.Exit(0);
	}
	db,err := sql.Open("mysql",comb)
	if(err !=nil){
		panic(err.Error());
	}
	query:="UPDATE Wallet SET amount="+amount+ " WHERE username='"+username+"' and password='"+password+"';";
	db.Query(query);

	updated:=accountInfo(username,password);
	fmt.Println("********** UPDATED ACCOUNT DETAILS **********")
	for i:=0;i<len(updated);i++{
		fmt.Println(updated[i])
	}

	defer db.Close()
}
func main(){

	if( len(os.Args) < 2){

		fmt.Println("./access {arg} {username} {password} {amount} ")
		os.Exit(0)

	}


	//var hostname string = "localhost"
	var arg string = os.Args[1]
	var username string = os.Args[2]
	var password string = os.Args[3];


	if(arg == "-a"){
		if(len(os.Args) != 4){
			fmt.Println("./exectuable {arg} {username} {password} {amount}")
			os.Exit(0);
		}
		updated:=accountInfo(username,password);
		for i:=0;i<len(updated);i++{
			fmt.Println(updated[i])
		}
	}
	if(arg == "-w"){
		var money string = os.Args[4]
		if(len(os.Args) != 5){
			fmt.Println("./exectuable {arg} {username} {password} {amount}")
			os.Exit(0);

		}
		accountChange(username,password,money);

	}

}
