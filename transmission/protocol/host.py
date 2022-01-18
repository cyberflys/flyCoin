import socket
import os,sys,io
import threading
import mysql.connector


class Server(object):

    def checkLogin(self,username,password):
        conx = mysql.connector.connect(host='localhost',user='gavrilopalalic',password='gasha2015A!',db='flycoin')

        cur = conx.cursor()
        cur.execute(f"SELECT * FROM Wallet WHERE username='{username}' and password='{password}'")

        data=list()
        i = 0 
        for answer in cur.fetchall():
            print(answer)
            return answer
            
    def writeValues(self,username,password,amount,sock):
        om = self.checkLogin(username,password)
        if( not om):
                sock.send(b"404 Bad Request.")
                sys.exit()
        if(om[2] != username and om[3] != password):
            print("Invalid credidentials.")
            sys.exit()
        
        conx = mysql.connector.connect(host='localhost',user='gavrilopalalic',password='gasha2015A!',db='flycoin')
        cur = conx.cursor()
        cur.execute(f"UPDATE Wallet SET amount={amount} WHERE username='{username}' and password='{password}';")
        print("Account updated.")
        conx.commit()
        conx.close()
        data= self.checkLogin(username,password)
        comb =str(data[0])+"\n"+data[1]+"\n"+data[2]+"\n"+data[3]+"\n"
        sock.send(comb.encode())
        
    def __init__(self):

        print("flyCoin Transfer Protocol Started.\n**********************************************\n")
        s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        s.bind(('0.0.0.0',1907))
        s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,0)

        s.listen(65535)
        while True:
            conn,addr =s.accept()

            t = threading.Thread(target=self.dates,args=(conn,addr))
            t.start()
    def Mine(self,username,password,numb, conn):
            
            datfile = open("number.dat","r")

            number = str(datfile.read()) 
            

            hisnum = str(numb)
            data1= self.checkLogin(username,password)
            print("TUPLE")
            print(data1)
            print("HIS:"+str(hisnum))
            if( not data1):
                conn.send(b"404 Bad Request.")
            else:
                if(number == hisnum):
               
                    conn.send(b'Correct number.\nReceiving 1 flyCoin.')
                    cmd = self.checkLogin(username,password)
                    curram = float(cmd[0])+1.0
                    self.writeValues(username,password,str(curram),conn)

                else:
                    conn.send(b'Correct number.\nReceiving 1 flyCoin.')
                    cmd = self.checkLogin(username,password)
                    curram = float(cmd[0])+1.0
                    self.writeValues(username,password,str(curram),conn)
                    
                    print("NUMBER:"+str(number))
                    conn.send(b'Wrong number.')
                    conn.close()
                                   

                    conn.close()
                    
            
    def dates(self,conn,addr):
        data = conn.recv(1024)
        data =data.decode().split(":")
        if(data[2] == "a"):
            data1= self.checkLogin(data[0],data[1])
            print("TUPLE")
            print(data1)
            if( not data1):
                conn.send(b"404 Bad Request.")
            else:
                comb =str(data1[0])+"\n"+data1[1]+"\n"+data1[2]+"\n"+data1[3]+"\n"
                conn.send(comb.encode())
        
        elif data[2] == "w":
          
            for i in range(len(data)):
                print(data[i])
            self.writeValues(data[0],data[1],data[3],conn)

        elif data[2] == "m":
            self.Mine(data[0],data[1],data[3],conn)




app = Server()
app



