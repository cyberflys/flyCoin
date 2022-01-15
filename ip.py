import requests 
import os,sys,io 

ip = requests.get("https://www.myexternalip.com/raw").text.strip()
print(ip)
