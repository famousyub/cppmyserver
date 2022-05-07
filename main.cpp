#include<iostream>
#include<boost/multiprecision/cpp_int.hpp>
#include<stdio.h>
using namespace boost::multiprecision;
using namespace std;



cpp_int  boostproduct(long long A , long long B)
{
    cpp_int  res = (cpp_int) A * B ;
    return res ;
}

cpp_int boost_factorial(int num)
{
    cpp_int fact = 1;
    for (int i = num; i > 1; --i)
        fact *= i;
    return fact;
}

int main (int argc , const char *argv[])
{


 int num = 30;
    cout << "Factorial of " << num << " = "
         << boost_factorial(num);

    if(argc >1 ){

        long long first  = atoll(argv[1]);
        long long second=atoll(argv[2]);
        
        cpp_int r =  boostproduct(first , second);

         cout << "Factorial of " << first << " = "
         << boostproduct(first,second);

        cout <<" product of" << first << " " << second <<endl ;
        long long d   =  (long long ) r ;
       
        printf("%lld",d);
        
        
         


    }
    else   {

         long long a, b ; 
         cout << "enter number one " <<endl ;
         cin>> a ; 
         cout << "enter numeber   2" << endl ;
         cin>>b; 

           cout << "Factorial of " << a << " = "
         << boostproduct(a,b);

         long long   r=  (long long ) boostproduct(a,b);

          printf("%lld",r);
    }

    return  1;
}