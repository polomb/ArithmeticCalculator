#ifndef __MAINER__
#define __MAINER__

#include <string>
#include "m_leksich_analizer.h"
#include "m_sintaksis_analizer.h"
#include "m_translator_polski.h"

class Calculator
{
    LeksichAnalizer* leksich;
    SintaksisAnalizer* sintaksis;
    TranslatorPolski* translator;

public:
    Calculator();
    ~Calculator();

    double ArithmeticCalculator(const std::string& expression);
};

#endif

