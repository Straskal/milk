import { Game, GameState } from "../game";
import { ActionList, Action } from "./actionList";

export class CinematicState<Cinema> implements GameState {

    private readonly _context: Cinema;
    private readonly _actionList: ActionList<Cinema>;
    private readonly _nextState: GameState;

    public updateBelow = false;
    public drawBelow = false;

    constructor(context: Cinema, actions: Action<Cinema>[], nextState: GameState) {
        this._context = context;
        this._actionList = new ActionList(actions);
        this._nextState = nextState;
    }

    update(game: Game): void {
        if (this._actionList.update(this._context)) {
            game.popState();
            game.pushState(this._nextState);
        }
    }

    draw(_: Game): void {
        this._actionList.draw(this._context);
    }

    enter(_: Game): void { }
    exit(_: Game): void { }
}
