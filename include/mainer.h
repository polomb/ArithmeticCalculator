#ifndef __MAINER__
#define __MAINER__

#include <string>
#include <memory>
#include <unordered_map>

#include "m_leksich_analizer.h"
#include "m_sintaksis_analizer.h"
#include "m_translator_polski.h"

class Calculator
{
    std::unique_ptr<LeksichAnalizer> leksich;
    std::unique_ptr<SintaksisAnalizer> sintaksis;
    std::unique_ptr<TranslatorPolski> translator;

public:
    Calculator();   

    double ArithmeticCalculator(const std::string& expression, std::unordered_map<std::string, double>& peremF);
};

#endif

