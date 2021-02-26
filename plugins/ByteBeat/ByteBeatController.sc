ByteBeatController {
    var synth, synthIndex;

    *new { arg synth, synthIndex;
        ^super.newCopyArgs(synth, synthIndex);
    }

    eval { arg expression;
        this.sendMsg('/eval', expression)
    }

    sendMsg { arg cmd ... args;
        synth.server.sendMsg('/u_cmd', synth.nodeID, synthIndex, cmd, *args)
    }
}
