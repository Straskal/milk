// if (os.getenv("LOCAL_LUA_DEBUGGER_VSCODE") == "1") {
//     // @ts-ignore
//     import * as lldbg from "lldebugger";
//     lldbg.start();
// }

import { Game } from "./scripts/game";

let game = new Game();

_init = () => game.init();
_update = () => game.update();
_draw = () => game.draw();
