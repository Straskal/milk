import { Entity, EntityFlags } from "./entity";
import { PlayerSystem } from "./systems/player";
import { AnimationSystem } from "./systems/animation";
import { DrawSystem } from "./systems/render";

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

    private static _player: PlayerSystem;
    private static _anim: AnimationSystem;
    private static _draw: DrawSystem;

    static get ticks() {
        return this._ticks;
    }

    static set ticks(value: number) {
        this._ticks = value;
    }

    static init(): void {
        this._player = new PlayerSystem();
        this._anim = new AnimationSystem();
        this._draw = new DrawSystem();

        let player = new Entity();
        player.flags |= EntityFlags.PLAYER;
        player.components.position = {
            x: 10,
            y: 10
        };
        player.components.collision = {
            w: 16,
            h: 16
        };
        player.components.sprite = {
            bmp: bitmap("art/peasant.bmp"),
            sprite: 0,
            w: 2,
            h: 3,
            flip: 0
        };
        let walkUp = {
            frames: [8, 10],
            speed: 12
        };
        let walkDown = {
            frames: [2, 4],
            speed: 12
        };
        let walkRight = {
            frames: [12, 14],
            speed: 12
        };
        player.components.animations = {
            enabled: true,
            animations: new Map([
                ["walkUp", walkUp],
                ["walkDown", walkDown],
                ["walkRight", walkRight]
            ]),
            current: walkDown,
            currentFrame: 0,
            timer: 0
        };

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
