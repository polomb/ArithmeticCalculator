#include "mainer.h"
#include <stdexcept>

Calculator::Calculator()
{
    leksich = new LeksichAnalizer("");
    sintaksis = new SintaksisAnalizer();
    translator = new TranslatorPolski();
}

Calculator::~Calculator()
{
    delete leksich;
    delete sintaksis;
    delete translator;
}

double Calculator::ArithmeticCalculator(const std::string& expression)
{
    //Leksika
    delete leksich;
    leksich = new LeksichAnalizer(expression);
    std::vector<Token*> tokens = leksich->tokenize();
    //Sintaksis
    sintaksis->isCorrect(tokens);
    //PolskayaZapis
    std::vector<Token*> polishTokens = translator->toPolishNotation(tokens);
    //Calculation
    double result = translator->calculate(polishTokens);
    //ClearMemory
    for (Token* token : tokens)
        delete token;
    for (Token* token : polishTokens)
        delete token;
    return result;
}

