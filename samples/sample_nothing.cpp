#include <iostream>
#include <string>
#include <unordered_map>

#include "mainer.h"

int main()
{
    Calculator Polsha;
    std::unordered_map<std::string, double> perem;
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
            double result = Polsha.ArithmeticCalculator(input, perem);
            std::cout << "Result: " << result << std::endl;
        }
        catch (const std::exception& excep)
        {
            std::cout << "Error: " << excep.what() << std::endl;
        }
    }
    return 0;
}