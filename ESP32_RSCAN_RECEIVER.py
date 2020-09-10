# -*- coding: utf-8 -*-
"""
Created on Tue Aug 25 15:48:48 2020

@author: SID7CA
"""

import serial
import keyboard
import time
import sys
import cantools 

COM ='COM6'
BaudRate= 9600
Serial_COM = serial.Serial(COM,BaudRate)


Data = ''
read = ''
    

#Reading DBC file 
db = cantools.database.load_file('XGU.dbc')


#Data position in the Message CAN
data_set=[25,29,33,37,41,45,49,53]


while not keyboard.is_pressed('w'):
    
    #Receiving Serial Information
    carac = chr(ord(Serial_COM.read()))
    read += carac
    
    if(carac == '\n'):
        
         #Separating time_stamp, ID , DLC
         time_stamp= hex(int(read[3:11],16))
         ID=hex(int(read[13:21],16))
         DLC = hex(int(read[22:24],16))
         
         #Separating Data CAN variables
         Data_str = read[25:55]
         Data_byte = bytearray(Data_str,'utf-8')
         
         read =''  

         #Receiving Variable 
         DBC_data = db.decode_message(int(ID,16),Data_byte)
         
         #Print DATA translated by DBC
         #print(DBC_data)
         
         #Printing CAN Information
         print(str(time_stamp) +' '+str(ID) + ' ' + str(DLC)+ ' ' + str(Data_str))
        

Serial_COM.close()
print(COM + ' closed')
