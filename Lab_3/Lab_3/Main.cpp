#include <iostream>
#include "Wrapper.h"

class Subject
{
public:

    int f3(int a, int b)
    {
        return a + b;
    }
};

int main()
{
    Subject subj;
    Wrapper wrapper(&subj, &Subject::f3, { {"arg1", 0}, {"arg2", 0} });
}

