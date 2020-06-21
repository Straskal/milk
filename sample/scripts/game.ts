import { IntroState } from "./intro/state";
import { CheatHandler } from "./cheatHandler";

export interface GameState {
    enter(game: Game): void;
    update(game: Game): void;
    draw(game: Game): void;
    exit(game: Game): void;
}

export class Game {

    private _ticks = 0;    
    private _stateStack: GameState[] = [];
    private _cheatHandler = new CheatHandler(this);

    public get ticks() {
        return this._ticks;
    }

    public init(): void {
        this.pushState(new IntroState());
    }

    public update(): void {
        const length = this._stateStack.length;
        for (let i = length - 1; i >= 0; i--) {
            this._stateStack[i].update(this);
        }
    }

    public draw(): void {
        for (const state of this._stateStack) {
            state.draw(this);
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