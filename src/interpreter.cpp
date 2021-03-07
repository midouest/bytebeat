#include "interpreter.hpp"
#include "parse.hpp"

namespace bb
{

void Interpreter::parse(const string &input) { mAst = bb::parse(input); }

float Interpreter::eval(int t)
{
    if (t == mPrevT)
    {
        return mPrevSample;
    }

    float sample = 0;
    Value val = mAst->eval(t);
    if (val.is_int())
    {
        uint8_t byte = val.to_int();
        sample = 2 * (float)byte / 255 - 1;
    }
    mPrevT = t;
    mPrevSample = sample;

    return sample;
}

} // namespace bb
