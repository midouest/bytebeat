#include <SC_PlugIn.hpp>

#include <string>
#include <stdexcept>

#include "ByteBeat.hpp"
#include "parse.hpp"

#define BYTEBEAT_SAMPLERATE 8000

static InterfaceTable *ft;

namespace ByteBeat
{
    ByteBeat::ByteBeat()
    {
        mCalcFunc = make_calc_function<ByteBeat, &ByteBeat::next>();

        // Initialize with no audio to avoid popping/unitialized buffer.
        // Expressions are evaluated as 32-bit signed integers, then cast to
        // 8-bit unsigned integers, then transformed to floats with a range of
        // +/-1.0. An expression that emits a constant value of 128 will
        // correspond roughly to 0.0 at the output.
        mExpression = bb::ExpressionPtr(new bb::Constant(128));
    }

    void ByteBeat::parse(const char *input)
    {
        string s = input;

        try
        {
            mExpression = bb::parse(s);
        }
        catch (invalid_argument &ex)
        {
            Print("%s", ex.what());
            // TODO: Send back to client somehow. May not be possible without a
            //       more general sendResponse interface.
            // See: https://scsynth.org/t/scsynth-plugincmd-and-sending-responses/2638
        }
    }

    void ByteBeat::next(int nSamples)
    {
        const float *tBuf = in(0);
        float *outBuf = out(0);

        for (int i = 0; i < nSamples; ++i)
        {
            int t = tBuf[i];
            float sample;

            // In most cases, the t input will change slower than the sample
            // rate. Save some cycles by only evaluating the expression when
            // t changes.
            if (t == mPrevT)
            {
                sample = mPrevSample;
            }
            else
            {
                uint8_t byte = mExpression->evaluate(t);
                sample = 2 * (float)byte / 255 - 1;
                mPrevT = t;
                mPrevSample = sample;
            }

            outBuf[i] = sample;
        }
    }

    /**
     * Unit command callback for the /eval command. Expects args to contain
     * a single string argument representing the new bytebeat expression.
     */
    void evalCmd(ByteBeat *unit, sc_msg_iter *args)
    {
        unit->parse(args->gets());
    }
}

PluginLoad(ByteBeat)
{
    ft = inTable;

    registerUnit<ByteBeat::ByteBeat>(ft, "ByteBeat", false);

    DefineUnitCmd("ByteBeat", "/eval", (UnitCmdFunc)ByteBeat::evalCmd);
}
