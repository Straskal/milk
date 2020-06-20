import { Game } from "../game";
import { GameplayState } from "./state";
import { BulletPool } from "./bullets";
import { CollisionType, ICollidable } from "./collision";

export class Player implements ICollidable {

    readonly ATTACK_BUFFER = 8;

    public x = 256 / 2 - 8;
    public y = 224 - 64;
    public type = CollisionType.PLAYER;
    public mask = 0;
    public width = 16;
    public height = 16;

    private speed = 1;
    private sprite = 0;
    private bulletTimer = 0;
    private stunnedTimer = 0;
    private hearts = 3;

    public update(game: Game, gameplay: GameplayState): void {
        this.move();
        this.attack(game.ticks, gameplay.bulletPool);
        this.animate(game.ticks);
    }

    public draw(): void {
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
        this.sprite = ticks % 12 < 6 ? 0 : 1;
    }

    private attack(ticks: number, bulletPool: BulletPool): void {
        if (btn(5) && ticks > this.bulletTimer) {
            play(1, 1, 50);
            bulletPool.create(this.x, this.y, -3, 2, CollisionType.ENEMY);
            this.bulletTimer = ticks + this.ATTACK_BUFFER;
        }
    }
}
