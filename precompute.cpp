#include<iostream>
#include<math.h>
using namespace std;
const double k = 3.05;
int convert(double p)
{
    return (exp(k*p)-1)/(exp(k)-1)*5000+0.5;
}
int main()
{
    cout<<"{";
    for (int i=0;i<256;++i)
    {
        if (i) cout<<", ";
        cout<<convert(i/255.0);
    }
    cout<<"}\n";
    return 0;
}
