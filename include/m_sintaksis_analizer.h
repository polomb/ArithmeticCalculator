#ifndef __M_SINTAKSIS_ANALIZER__
#define __M_SINTAKSIS_ANALIZER__

#include <string>
#include <vector>

#include "token.h"

class SintaksisAnalizer
{
public:
    SintaksisAnalizer();
    ~SintaksisAnalizer();

    bool isCorrect(const std::vector<Token*>& tokens);
};

#endif

