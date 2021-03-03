#include <SC_PlugIn.hpp>

#include <stdexcept>
#include <string>

#include "ByteBeat.hpp"
#include "parse.hpp"

#define BYTEBEAT_SAMPLERATE 8000

static InterfaceTable *ft;

namespace ByteBeat
{
ByteBeat::ByteBeat()
{
    mCalcFunc = make_calc_function<ByteBeat, &ByteBeat::next>();

    // Initialize with no audio to avoid popping/unitialized buffer. The
    // Undefined expression will be evaluated as 0.0 when sampled.
    mAst = bb::AstPtr(new bb::Undefined());
}

void ByteBeat::parse(const char *input)
{
    string s = input;

    try
    {
        mAst = bb::parse(s);
    }
    catch (invalid_argument &ex)
    {
        Print("%s", ex.what());
        // TODO: Send back to client somehow. May not be possible without a
        //       more general sendResponse interface.
        // See:
        // https://scsynth.org/t/scsynth-plugincmd-and-sending-responses/2638
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
            bb::Value val = mAst->eval(t);
            if (val.is_int())
            {
                uint8_t byte = val.to_int();
                sample = 2 * (float)byte / 255 - 1;
            }
            else
            {
                sample = 0;
            }
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
void evalCmd(ByteBeat *unit, sc_msg_iter *args) { unit->parse(args->gets()); }
} // namespace ByteBeat

PluginLoad(ByteBeat)
{
    ft = inTable;

    registerUnit<ByteBeat::ByteBeat>(ft, "ByteBeat", false);

    DefineUnitCmd("ByteBeat", "/eval", (UnitCmdFunc)ByteBeat::evalCmd);
}
