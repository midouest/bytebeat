#pragma once

#include "ast.hpp"
#include <SC_PlugIn.hpp>
#include <tuple>

using namespace std;

namespace ByteBeat
{

class InterpreterUnit : public SCUnit
{
public:
    InterpreterUnit();

    /**
     * Parse the incoming expression and replace the existing expression.
     * Does not replace the existing expression if the incoming expression
     * cannot be parsed.
     */
    void parse(const char *input);

protected:
    inline float sample(int t) const;

    bb::AstPtr mAst;
};

/**
 * ByteBeat is able to parse simple mathematical expressions and evaluate
 * them to produce audio samples.
 *
 * ByteBeat will not produce any audio output initially. The /eval unit
 * command must be used to send expressions to the synth to be parsed. Once
 * an expression has been parsed, it will become the active expression and
 * begin producing audio samples.
 *
 * ByteBeat expects a single audio-rate input, "t", that is passed to the
 * expression.
 */
class ByteBeat : public InterpreterUnit
{
public:
    ByteBeat();

private:
    /**
     * Evaluate the current bytebeat expression for the given number of
     * samples.
     */
    void next(int nSamples);

    int mPrevT;
    float mPrevSample;
};

struct ByteGrainG
{
    double b1, y1, y2, curamp, winPos, winInc;
    int counter, chan, prevT;
    float pan1, pan2, winType, prevSample;
};

class ByteGrain : public InterpreterUnit
{
public:
    ByteGrain();
    ~ByteGrain();

private:
    void next_a(int inNumSamples);
    void next_k(int inNumSamples);

    template <bool full_rate> void start_new(int inNumSamples, int position);
    void play_active(int inNumSamples);

    bool mFirst;
    int mNumActive, mChannels, mMaxGrains;
    float curtrig;
    ByteGrainG *mGrains;
};

class ByteTest : public SCUnit
{
public:
    ByteTest();

private:
    void next(int nSamples);
};

} // namespace ByteBeat
