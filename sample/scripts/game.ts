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
    private _omrs!: Bitmap;
    private _omrsFrame = 0;
    private _omrsTimer = 0;
    private _omrsFrames = [
        0, 4, 8, 12, 64, 68
    ];
    private _xPos: number = 0;
    private _tiles = [
        0, 0, 8, 0, 0, 0, 0, 0, 0, 2, 0,
        0, 0, 8, 0, 0, 0, 10, 0, 0, 2, 0,
        0, 0, 8, 10, 0, 0, 0, 0, 0, 0, 0,
        8, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 8, 0, 0, 0, 10, 0, 0, 0, 0,
        0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0
    ];

    public get ticks() {
        return this._ticks;
    }

    public set ticks(value: number) {
        this._ticks = value;
    }

    public init(): void {
        this._omrs = bitmap("art/omrs.bmp");
        this._bitmap = bitmap("art/LOTP.bmp");
        this._music = stream("sounds/02 Underclocked (underunderclocked mix).wav");
        //playstream(this._music, 128, true);
    }

    public update(): void {
        for (let i = this._stateStack.length - 1; i >= 0; i--) {
            const state = this._stateStack[i];
            state.update(this);
            if (!state.updateBelow)
                break;
        }

        if (this.ticks > this._omrsTimer) {
            if (++this._omrsFrame > 5)
                this._omrsFrame = 0;
            this._omrsTimer += 5;
        }

        if (btn(3))
            this._xPos -= 1;
        if (btn(4))
            this._xPos += 1;
    }

    public draw(): void {
        clrs();
        tiles(this._bitmap, this._tiles, this._xPos, 0, 2, 2, 11);
        sprite(this._omrs, this._omrsFrames[this._omrsFrame], 100, 60, 4, 4);
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
