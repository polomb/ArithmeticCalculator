#include <iostream>
#include <string>
#include "mainer.h"

int main()
{
    Calculator Polsha;
    while (true)
    {
        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);
        //
        if (input.empty())
        {
            continue;
        }
        //
        try
        {
            double result = Polsha.ArithmeticCalculator(input);
            std::cout << "Result: " << result << std::endl;
        }
        catch (const std::exception& excep)
        {
            std::cout << "Error: " << excep.what() << std::endl;
        }
    }
    return 0;
}