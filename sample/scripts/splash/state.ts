import { GameState, Game } from "../game";
import { MainMenuState } from "../mainMenu/state";

const NUMTICKS = 200;

export class SplashState implements GameState {

    private _timer = 0;

    updateBelow = false;
    drawBelow = false;

    enter(game: Game): void {
        this._timer = game.ticks + NUMTICKS;
    }
    update(game: Game): void {
        if (game.ticks > this._timer) {
            game.popState();
            game.pushState(new MainMenuState());
        }
    }
    draw(_: Game): void {
        clrs();
        font(0, 10, 10, "it'sgood! games", 2);
    }
    exit(_: Game): void {
    }
}
