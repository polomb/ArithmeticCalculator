#include "m_leksich_analizer.h"
#include "m_sintaksis_analizer.h"
#include "m_translator_polski.h"

#include <gtest.h>

static std::vector<std::unique_ptr<Token>> CreatingPolish(const std::string& expression,\
    std::unordered_map<std::string, double>& perem, std::unique_ptr<TranslatorPolski>& translator)
{
    std::unique_ptr<LeksichAnalizer> leksich;
    leksich = std::make_unique<LeksichAnalizer>(expression);
    std::vector<std::unique_ptr<Token>> tokens = leksich->tokenize();
    //
    std::unique_ptr<SintaksisAnalizer> sintaksis;
    sintaksis = std::make_unique<SintaksisAnalizer>();
    sintaksis->isCorrect(tokens, perem);
    //
    std::vector<std::unique_ptr<Token>> polishTokens = translator->toPolishNotation(tokens);
    return polishTokens;
}
static std::unique_ptr<TranslatorPolski> CreatingTranslator()
{
    std::unique_ptr<TranslatorPolski> translator;
    translator = std::make_unique<TranslatorPolski>();
    return translator;
}

TEST(TranslatorPolskiTest, LN_Problem_stated_in_comment)
{
    std::string expression = "(ln(2)--ln(2))-2*ln(2)";
    std::unordered_map<std::string, double> perem;
    //
    std::unique_ptr<TranslatorPolski> translator = CreatingTranslator();
    std::vector<std::unique_ptr<Token>> polishTokens = CreatingPolish(expression, perem, translator);;
    double result = translator->calculate(polishTokens, perem);
    EXPECT_EQ(0, result);
    //ADD_FAILURE();
}