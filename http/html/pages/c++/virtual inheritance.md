### Virtual inheritance (diamond problem) in pictures

#### You want: (Achievable with virtual inheritance)

   A  
/      \  
B     C  
 \     /  
   D
 
#### And not: (What happens without virtual inheritance)


A     A  
|       |
B     C  
 \     /  
    D 

### Virtual inheritance means that there will be only 1 instance of the base A class not 2.