import { GameState, Game } from "../game";
import { DrawSystem } from "../systems/render";
import { AnimationSystem } from "../systems/animation";
import { PlayerSystem } from "../systems/player";
import { Entity, EntityFlags } from "../entity";

export class GameplayState implements GameState {

    updateBelow = false;
    drawBelow = false;

    playerSystem = new PlayerSystem();
    animationSystem = new AnimationSystem();
    renderSystem = new DrawSystem();

    enter(): void {
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

        this.playerSystem.onEntityAdded(player);
        this.animationSystem.onEntityAdded(player);
        this.renderSystem.onEntityAdded(player);
    }

    update(): void {
        this.playerSystem.update(Game.ticks);
        this.animationSystem.update(Game.ticks);
    }

    draw(): void {
        this.renderSystem.update(Game.ticks);
    }

    exit(): void {

    }
}
