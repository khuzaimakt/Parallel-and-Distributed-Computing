#include<iostream>
#include<omp.h>
#include<chrono>

using namespace std;
using namespace std::chrono;

const int NUM_THREADS = 4;

double compute(double x) {
double result = 0;
for (int i = 0; i < 10000000; i++) 
{
result += sin(x) * cos(x) * tan(x);
}
return result;
}

int main()

{
    double initialx = 1.0;
    double initialy = 2.0;
    double f1;
    double f2;
    double f3;
    double f4;
    double f5;
    cout << "Sequential" << endl << endl;
    auto ss = high_resolution_clock::now();
    

    for(int i=0;i<=4;i++)
    {
        if(i==0)
          f1=compute(initialx);

        if (i == 1)
            f2 = compute(initialy);

        if(i==2)
            f3=compute(f1+f2);

        if(i==3)
            f4=compute(f3);
        if (i == 4)
            f5 = compute(f3);
    }

    
    auto sts = high_resolution_clock::now();
    auto ds = duration_cast<milliseconds>(sts - ss);

    cout << "F1 = " << f1 << endl;
    cout << "F2 = " << f2 << endl;
    cout << "F3 = " << f3 << endl;
    cout << "F4 = " << f4 << endl;
    cout << "F5 = " << f5 << endl;
    cout << "Sequential Time=" << ds.count() << endl << endl;

    cout << "Paralell:" << endl << endl;
    auto sp = high_resolution_clock::now();

    #pragma omp parallel num_threads(NUM_THREADS)
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                f1 = compute(initialx);
            }
            #pragma omp section
            {
                f2 = compute(initialy);
            }
        }
        #pragma omp barrier
        f3 = compute(f1 + f2);
        #pragma omp sections 
        {
            #pragma omp section
            {
                f4 = compute(f3);
            }
            #pragma omp section
            {
                f5 = compute(f3);
            }
            
        }
    }
    auto stp = high_resolution_clock::now();
    cout << "F1 = " << f1 << endl;
    cout << "F2 = " << f2 << endl;
    cout << "F3 = " << f3 << endl;
    cout << "F4 = " << f4 << endl;
    cout << "F5 = " << f5 << endl;
    auto duration_par = duration_cast<milliseconds>(stp - sp);
    cout <<"Paralell Time:"<< duration_par.count() << endl;

    
    return 0;
}