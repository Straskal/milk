export interface GameState {
    updateBelow: boolean;
    drawBelow: boolean;

    enter(): void;
    update(): void;
    draw(): void;
    exit(): void;
}

/**
 * Main game class. All game states are pushed to and popped from the game's state stack.
 */
export class Game {

    private static _ticks = 0;
    private static _stateStack: GameState[] = [];

    static get ticks() {
        return this._ticks;
    }

    static set ticks(value: number) {
        this._ticks = value;
    }

    static init(initialState: GameState): void {
        this.pushState(initialState);
    }

    static update(): void {
        for (let i = this._stateStack.length - 1; i >= 0; i--) {
            const state = this._stateStack[i];
            state.update();
            if (!state.updateBelow)
                break;
        }
    }

    static draw(): void {
        clrs();
        const length = this._stateStack.length;
        for (let i = 0; i < length; i++) {
            if (i < length - 1 && !this._stateStack[i + 1].drawBelow)
                continue;

            this._stateStack[i].draw();
        }
        this._ticks++;
    }

    static peek(): GameState {
        return this._stateStack[this._stateStack.length - 1];
    }

    static pushState(state: GameState): void {
        state.enter();
        this._stateStack.push(state);
    }

    static popState(): void {
        const top = this._stateStack.pop();
        top?.exit();
    }
}
