#pragma once

#include "ast.hpp"

namespace bb
{

class Interpreter
{
public:
    void parse(const string &input);
    float eval(int t);

private:
    float mPrevSample = 0;
    int mPrevT = 0;
    AstPtr mAst = AstPtr(new Undefined());
};

} // namespace bb
