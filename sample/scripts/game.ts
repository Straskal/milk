import { IntroState } from "./intro/state";
import { CheatHandler } from "./cheatHandler";

export interface GameState {
    updateBelow: boolean;
    drawBelow: boolean;

    enter(game: Game): void;
    update(game: Game): void;
    draw(game: Game): void;
    exit(game: Game): void;
}

/**
 * Main game class. All game states are pushed to and popped from the game's state stack.
 */
export class Game {

    private _ticks = 0;
    private _stateStack: GameState[] = [];
    private _cheatHandler = new CheatHandler(this);

    public get ticks() {
        return this._ticks;
    }

    public set ticks(value: number) {
        this._ticks = value;
    }

    public init(): void {
        this.pushState(new IntroState());
    }

    public update(): void {
        for (let i = this._stateStack.length - 1; i >= 0; i--) {
            const state = this._stateStack[i];
            state.update(this);

            if (!state.updateBelow)
                break;
        }
    }

    public draw(): void {
        const length = this._stateStack.length;
        for (let i = 0; i < length; i++) {
            if (i < length - 1 && !this._stateStack[i + 1].drawBelow)
                continue;

            this._stateStack[i].draw(this);
        }

        this._ticks++;
    }

    public cheat(command: string, args: string[]): void {
        this._cheatHandler.handleCheat(command, args);
    }

    public peek(): GameState {
        return this._stateStack[this._stateStack.length - 1];
    }

    public pushState(state: GameState): void {
        state.enter(this);
        this._stateStack.push(state);
    }

    public popState(): void {
        const top = this._stateStack.pop();
        top?.exit(this);
    }
}