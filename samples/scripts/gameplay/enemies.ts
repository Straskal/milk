import { ICollidable, CollisionType } from "./collision";
import { Game } from "../game";

export interface IEnemy extends ICollidable {
    xMove: number;
    yMove: number;
    rotation: number;
    sprite: number;
    health: number;
}

export class EnemyPool {

    readonly POOL_SIZE = 6;

    private _pool: IEnemy[] = [];

    constructor() {
        for (let i = 0; i < this.POOL_SIZE; i++) {
            this._pool.push({
                type: CollisionType.ENEMY,
                mask: 0,
                x: 0,
                y: 0,
                width: 16,
                height: 16,
                xMove: math.random(256),
                yMove: -math.random(100),
                rotation: math.random(20),
                sprite: 16,
                health: 1
            });
        }
    }

    public get liveEnemies() {
        return this._pool;
    }

    public update(game: Game): void {
        for (let i = 0; i < this.POOL_SIZE; i++) {
            const enemy = this._pool[i];
            enemy.x = enemy.rotation * math.sin(game.ticks / 20) + enemy.xMove
            enemy.y = enemy.rotation * math.cos(game.ticks / 20) + ++enemy.yMove
            if (this.isOutOfBounds(enemy))
                this.destroy(enemy);
        }
    }

    public draw(): void {
        for (let i = 0; i < this.POOL_SIZE; i++) {
            const enemy = this._pool[i];
            spr(enemy.sprite, enemy.x - 8, enemy.y - 8);
        }
    }

    public destroy(enemy: IEnemy): void {
        enemy.xMove = math.random(256);
        enemy.yMove = -math.random(100);
        enemy.rotation = math.random(20);
    }

    private isOutOfBounds(enemy: IEnemy) {
        return enemy.y > 224;
    }
}