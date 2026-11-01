#include"iostream"
template<typename T>
T absoluteDifference(T &a, T &b)
{
    T Delta_v = 0;
    if(a - b < 0)
    {
        Delta_v = b - a;
    }
    else
    {
        Delta_v = a - b;
    }
    return Delta_v;
}