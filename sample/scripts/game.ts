import { AnimationSystem, DrawSystem, PlayerSystem } from "./systems";
import { Entity, EntityFlags } from "./entity";
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

    private static _player = new PlayerSystem();
    private static _anim = new AnimationSystem();
    private static _draw = new DrawSystem();

    private static tileBmp: Bitmap;
    private static tileData = [
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    ];

    static get ticks() {
        return this._ticks;
    }

    static set ticks(value: number) {
        this._ticks = value;
    }

    static init(): void {
        this.tileBmp = bitmap("art/LOTP.bmp");

        let player = new Entity();
        player.flags |= EntityFlags.PLAYER;

        let position = new Position();
        position.x = 10;
        position.y = 10;

        let sprite = new Sprite();
        sprite.bmp = bitmap("art/peasant.bmp");
        sprite.w = 2;
        sprite.h = 3;

        let animations = new Animations();
        let idle = {
            frames: [0],
            speed: 0
        };
        let walk = {
            frames: [2, 4],
            speed: 12
        };
        animations.animations.set("idle", idle);
        animations.animations.set("walk", walk);
        animations.current = idle;

        player.components.push(position);
        player.components.push(sprite);
        player.components.push(animations);

        this._player.onEntityAdded(player);
        this._anim.onEntityAdded(player);
        this._draw.onEntityAdded(player);
    }

    static update(): void {
        this._player.update(this.ticks);
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
        tiles(this.tileBmp, this.tileData, 0, 0, 2, 2, 13);
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
