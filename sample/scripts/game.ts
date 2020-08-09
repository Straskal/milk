import { AnimationSystem, DrawSystem } from "./systems";
import { Entity } from "./entity";
import { Position, Animations, Sprite } from "./components";

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

    private static _ticks = 0;
    private static _stateStack: GameState[] = [];

    private static _anim = new AnimationSystem();
    private static _draw = new DrawSystem();

    static get ticks() {
        return this._ticks;
    }

    static set ticks(value: number) {
        this._ticks = value;
    }

    static init(): void {
        let e = new Entity();
        let p = new Position();
        p.x = 10;
        p.y = 10;
        let s = new Sprite();
        s.bmp = bitmap("art/omrs.bmp");
        s.w = 4;
        s.h = 4;
        let a = new Animations();
        let idle = {
            frames: [0, 4, 8, 12, 64, 68 ],
            name: "idle"
        };
        a.animations.set("idle", idle);
        a.current = idle;

        e.components.push(p);
        e.components.push(s);
        e.components.push(a);

        this._anim.onEntityAdded(e);
        this._draw.onEntityAdded(e);
    }

    static update(): void {
        this._anim.update(this.ticks);

        for (let i = this._stateStack.length - 1; i >= 0; i--) {
            const state = this._stateStack[i];
            state.update(this);
            if (!state.updateBelow)
                break;
        }
    }

    static draw(): void {
        clrs();
        this._draw.update(this._ticks);
        this._ticks++;
    }

    static peek(): GameState {
        return this._stateStack[this._stateStack.length - 1];
    }

    static pushState(state: GameState): void {
        state.enter(this);
        this._stateStack.push(state);
    }

    static popState(): void {
        const top = this._stateStack.pop();
        top?.exit(this);
    }
}
