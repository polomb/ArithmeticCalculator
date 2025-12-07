#include "m_leksich_analizer.h"

#include <gtest.h>

TEST(LeksichAnalizerTest, CanMakeAllTokens)
{
    std::string expression = "123 + - * / perem_1 ( )";
    LeksichAnalizer* leksich = new LeksichAnalizer(expression);
    std::vector<std::unique_ptr<Token>>& tokens = leksich->tokenize();
    EXPECT_EQ(TokenType::NUMBER, tokens[0]->getType());
    EXPECT_EQ(TokenType::OPERATION, tokens[1]->getType());
    EXPECT_EQ(TokenType::OPERATION, tokens[2]->getType());
    EXPECT_EQ(TokenType::OPERATION, tokens[3]->getType());
    EXPECT_EQ(TokenType::OPERATION, tokens[4]->getType());
    EXPECT_EQ(TokenType::PEREMENNAYA, tokens[5]->getType());
    EXPECT_EQ(TokenType::SKOBKI, tokens[6]->getType());
    EXPECT_EQ(TokenType::SKOBKI, tokens[7]->getType());
    //ADD_FAILURE();
}