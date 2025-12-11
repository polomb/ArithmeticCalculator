#ifndef __M_SINTAKSIS_ANALIZER__
#define __M_SINTAKSIS_ANALIZER__

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "token.h"

class SintaksisAnalizer
{
public:
    SintaksisAnalizer();

    bool isCorrect(const std::vector<std::unique_ptr<Token>>& tokens, std::unordered_map<std::string, double>& perem);
};

#endif

