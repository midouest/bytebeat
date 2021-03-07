#include <SC_PlugIn.hpp>

#include <stdexcept>
#include <string>

#include "ByteBeat.hpp"
#include "parse.hpp"

static InterfaceTable *ft;

namespace ByteBeat
{

void InterpreterUnit::parse(const char *input)
{
    try
    {
        mInterpreter.parse(string{input});
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

ByteBeat::ByteBeat()
{
    mCalcFunc = make_calc_function<ByteBeat, &ByteBeat::next>();
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

ByteGrain::ByteGrain()
{
    mCalcFunc = make_calc_function<ByteGrain, &ByteGrain::next>();
}

void ByteGrain::next(int nSamples)
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
void evalCmd(InterpreterUnit *unit, sc_msg_iter *args)
{
    unit->parse(args->gets());
}

} // namespace ByteBeat

PluginLoad(ByteBeat)
{
    ft = inTable;

    registerUnit<ByteBeat::ByteBeat>(ft, "ByteBeat", false);
    registerUnit<ByteBeat::ByteGrain>(ft, "ByteGrain", false);

    DefineUnitCmd("ByteBeat", "/eval", (UnitCmdFunc)ByteBeat::evalCmd);
    DefineUnitCmd("ByteGrain", "/eval", (UnitCmdFunc)ByteBeat::evalCmd);
}
