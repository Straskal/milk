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
    color: number;
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
                health: 2,
                color: 0x00
            });
        }
    }

    get liveEnemies() {
        return this._pool;
    }

    update(game: Game): void {
        for (let i = 0; i < PoolSize; i++) {
            const enemy = this._pool[i];
            enemy.color = 0x00;
            enemy.x = enemy.rotation * math.sin(game.ticks / 20) + enemy.xMove
            enemy.y = enemy.rotation * math.cos(game.ticks / 20) + ++enemy.yMove

            if (isOutOfBounds(enemy)) {
                enemy.health = 2;
                enemy.xMove = math.random(256);
                enemy.yMove = -math.random(100);
                enemy.rotation = math.random(20);
            }
        }
    }

    draw(): void {
        for (let i = 0; i < PoolSize; i++) {
            const enemy = this._pool[i];
            const scale = enemy.y / 224;
            spr(enemy.sprite, enemy.x - 8, enemy.y - 8, 1, 1, scale, 0, enemy.color);
        }
    }

    damage(enemy: Enemy, amount: number): void {
        enemy.health -= amount;
        enemy.color = 0xffffff;
        if (enemy.health <= 0) {
            enemy.health = 2;
            enemy.xMove = math.random(256);
            enemy.yMove = -math.random(100);
            enemy.rotation = math.random(20);
        }
    }
}
