import { GameState, Game } from "../game";
import { MainMenuState } from "../mainMenu/state";

const NUMTICKS = 200;
const min = math.min;

export class SplashState implements GameState {

    private _timer = 0;
    private _r = 0;
    private _g = 0;
    private _b = 0;

    updateBelow = false;
    drawBelow = false;

    enter(game: Game): void {
        loadsnd(0, "sounds/splash.wav");
        play(0, 0, 128);
        this._timer = game.ticks + NUMTICKS;
    }
    update(game: Game): void {
        if (true) {
            this._r = min(this._r + 4, 255);
            this._g = min(this._g + 4, 255);
            this._b = min(this._b + 4, 255);
        }

        if (game.ticks > this._timer && this._r == 255) {
            game.popState();
            game.pushState(new MainMenuState());
        }
    }
    draw(_: Game): void {
        clrs();
        blend(0);
        const col = (this._r << 16) | (this._g << 8) | this._b;
        font(0, 10, 90, "it'sgood! games", 2, col);
        blend(1);
    }
    exit(_: Game): void {
        freesnd(0);
    }
}
