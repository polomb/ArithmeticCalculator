#include "mainer.h"
#include <stdexcept>

Calculator::Calculator()
{
    sintaksis = std::make_unique<SintaksisAnalizer>();
    translator = std::make_unique<TranslatorPolski>();
}

double Calculator::ArithmeticCalculator(const std::string& expression, std::unordered_map<std::string, double>& perem)
{
    //Leksika
    leksich = std::make_unique<LeksichAnalizer>(expression);
    std::vector<std::unique_ptr<Token>> tokens = leksich->tokenize();
    //Sintaksis
    sintaksis->isCorrect(tokens, perem);
    //PolskayaZapis
    std::vector<std::unique_ptr<Token>> polishTokens = translator->toPolishNotation(tokens);
    //Calculation
    double result = translator->calculate(polishTokens, perem);

    return result;
}

