ByteBeat : UGen {
    *ar { arg t=0.0, mul=1.0, add=0.0;
        ^this.multiNew('audio', t).madd(mul, add);
    }

    checkInputs {
        if (inputs.at(0).rate != 'audio') {
            ^("t input is not audio rate: " + inputs.at(0) + inputs.at(0).rate);
        }
        ^this.checkValidInputs;
    }
}

ByteGrain : MultiOutUGen {
    *ar { arg numChannels = 1, trigger = 0, dur = 1, t = 0.0, pan = 0,
              envBufNum = -1, maxGrains = 512, mul = 1.0, add = 0.0;
        ^this.multiNew(
            'audio', numChannels, trigger, dur, t, pan, envBufNum, maxGrains
        ).madd(mul, add);
    }

    init { arg argNumChannels ... theInputs;
        inputs = theInputs;
        ^this.initOutputs(argNumChannels, rate);
    }

    argNamesInputsOffset { ^2 }

    checkInputs {
        if (inputs.at(0).rate != 'control') {
            ^("trigger input is not control rate: " + inputs.at(0) + inputs.at(0).rate);
        };
        if (inputs.at(2).rate != 'audio') {
            ^("counter input is not audio rate: " + inputs.at(2) + inputs.at(2).rate);
        };
        ^this.checkValidInputs;
    }
}
