#!/usr/bin/python3
import hashlib
import sys

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
    H1 = hashlib.sha256(f1).hexdigest()
    return  H1

def pof(z):
    msgf="fail"
    msgp="pass"
    if z== True:
        return msgp
    else:
        return msgf

def err_print(f1, of):
    p_o_f = True
    of1=of.read()
    o_hash=hash(of1)
    h_ihash=None
    h_proof=""
    h_bit=None
    h_fhash=None
    msg1="Initial-hash: "
    msg2="Proof-of-work: "
    msg3="Leading-zero-bits: "
    msg4="Hash: "

    lines= f1.readlines()

    for i in lines:
        i=i.strip()
        if i.startswith(msg1) : 
            h_ihash=i.replace(msg1, "")
        elif i.startswith(msg2) :
            h_proof=i.replace(msg2, "")
        elif i.startswith(msg3) :
            h_bit=i.replace(msg3, "")
        elif i.startswith(msg4) : 
            h_fhash=i.replace(msg4, "")
    f1.close()
    to_be_hash=o_hash + h_proof
    f_hashed=hash(str.encode(to_be_hash))
    f_bit=count(f_hashed)

    if h_ihash == None:
        p_o_f = False 
        print("ERROR: missing Initial-hash in header")
    else:
        if o_hash== h_ihash:
            print("PASSED: initial file hashes match")
        else:
            p_o_f = False
            print("ERROR: initial hashes don't match\n", "      expected: %s \n" %(o_hash), "      header has: %s" % (h_ihash))
        
    if h_bit== None:
        p_o_f = False
        print("ERROR: missing Leading-zero-bits in header")
    else:
        if int(h_bit)==int(f_bit):
            print("PASSED: leading zero bits is correct")
        else:
            p_o_f = False
            print("ERROR: Leading-zero-bits value: %s, but hash has %s leading zero bits" %(h_bit, f_bit))
    
    if h_fhash==None:
        p_o_f=False
        print("ERROR: missing pow hash in header")
    else: 
        if h_fhash==f_hashed:
            print("PASSED: pow hash matches Hash header")
        else: 
            p_o_f=False
            print("ERROR: pow hash does not match Hash header\n", "      expected: %s \n" %(f_hashed), "      header has: %s" % (h_fhash))
    z=pof(p_o_f)
    print(z)


if __name__ == "__main__":
    er=0
    try:
        file1= open(sys.argv[1], "r")
        o_file= open(sys.argv[2], "rb")
    except: 
        er=1
        print("Error: Can't open input file")
    if er==1:
        quit()
    if len(sys.argv) != 3: 
        print("Error: wrong number of arguments")
        quit()
    else:
        err_print(file1, o_file)