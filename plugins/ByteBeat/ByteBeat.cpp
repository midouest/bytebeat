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
}

void ByteBeat::parse(const char *input)
{
    string s = input;
    try
    {
        mInterpreter.parse(s);
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
        outBuf[i] = mInterpreter.eval(tBuf[i]);
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
