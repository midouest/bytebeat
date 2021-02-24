#include <SC_PlugIn.hpp>

#include <string>
#include <stdexcept>

#include "ByteBeat.hpp"
#include "parse.hpp"

#define BYTEBEAT_SAMPLERATE 8000

static InterfaceTable *ft;

namespace ByteBeat
{
    ByteBeat::ByteBeat() : mSampleStep(BYTEBEAT_SAMPLERATE / sampleRate())
    {
        mCalcFunc = make_calc_function<ByteBeat, &ByteBeat::next>();

        // Initialize with no audio to avoid popping/unitialized buffer.
        // Expressions are evaluated as 32-bit signed integers, then cast to
        // 8-bit unsigned integers, then transformed to floats with a range of
        // +/-1.0. An expression that emits a constant value of 128 will
        // correspond roughly to 0.0 at the output.
        mExpression = new bb::Constant(128);
    }

    ByteBeat::~ByteBeat()
    {
        delete mExpression;
    }

    void ByteBeat::setExpression(const char *input)
    {
        string s = input;

        try
        {
            bb::Expression *prevExpr = mExpression;
            mExpression = bb::parse(s);
            mNextSample = sampleByteBeat();
            delete prevExpr;
        }
        catch (invalid_argument &ex)
        {
            Print("%s", ex.what());
            // TODO: Send back to client somehow. May not be possible without a
            //       more general sendResponse interface.
            // See: https://scsynth.org/t/scsynth-plugincmd-and-sending-responses/2638
        }
    }

    void ByteBeat::restart()
    {
        mTime = 0;
    }

    void ByteBeat::next(int nSamples)
    {
        float *outbuf = out(0);

        for (int i = 0; i < nSamples; ++i)
        {
            float data = lininterp(mAccumulator, mPrevSample, mNextSample);
            outbuf[i] = data;

            mAccumulator += mSampleStep;
            if (mAccumulator >= 1)
            {
                mAccumulator -= 1;
                ++mTime;
                mPrevSample = mNextSample;
                mNextSample = sampleByteBeat();
            }
        }
    }

    inline float ByteBeat::sampleByteBeat() const
    {
        uint8_t sample = mExpression->evaluate(mTime);
        return 2 * (float)sample / 255 - 1;
    }

    /**
     * Unit command callback for the /setexpr command. Expects args to contain
     * a single string argument representing the new bytebeat expression.
     */
    void setExprCmd(ByteBeat *unit, sc_msg_iter *args)
    {
        unit->setExpression(args->gets());
        if (args->geti(1))
        {
            unit->restart();
        }
    }

    /**
     * Unit command callback for the /restart command. Resets the bytebeat time
     * counter to 0.
     */
    void restartCmd(ByteBeat *unit, sc_msg_iter *args)
    {
        unit->restart();
    }
}

PluginLoad(ByteBeat)
{
    ft = inTable;

    registerUnit<ByteBeat::ByteBeat>(ft, "ByteBeat", false);

    DefineUnitCmd("ByteBeat", "/setexpr", (UnitCmdFunc)ByteBeat::setExprCmd);
    DefineUnitCmd("ByteBeat", "/restart", (UnitCmdFunc)ByteBeat::restartCmd);
}
