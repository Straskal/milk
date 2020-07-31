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
    private _bitmap!: Bitmap;
    private _music!: Stream;
    private _xPos: number = 0;
    private _tiles = [
        40, 40, 40, 40, 40,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 40, 40, 40, 40, 40,
        40, 40, 40, 40, 40,  0,  0,  6,  0,  0,  0,  0,  6,  0,  0, 40, 40, 40, 40, 40,
        40, 40, 40, 40, 40,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 40, 40, 40, 40, 40,
        40, 40, 40, 40, 40,  0,  0, 08,  0,  0,  0,  0,  6,  0,  0, 40, 40, 40, 40, 40,
        40, 40, 40, 40, 40,  0,  0,  0,  0,  0,  2,  0,  0,  0,  0, 40, 40, 40, 40, 40,
        40, 40, 40, 40, 40, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 40, 40, 40, 40, 40,
        40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
        40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
        40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
        40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
        40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
        40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
        40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
        40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
        40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
    ];

    public get ticks() {
        return this._ticks;
    }

    public set ticks(value: number) {
        this._ticks = value;
    }

    public init(): void {
        this._bitmap = bitmap("art/town_tiles.bmp");
        this._music = stream("sounds/02 Underclocked (underunderclocked mix).wav");
        playstream(this._music, 128, true);
    }

    public update(): void {
        for (let i = this._stateStack.length - 1; i >= 0; i--) {
            const state = this._stateStack[i];
            state.update(this);
            if (!state.updateBelow)
                break;
        }

        if (btn(3))
            this._xPos -= 1;
        if (btn(4))
            this._xPos += 1;
    }

    public draw(): void {
        clrs();
        tiles(this._bitmap, this._tiles, this._xPos, 0, 2, 2, 20);
        this._ticks++;
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
