#include "t_number.h"

#include <gtest.h>

TEST(NumberTest, DefaultConstructor)
{
    TNumber num(123.456);
    EXPECT_DOUBLE_EQ(num.getNumberValue(), 123.456);
}
TEST(NumberTest, CorrectType)
{
    TNumber num(1.0);
    EXPECT_EQ(num.getType(), TokenType::NUMBER);
}