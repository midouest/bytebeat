ByteBeatController {
    var synth, synthIndex;

    *new { arg synth, synthIndex;
        ^super.newCopyArgs(synth, synthIndex);
    }

    setExpression { arg input;
        this.sendMsg('/setexpr', input)
    }

    restart {
        this.sendMsg('/restart')
    }

    sendMsg { arg cmd ... args;
        synth.server.sendMsg('/u_cmd', synth.nodeID, synthIndex, cmd, *args)
    }
}
