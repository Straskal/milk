import { GameState, Game } from "../game";
import { MainMenuState } from "../mainMenu/state";
import { ActionList, Action } from "../common/actionList";

const NUMTICKS = 200;
const min = math.min;
const max = math.max;

const SplashText = "it'sgood! games";
const SplashTextXPosition = 10;
const SplashTextYPosition = 90;

class FadeIn implements Action<Game> {

    private _r = 0;
    private _g = 0;
    private _b = 0;

    enter(t: Game): void {
        play(0, 0, 128);
    }
    update(t: Game): boolean {
        this._r = min(this._r + 4, 255);
        this._g = min(this._g + 4, 255);
        this._b = min(this._b + 4, 255);

        return this._r == 255;
    }
    draw(t: Game): void {
        clrs();
        font(0, SplashTextXPosition, SplashTextYPosition, SplashText, 2, (this._r << 16) | (this._g << 8) | this._b);
    }
    exit(t: Game): void {}
}

class FadeOut implements Action<Game> {

    private _r = 255;
    private _g = 255;
    private _b = 255;

    enter(t: Game): void {}
    update(t: Game): boolean {
        this._r = max(this._r - 4, 0);
        this._g = max(this._g - 4, 0);
        this._b = max(this._b - 4, 0);

        return this._r == 0;
    }
    draw(t: Game): void {
        clrs();
        font(0, SplashTextXPosition, SplashTextYPosition, SplashText, 2, (this._r << 16) | (this._g << 8) | this._b);
    }
    exit(t: Game): void {}
}

export class SplashState implements GameState {

    private _actionList: ActionList<Game>;

    updateBelow = false;
    drawBelow = false;

    constructor() {
        this._actionList = new ActionList<Game>([
            new FadeIn(),
            new FadeOut()
        ]);
    }

    enter(_: Game): void {
        loadsnd(0, "sounds/splash.wav");
    }
    update(game: Game): void {
        if (this._actionList.update(game)) {
            game.popState();
            game.pushState(new MainMenuState());
        }
    }
    draw(game: Game): void {
        this._actionList.draw(game);
    }
    exit(_: Game): void {
        freesnd(0);
    }
}
