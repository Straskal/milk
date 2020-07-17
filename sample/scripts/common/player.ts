import { Game } from "../game";
import { GameplayState } from "../gameplay/state";
import { Bullets } from "./bullets";
import { CollisionType, ICollidable } from "./collision";

const AttackTimeDelay = 8;
const AnimationFrame_1 = 0;
const AnimationFrame_2 = 1;

export interface PlayerAssetInfo {
    shootSound: number;
}

export class Player implements ICollidable {

    x = 256 / 2 - 8;
    y = 224 - 64;
    type = CollisionType.PLAYER;
    mask = 0;
    width = 16;
    height = 16;
    speed = 1;
    sprite = AnimationFrame_1;
    bulletTimer = 0;

    constructor(private readonly _assetInfo: PlayerAssetInfo) {}

    update(game: Game, gameplay: GameplayState): void {
        this.move();
        this.attack(game.ticks, gameplay.bulletPool);
        this.animate(game.ticks);
    }

    draw(): void {
        spr(this.sprite, this.x - 8, this.y - 8);
    }

    private move(): void {
        let xVelocity = 0;
        let yVelocity = 0;

        if (btn(1))
            yVelocity = -this.speed;
        if (btn(2))
            yVelocity = this.speed;
        if (btn(3))
            xVelocity = -this.speed;
        if (btn(4))
            xVelocity = this.speed;

        this.x += xVelocity;
        this.y += yVelocity;
    }

    private animate(ticks: number): void {
        this.sprite = ticks % 12 < 6 ? AnimationFrame_1 : AnimationFrame_2;
    }

    private attack(ticks: number, bulletPool: Bullets): void {
        if (btn(5) && ticks > this.bulletTimer) {
            bulletPool.create(this.x, this.y, -3, 2, CollisionType.ENEMY);
            this.bulletTimer = ticks + AttackTimeDelay;

            play(this._assetInfo.shootSound, 1, 50);
        }
    }
}
