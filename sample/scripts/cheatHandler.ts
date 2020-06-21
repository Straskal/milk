import { Game } from "./game";
import { GameplayState } from "./gameplay/state";

interface HandlerRegistration {
    cheat: string,
    handle: (game: Game, args: string[]) => void;
}

function addScore(game: Game, args: string[]) {
    const argCount = args.length;
    if (argCount != 1)
        return;
    const scoreAddition = tonumber(args[0]);
    if (scoreAddition == null)
        return;

    const top = game.peek();
    if (top instanceof GameplayState) {
        const gameplayState = <GameplayState>top;
        gameplayState.addToScore(scoreAddition);
    }
}

export class CheatHandler {

    private readonly _handlers: HandlerRegistration[] = [
        { cheat: "addscore", handle: addScore }
    ];

    constructor(private readonly _game: Game) { }

    public handleCheat(cheat: string, args: string[]) {
        const handler = this._handlers.filter(handler => handler.cheat == cheat)[0];
        handler?.handle(this._game, args);
    }
}