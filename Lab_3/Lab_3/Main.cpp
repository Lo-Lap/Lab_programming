#include <iostream>
#include "Wrapper.h"
#include "Engine.h"


class Subject
{
public:
    int square(int a)
    {
        return a * a;
    }

    int f2(int a, int b, int c)
    {
        return a * b + c;
    }

    int f3(int a, int b)
    {
        return square(a) + square(b);
    }
};

int main()
{
    Subject subj;

    //call a template class with no template arguments
    //possible starting with standard c+17
    Wrapper wrapper(&subj, &Subject::f3, { {"arg1", 0}, {"arg2", 0} });

    Engine engine;

    engine.register_command(&wrapper, "command1");

    std::cout << engine.execute("command1", { {"arg1", 4}, {"arg3", 5} }) << std::endl;


    //health check for another example

    Wrapper wrapper1(&subj, &Subject::f2, { {"arg1", 0}, {"arg2", 0}, {"arg3", 0} });

    engine.register_command(&wrapper1, "command2");

    //the output should be the same
    std::cout << engine.execute("command2", { {"arg1", 4}, {"arg2", 5}, {"arg3", 0} }) << std::endl;

    std::cout << engine.execute("command2", { {"arg1", 4}, {"arg3", 0}, {"arg2", 5} }) << std::endl;

}

