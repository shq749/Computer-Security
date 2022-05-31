import sys
import csv


def AddUser (user,password):
    try:
        x=0
        if user=='':
            print('Error: username missing')
        else:
            with open('users.csv','r') as f1:
                reader=csv.reader(f1)
                for line in reader:
                    name=line[0].replace('\t', '')
                    if user==name:
                        x=0
                        break
                    else:
                        x=1

                if x==0:
                    print('Error: user exists')
                else:
                    with open('users.csv','a') as f1:
                        csvappend=csv.writer(f1, delimiter='\t')
                        csvappend.writerow(user+','+password)
                        print('Success')
    except:
        with open('users.csv','a') as f1:
            csvappend=csv.writer(f1, delimiter='\t')
            csvappend.writerow(user+','+password)
            f1.close()
            print('Success')

def Authenticate (user,password):
    try: 
        x=0
        with open('users.csv','r') as f1:
            reader=csv.reader(f1)
            for line in reader:
                name=line[0].replace('\t', '')
                passwor=line[1].replace('\t','')
                if user==name and password==passwor:
                    x=0
                    print('Success')
                    break
                elif user==name and passwor!=password:
                    x=2
                    print('Error: bad password')
                    break
                else:
                    x=1
            if x==1:
                print('Error: no such user')
    except: 
        print('Error: no such user')

def SetDomain(user, domain):
    try:
        a=0
        b=0     
        if domain=='':
            print('Error: missing domain')
        else:
            with open('users.csv', 'r') as f1:
                reader=csv.reader(f1)
                for line in reader:
                    name=line[0].replace('\t','')

                    if user==name:
                        with open('domain.csv','r') as f2:
                            reader2=csv.reader(f2)
                            for line in reader2:
                                group=line[0].replace('\t','')
                                uname=line[1].replace('\t','')
                                if domain==group and uname==user:
                                    a=1
                                    break
                                else:
                                    pass
                            if a==0:
                                with open('domain.csv','a') as f3:
                                    csvappend2=csv.writer(f3, delimiter='\t')
                                    csvappend2.writerow(domain+','+user)
                                    f3.close()
                                    print("Success")
                        b=1
                        break
                    else:
                        pass
                if b==0:
                    print("Error: no such user")
                        


    except:
        with open('domain.csv','a') as f2:
            csvappend2=csv.writer(f2, delimiter='\t')
            csvappend2.writerow(domain+','+user)
            print("Success")
            f2.close()

def DomainInfo(domain):
    try: 
        d=0
        if domain=='':
            print('Error: missing domain')
        else:
            with open('domain.csv','r') as f1:
                reader2=csv.reader(f1)
                for line in reader2:
                    domainn=line[0].replace('\t','')
                    if domain==domainn:
                        d=1
                        if line[1]=='':
                            pass
                        else:
                            print(line[1].replace('\t',''))
                if d==0: 
                    print("Error: domain not exists")
    except: 
        print("Error: domain not exists")

def SetType(objectname,typee):
    
    try:
        a=0
        if objectname=='':
            print('Error: missing object')
        elif typee=='':
            print('Error: missing type')
        else:
            with open('types.csv','r') as f4:
                readerr=csv.reader(f4)
                for line in readerr:
                    type1=line[1].replace('\t', '')
                    objectname1=line[0].replace('\t','')
                    if objectname==objectname1 and typee==type1:
                        print('Success')
                        a=1
                        break
                if a==0:
                    with open('types.csv','a') as f4:
                        csvappend1=csv.writer(f4, delimiter='\t')
                        csvappend1.writerow(objectname+','+typee)
                        print('Success')
    except:
        with open('types.csv','a') as f4:
            csvappend1=csv.writer(f4, delimiter='\t')
            csvappend1.writerow(objectname+','+typee)
            f4.close()
            print('Success') 
   
def TypeInfo(types):
    try:
        t=0
        if types=='':
            print('Error: missing type')
        else:
            with open('types.csv','r') as f4:
                readerr=csv.reader(f4)
                for line in readerr:
                    type1=line[1].replace('\t','')
                    if type1==types:
                        t=1
                        if line[0]=='':
                            pass
                        else:
                            print(line[0].replace('\t','')+'_'+line[1].replace('\t',''))
                if t==0:
                    print("Error: type not exists")
    except:
        print("Error: type not exists")

def AddAccess(operation,userrr,objects):
    try:
        x=0
        a=0
        b=0
        if operation=='':
            print('Error: missing operation')
        elif userrr=='':
            print('Error: missing domain')
        elif objects=='':
            print('Error: missing type')
        else:
            try:
                with open('domain.csv','r') as f1:
                    readerr=csv.reader(f1)
                    for line in readerr:
                        domain1=line[0].replace('\t','')
                        if userrr==domain1:
                            x=0
                            break
                        else:
                            x=1
            except:
                with open('domain.csv','a') as f1:
                    csvappend2=csv.writer(f1, delimiter='\t')
                    csvappend2.writerow(domain+','+'')
                f1.close()
                
            try:
                with open('types.csv','r') as f2:
                    reader2=csv.reader(f2)
                    for line in reader2:
                        types1=line[1].replace('\t','')
                        if objects==types1:
                            a=0
                            break
                        else:
                             a=1
            except:
                with open('types.csv','a') as f4:
                    csvappend1=csv.writer(f4, delimiter='\t')
                    csvappend1.writerow(''+','+typee)
                f4.close() 
                         
            if x==1:
                with open('domain.csv','a') as f1:
                    csvappend2=csv.writer(f1, delimiter='\t')
                    csvappend2.writerow(userrr+','+'')
            if a==1:
                with open('types.csv','a') as f2:
                    csvappendd=csv.writer(f2, delimiter='\t')
                    csvappendd.writerow(''+','+objects)
            with open('Access.csv','r') as f3:
                reader3=csv.reader(f3)
                for i in reader3:
                    operation1=i[0].replace('\t','')
                    user11=i[1].replace('\t','')
                    objectss=i[2].replace('\t','')
                    if operation==operation1 and userrr==user11 and objects==objectss:
                        b=0
                        break
                    else:
                        b=1
            if b==1:
                with open('Access.csv','a') as f3:
                    csvappend3=csv.writer(f3, delimiter='\t')
                    csvappend3.writerow(operation+','+userrr+','+objects)
                    print('Success')
                                
    except:
        try:
            with open('domain.csv','r') as f1:
                readerr=csv.reader(f1)
                for line in readerr:
                    domain1=line[0].replace('\t','')
                    if userrr==domain1:
                        x=0
                        break
                    else:
                        x=1
        except:
            with open('domain.csv','a') as f1:
                csvappend2=csv.writer(f1, delimiter='\t')
                csvappend2.writerow(domain+','+'')
            f1.close()
                
        try:
            with open('types.csv','r') as f2:
                reader2=csv.reader(f2)
                for line in reader2:
                    types1=line[1].replace('\t','')
                    if objects==types1:
                        a=0
                        break
                    else:
                        a=1
        except:
            with open('types.csv','a') as f2:
                csvappend1=csv.writer(f2, delimiter='\t')
                csvappend1.writerow(''+','+typee)
            f2.close()
            
        if x==1:
            with open('domain.csv','a') as f3:
                csvappend2=csv.writer(f3, delimiter='\t')
                csvappend2.writerow(userrr+','+'')
                f3.close()
        if a==1:
            with open('types.csv','a') as f4:
                csvappendd=csv.writer(f4, delimiter='\t')
                csvappendd.writerow(''+','+objects)
                f4.close()
                
        with open('Access.csv','a') as f5:
            csvappend2=csv.writer(f5, delimiter='\t')
            csvappend2.writerow(operation+','+userrr+','+objects)
            print('Success')
            f5.close()
            
def CanAccess(operation,user,objectt):
    try:
        x=0
        a=0
        list_user=[]
        list_object=[]
        with open('domain.csv','r') as f1:
            reader1=csv.reader(f1)
            for line in reader1:
                users=line[1].replace('\t','')
                if user==users and line[1]!='':
                    list_user.append(line[0])
                else:
                    pass
        with open('types.csv','r') as f2:
            reader2=csv.reader(f2)
            for line in reader2:
                objects=line[0].replace('\t','')
                if objectt==objects and line[0]!='':
                    list_object.append(line[1])
                else:
                    pass
        with open('Access.csv','r') as f3:
            reader3=csv.reader(f3)
            for line in reader3:
                operations=line[0].replace('\t','')
                if operation==operations:
                    x=1
                    break
                else:
                    x=0
        if list_user==[]:
            print('Error: access denied')
        elif list_object==[]:
            print('Error: access denied')
        elif x==0:
            print('Error: access denied')
        else:
            with open('Access.csv','r') as f3:
                reader3=csv.reader(f3)
                for line in reader3:
                    op=line[0].replace('\t','')
                    if a==1:
                        break
                    for i in list_user:
                        if a==1:
                            break
                        else: 
                            for j in list_object:
                                if operation==op and i.replace('\t','')==line[1].replace('\t','') and j.replace('\t','')==line[2].replace('\t',''):
                                    print('Success')
                                    a=1
                                    break
                                else:
                                    a=0                                 
            if a==0:
                print('Error: access denied')
    except:
        print('Error: access denied')
        
                        
                    

                

        
if __name__== "__main__":
    try:
        fun_name=sys.argv[1]
        
        if fun_name=='AddUser':  
            if len(sys.argv)>4:
                print("Error: too many input arguments")
            else: 
                AddUser(sys.argv[2], sys.argv[3])
            
        elif fun_name=='Authenticate':
            if len(sys.argv)>4:
                print("Error: too many input arguments")
            else: 
                Authenticate(sys.argv[2], sys.argv[3])
            
        elif fun_name=='SetDomain':
            if len(sys.argv)>5:
                print("Error: too many input arguments")
            else: 
                SetDomain(sys.argv[2], sys.argv[3])
            
        elif fun_name=='DomainInfo':
            if len(sys.argv)>3:
                print("Error: too many input arguments")
            else: 
                DomainInfo(sys.argv[2])
            
        elif fun_name=='SetType':
            if len(sys.argv)>4:
                print("Error: too many input arguments")
            else: 
                SetType(sys.argv[2],sys.argv[3])
            
        elif fun_name=='TypeInfo':
            if len(sys.argv)>3:
                print("Error: too many input arguments")
            else: 
                TypeInfo(sys.argv[2])
            
        elif fun_name=='AddAccess':
            if len(sys.argv)>5:
                print("Error: too many input arguments")
            else: 
                AddAccess(sys.argv[2],sys.argv[3],sys.argv[4])
               
        elif fun_name=='CanAccess':
            if len(sys.argv)>5:
                print("Error: too many input arguments")
            else: 
                CanAccess(sys.argv[2],sys.argv[3],sys.argv[4])
        
        else:
            print('Error: function not exists')
            
    except:
        print("Error: missing input arguments")
    
    
    
   
