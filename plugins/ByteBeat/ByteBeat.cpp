#include <SC_PlugIn.hpp>

#include <stdexcept>
#include <string>

#include "ByteBeat.hpp"
#include "parse.hpp"

static InterfaceTable *ft;

namespace ByteBeat
{

InterpreterUnit::InterpreterUnit() : mAst(bb::AstPtr(new bb::Undefined())) {}

void InterpreterUnit::parse(const char *input)
{
    try
    {
        mAst = bb::parse(string{input});
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

inline float InterpreterUnit::sample(int t) const
{
    bb::Value val = mAst->eval(t);
    if (!val.is_int())
    {
        return 0;
    }
    uint8_t byte = val.to_int();
    return 2 * (float)byte / 255 - 1;
}

ByteBeat::ByteBeat() : mPrevT(-1), mPrevSample(0)
{
    mCalcFunc = make_calc_function<ByteBeat, &ByteBeat::next>();
}

void ByteBeat::next(int nSamples)
{
    const float *tBuf = in(0);
    float *outBuf = out(0);

    for (int i = 0; i < nSamples; ++i)
    {
        float s;
        int t = tBuf[i];
        if (t == mPrevT)
        {
            s = mPrevSample;
        }
        else
        {
            s = sample(t);
            mPrevT = t;
            mPrevSample = s;
        }
        outBuf[i] = s;
    }
}

inline float GRAIN_IN_AT(Unit *unit, int index, int offset)
{
    if (INRATE(index) == calc_FullRate)
        return IN(index)[offset];
    if (INRATE(index) == calc_DemandRate)
        return DEMANDINPUT_A(index, offset + 1);
    return IN0(index);
}

template <bool full_rate>
inline float grain_in_at(Unit *unit, int index, int offset)
{
    if (full_rate)
        return GRAIN_IN_AT(unit, index, offset);

    if (INRATE(index) == calc_DemandRate)
        return DEMANDINPUT_A(index, offset + 1);
    else
        return IN0(index);
}

#define DECLARE_WINDOW                                                         \
    double winPos, winInc, w, b1, y1, y2, y0;                                  \
    float amp;                                                                 \
    winPos = winInc = w = b1 = y1 = y2 = y0 = amp = 0.;                        \
    SndBuf *window;                                                            \
    const float *windowData __attribute__((__unused__)) = 0;                   \
    uint32 windowSamples __attribute__((__unused__)) = 0;                      \
    uint32 windowFrames __attribute__((__unused__)) = 0;                       \
    int windowGuardFrame = 0;

#define GET_GRAIN_WIN_RELAXED(WINTYPE)                                         \
    do                                                                         \
    {                                                                          \
        assert(WINTYPE < mWorld->mNumSndBufs);                                 \
        window = mWorld->mSndBufs + (int)WINTYPE;                              \
        windowData = window->data;                                             \
        windowSamples = window->samples;                                       \
        windowFrames = window->frames;                                         \
        windowGuardFrame = windowFrames - 1;                                   \
    } while (0);

static inline bool getGrainWin(Unit *unit, float wintype, SndBuf *&window,
                               const float *&windowData, uint32 &windowSamples,
                               uint32 &windowFrames, int &windowGuardFrame)
{
    if (wintype >= unit->mWorld->mNumSndBufs)
    {
        Print("Envelope buffer out of range!\n");
        return false;
    }

    assert(wintype < unit->mWorld->mNumSndBufs);

    if (wintype < 0)
        return true; // use default hann window

    window = unit->mWorld->mSndBufs + (int)wintype;
    windowData = window->data;
    if (!windowData)
        return false;

    windowSamples = window->samples;
    windowFrames = window->frames;
    windowGuardFrame = windowFrames - 1;

    return true;
}

#define CALC_GRAIN_PAN                                                         \
    float panangle, pan1, pan2;                                                \
    float *out1, *out2;                                                        \
    if (numOutputs > 1)                                                        \
    {                                                                          \
        if (numOutputs > 2)                                                    \
        {                                                                      \
            pan = sc_wrap(pan * 0.5f, 0.f, 1.f);                               \
            float cpan = numOutputs * pan + 0.5f;                              \
            float ipan = floor(cpan);                                          \
            float panfrac = cpan - ipan;                                       \
            panangle = panfrac * pi2_f;                                        \
            grain->chan = (int)ipan;                                           \
            if (grain->chan >= (int)numOutputs)                                \
                grain->chan -= numOutputs;                                     \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            grain->chan = 0;                                                   \
            pan = sc_clip(pan * 0.5f + 0.5f, 0.f, 1.f);                        \
            panangle = pan * pi2_f;                                            \
        }                                                                      \
        pan1 = grain->pan1 = cos(panangle);                                    \
        pan2 = grain->pan2 = sin(panangle);                                    \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        grain->chan = 0;                                                       \
        pan1 = grain->pan1 = 1.;                                               \
        pan2 = grain->pan2 = 0.;                                               \
    }

#define GET_GRAIN_INIT_AMP                                                     \
    if (grain->winType < 0.)                                                   \
    {                                                                          \
        w = pi / counter;                                                      \
        b1 = grain->b1 = 2. * cos(w);                                          \
        y1 = sin(w);                                                           \
        y2 = 0.;                                                               \
        amp = y1 * y1;                                                         \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        amp = windowData[0];                                                   \
        winPos = grain->winPos = 0.f;                                          \
        winInc = grain->winInc = (double)windowSamples / counter;              \
    }

#define CALC_NEXT_GRAIN_AMP_INTERNAL                                           \
    do                                                                         \
    {                                                                          \
        y0 = b1 * y1 - y2;                                                     \
        y2 = y1;                                                               \
        y1 = y0;                                                               \
        amp = y1 * y1;                                                         \
    } while (0)

#define CALC_NEXT_GRAIN_AMP_CUSTOM                                             \
    do                                                                         \
    {                                                                          \
        winPos += winInc;                                                      \
        int iWinPos = (int)winPos;                                             \
        double winFrac = winPos - (double)iWinPos;                             \
        const float *winTable1 = windowData + iWinPos;                         \
        const float *winTable2 = winTable1 + 1;                                \
        if (!windowData)                                                       \
            break;                                                             \
        if (winPos > windowGuardFrame)                                         \
            winTable2 -= windowSamples;                                        \
        amp = lininterp(winFrac, winTable1[0], winTable2[0]);                  \
    } while (0);                                                               \
    if (!windowData)                                                           \
        break;

#define CALC_NEXT_GRAIN_AMP                                                    \
    if (grain->winType < 0.)                                                   \
    {                                                                          \
        CALC_NEXT_GRAIN_AMP_INTERNAL;                                          \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        CALC_NEXT_GRAIN_AMP_CUSTOM;                                            \
    }

#define GET_GRAIN_AMP_PARAMS                                                   \
    if (grain->winType < 0.)                                                   \
    {                                                                          \
        b1 = grain->b1;                                                        \
        y1 = grain->y1;                                                        \
        y2 = grain->y2;                                                        \
        amp = grain->curamp;                                                   \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        GET_GRAIN_WIN_RELAXED(grain->winType);                                 \
        if (!windowData)                                                       \
            break;                                                             \
        winPos = grain->winPos;                                                \
        winInc = grain->winInc;                                                \
        amp = grain->curamp;                                                   \
    }

#define SAVE_GRAIN_AMP_PARAMS                                                  \
    grain->y1 = y1;                                                            \
    grain->y2 = y2;                                                            \
    grain->winPos = winPos;                                                    \
    grain->winInc = winInc;                                                    \
    grain->curamp = amp;                                                       \
    grain->counter -= nsmps;

#define WRAP_CHAN(offset)                                                      \
    out1 = out(grain->chan) + offset;                                          \
    if (numOutputs > 1)                                                        \
    {                                                                          \
        if ((grain->chan + 1) >= (int)numOutputs)                              \
            out2 = out(0) + offset;                                            \
        else                                                                   \
            out2 = out(grain->chan + 1) + offset;                              \
    }

#define GET_PAN_PARAMS                                                         \
    float pan1 = grain->pan1;                                                  \
    uint32 chan1 = grain->chan;                                                \
    float *out1 = out(chan1);                                                  \
    if (numOutputs > 1)                                                        \
    {                                                                          \
        pan2 = grain->pan2;                                                    \
        uint32 chan2 = chan1 + 1;                                              \
        if (chan2 >= numOutputs)                                               \
            chan2 = 0;                                                         \
        out2 = out(chan2);                                                     \
    }

ByteGrain::ByteGrain()
{
    if (inRate(0) == calc_FullRate)
    {
        mCalcFunc = make_calc_function<ByteGrain, &ByteGrain::next_a>();
    }
    else
    {
        mCalcFunc = make_calc_function<ByteGrain, &ByteGrain::next_k>();
    }

    mNumActive = 0;
    curtrig = 0.f;
    mFirst = true;

    next_k(1);
}

ByteGrain::~ByteGrain() { RTFree(mWorld, mGrains); }

void ByteGrain::next_a(int inNumSamples)
{
    ClearUnitOutputs(this, inNumSamples);

    play_active(inNumSamples);

    const float *trig = in(0);
    for (int i = 0; i < inNumSamples; ++i)
    {
        if ((curtrig <= 0) && (trig[i] > 0.0))
        {
            start_new<true>(inNumSamples, i);
        }
        curtrig = trig[i];
    }
}

void ByteGrain::next_k(int inNumSamples)
{
    ClearUnitOutputs(this, inNumSamples);
    if (mFirst)
    {
        mFirst = false;
        float maxGrains = in0(5);
        mMaxGrains = (int)maxGrains;
        mGrains =
            (ByteGrainG *)RTAlloc(mWorld, mMaxGrains * sizeof(ByteGrainG));
    }

    play_active(inNumSamples);

    float trig = in0(0);

    if ((curtrig <= 0) && (trig > 0.0))
    {
        start_new<false>(inNumSamples, 0);
    }
    curtrig = trig;
}

template <bool full_rate>
void ByteGrain::start_new(int inNumSamples, int position)
{
    if (mNumActive + 1 >= mMaxGrains)
    {
        Print("Too many grains!\n");
        return;
    }

    float winType = grain_in_at<full_rate>(this, 4, position);
    DECLARE_WINDOW
    bool success = getGrainWin(this, winType, window, windowData, windowSamples,
                               windowFrames, windowGuardFrame);
    if (!success)
    {
        return;
    }

    ByteGrainG *grain = mGrains + mNumActive++;
    float winSize = grain_in_at<full_rate>(this, 1, position);
    double counter = winSize * mRate->mSampleRate;
    counter = sc_max(4., counter);
    grain->counter = (int)counter;
    grain->winType = winType;
    grain->prevT = -1;
    grain->prevSample = 0;

    GET_GRAIN_INIT_AMP

    const uint32 numOutputs = mNumOutputs;
    const float *tBuf = in(2);

    const float *tBuf1 = tBuf + position;
    // begin add //
    float pan = grain_in_at<full_rate>(this, 3, position);

    CALC_GRAIN_PAN

    WRAP_CHAN(position)

    // end add //

    int nsmps = sc_min(grain->counter, inNumSamples - position);
    for (int j = 0; j < nsmps; ++j)
    {
        float s;
        int t = tBuf1[j];
        // if (t == grain->prevT)
        // {
        //     s = grain->prevSample;
        // }
        // else
        // {
        s = sample(t);
        grain->prevT = t;
        grain->prevSample = s;
        // }
        float outval = amp * s;

        // // begin add / change
        out1[j] += outval * pan1;
        if (numOutputs > 1)
        {
            out2[j] += outval * pan2;
        }
        // end add / change

        CALC_NEXT_GRAIN_AMP
    }

    SAVE_GRAIN_AMP_PARAMS

    if (grain->counter <= 0)
    {
        *grain = mGrains[--mNumActive]; // remove grain
    }
}

void ByteGrain::play_active(int inNumSamples)
{
    const unsigned int numOutputs = mNumOutputs;
    const float *tBuf = in(2);
    for (int i = 0; i < mNumActive;)
    {
        ByteGrainG *grain = mGrains + i;
        DECLARE_WINDOW
        GET_GRAIN_AMP_PARAMS

        // begin add //

        float pan2 = 0.f;
        float *out2;

        GET_PAN_PARAMS

        // end add //
        int nsmps = sc_min(grain->counter, inNumSamples);
        for (int j = 0; j < nsmps; ++j)
        {
            float s;
            int t = tBuf[j];
            // if (t == grain->prevT)
            // {
            //     s = grain->prevSample;
            // }
            // else
            // {
            s = sample(t);
            grain->prevT = t;
            grain->prevSample = s;
            // }
            float outval = amp * s;

            // // begin add / change
            out1[j] += outval * pan1;
            if (numOutputs > 1)
            {
                out2[j] += outval * pan2;
            }
            // end add / change

            CALC_NEXT_GRAIN_AMP
        }

        SAVE_GRAIN_AMP_PARAMS

        if (grain->counter <= 0)
        {
            *grain = mGrains[--mNumActive]; // remove grain
        }
        else
        {
            ++i;
        }
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
