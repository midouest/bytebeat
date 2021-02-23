ByteBeatController {
    var synth;

    *new {
        arg synth;
        ^super.newCopyArgs(synth);
    }

    setExpr {
        arg input;
        synth.server.sendMsg('/u_cmd', synth.nodeID, 0, "set_expr", input);
    }
}
