#!/usr/bin/python3 
import sys
import time
import hashlib

def count(H1): 
    c=0
    for i in H1 :
        if i == str(0) :
            c=c+4
        elif i == str(1) :
            c=c+3
            break
        elif i == str(2) or i == str(3) :
            c=c+2
            break
        elif i == str(4) or i == str(5) or i == str(6) or i == str(7) :
            c=c+1 
            break
        else :
            c
            break
    return c

def  hash (f1) :
    H1 = hashlib.sha256(f1)
    return  H1.hexdigest()

def pov(i, listh, first_c):
    po=ord(listh[i])
    if listh[i]==chr(126):
        first_c=True
        listh[i]=chr(33)
    else:
        listh[i]=chr(po+1)
        first_c=False
    return listh, first_c


def proof_of_work(diff,file) :
    f1 = open(file,'rb')
    f=(f1.read()) 
    f1.close()
    record=time.time()
    difficult = int(diff)
    ihash=hash(f)
    d1 = count(ihash)
    cur_ord=33
    listh = []
    listh.append(chr(cur_ord))
    First=33
    first_c= True
    dig=0
    iter=1
    while d1 <difficult:
        First+=1
        iter+=1
        if First==127:
            First=33
            for i in range(dig, -1, -1):
                listh, first_c=pov(i, listh, first_c)
                if first_c == False:
                    break    

            if first_c== True:
                dig=dig+1
                listh.append(chr(33))
                
        else:
            listh[dig]=chr(First)

        list1 = "".join(listh)
        hash_added = ihash+list1
        nhash= hash(str.encode(hash_added))
        d1=count(nhash)
    timenow=time.time()
    time_pass=timenow-record
        
    print("File: %s"%file)
    print("Initial-hash: %s"%ihash)
    print("Proof-of-work: %s"% "".join(listh))
    print("Hash: %s"%nhash)
    print("Leading-zero-bits: %s"%d1)
    print("Iterations: %s"%iter)
    print("Compute-time: %s"%time_pass)
    
if __name__== "__main__" :
    if len(sys.argv)!=3:
        print("Error: wrong number of arguments")
    if int(sys.argv[1]) < 0 :
        print("Error: wrong input of bits")
    else: 
        try:
            c=open(sys.argv[2], "rb")
            c.close()
            proof_of_work(sys.argv[1], sys.argv[2])
        except:
            print("Error: can't find input file")

            

   