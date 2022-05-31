To run my project, simply copy auth.py to "Home" on ilab, and use python3. 
example: python3 auth.py AddUser john 123

Please be aware that my file is sensitive about cases, so if you entered 'adduSer' instead of 'AddUSer', my program will think it's a wrong function. 
This also applies to other arguments like 'John' will not be the same as 'john'.

I used csv files to store data, so you'll see "import csv" at the top. 
I used /t as delimiter and ',' to separate different entities. 
In wps office, we can see john at (1,1) of the table and (123) at (1,2). This translates to 'j\to\th\tn\t,1\t2\t3\t'. 


#test cases: 

python3 auth.py AddUser john 123
python3 auth.py AddUser mary 456
python3 auth.py AddUser "" 123

python3 auth.py Authenticate eric 234
python3 auth.py Authenticate mary 876
python3 auth.py Authenticate john 123

python3 auth.py SetDomain tony cook
python3 auth.py SetDomain mary cook
python3 auth.py SetDomain mary cloth

python3 auth.py DomainInfo hiking
python3 auth.py DomainInfo career
python3 auth.py DomainInfo cook

python3 auth.py SetType "" cartoon
python3 auth.py SetType movie ""
python3 auth.py SetType cake backery
python3 auth.py SetType bread backery
python3 auth.py SetType blue color

python3 auth.py TypeInfo color
python3 auth.py TypeInfo backery
python3 auth.py TypeInfo sport

python3 auth.py AddAccess eat cook backery
python3 auth.py AddAccess "" swing spring

python3 auth.py CanAccess set cook color
python3 auth.py CanAccess eat mary cake
python3 auth.py CanAccess eat mary cake key
python3 auth.py CanAccess eat mary

#results: 

Success
Success
Error: username missing
Error: no such user
Error: bad password
Success
Error: no such user
Success
Success
Error: domain not exists
Error: domain not exists
mary
Error: missing object
Error: missing type
Success
Success
Success
blue_color
cake_backery
bread_backery
Error: type not exists
Success
Error: missing operation
Error: access denied
Success
Error: too many input arguments
Error: missing input arguments

