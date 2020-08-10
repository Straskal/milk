// if (os.getenv("LOCAL_LUA_DEBUGGER_VSCODE") == "1") {
//     // @ts-ignore
//     import * as lldbg from "lldebugger";
//     lldbg.start();
// }

import { Game } from "./scripts/game";
import { GameplayState } from "./scripts/states/gameplayState";

_init   = () => Game.init(new GameplayState());
_update = () => Game.update();
_draw   = () => Game.draw();
