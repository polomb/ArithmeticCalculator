#include "mainer.h"
#include <stdexcept>

Calculator::Calculator()
{
    sintaksis = std::make_unique<SintaksisAnalizer>();
    translator = std::make_unique<TranslatorPolski>();
}

double Calculator::ArithmeticCalculator(const std::string& expression)
{
    //Leksika
    leksich = std::make_unique<LeksichAnalizer>(expression);
    std::vector<std::unique_ptr<Token>> tokens = leksich->tokenize();
    //Sintaksis
    sintaksis->isCorrect(tokens);
    //PolskayaZapis
    std::vector<std::unique_ptr<Token>> polishTokens = translator->toPolishNotation(tokens);
    //Calculation
    double result = translator->calculate(polishTokens);

    return result;
}

