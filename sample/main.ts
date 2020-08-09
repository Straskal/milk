if (os.getenv("LOCAL_LUA_DEBUGGER_VSCODE") == "1") {
    // @ts-ignore
    import * as lldbg from "lldebugger";
    lldbg.start();
}

import { Game } from "./scripts/game";

_init   = () => Game.init();
_update = () => Game.update();
_draw   = () => Game.draw();
