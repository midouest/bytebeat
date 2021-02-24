ByteBeatController {
    var synth, synthIndex;

    *new { arg synth, synthIndex;
        ^super.newCopyArgs(synth, synthIndex);
    }

    setExpression { arg input, restart = 0;
        this.sendMsg('/setexpr', input, restart)
    }

    restart {
        this.sendMsg('/restart')
    }

    sendMsg { arg cmd ... args;
        synth.server.sendMsg('/u_cmd', synth.nodeID, synthIndex, cmd, *args)
    }
}
