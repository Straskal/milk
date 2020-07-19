import { ICollidable, CollisionType } from "./collision";
import { Game } from "../game";

const PoolSize = 6;
const EnemySprite = 16;

export interface Enemy extends ICollidable {
    xMove: number;
    yMove: number;
    rotation: number;
    sprite: number;
    health: number;
}

function isOutOfBounds(enemy: Enemy) {
    return enemy.y > 224;
}

export class Enemies {

    private readonly _pool: Enemy[] = [];

    constructor() {
        for (let i = 0; i < PoolSize; i++) {
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
                sprite: EnemySprite,
                health: 1
            });
        }
    }

    get liveEnemies() {
        return this._pool;
    }

    update(game: Game): void {
        for (let i = 0; i < PoolSize; i++) {
            const enemy = this._pool[i];
            enemy.x = enemy.rotation * math.sin(game.ticks / 20) + enemy.xMove
            enemy.y = enemy.rotation * math.cos(game.ticks / 20) + ++enemy.yMove

            if (isOutOfBounds(enemy))
                this.destroy(enemy);
        }
    }

    draw(): void {
        for (let i = 0; i < PoolSize; i++) {
            const enemy = this._pool[i];
            spr(enemy.sprite, enemy.x - 8, enemy.y - 8);
        }
    }

    destroy(enemy: Enemy): void {
        enemy.xMove = math.random(256);
        enemy.yMove = -math.random(100);
        enemy.rotation = math.random(20);
    }
}
