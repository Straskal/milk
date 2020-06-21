import { GameState } from "./gameState";
import { IntroState } from "./intro/state";
import { GameplayState } from "./gameplay/state";

export class Game {

    private _ticks = 0;    
    private _stateStack: GameState[] = [];

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
        if (command == "addscore" && args.length == 1) {
            const top = this._stateStack[this._stateStack.length - 1];
            if (top instanceof GameplayState) {
                const topAsGameplay = <GameplayState>top;
                topAsGameplay.addToScore(<number>tonumber(args[0]));
            }
        }
        else if (command == "boi") {
            this.popState();
            this.pushState(new GameplayState());
        }
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