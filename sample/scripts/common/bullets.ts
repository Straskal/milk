import { ICollidable, CollisionType, isColliding } from "./collision";

const PoolSize = 10;

interface Bullet extends ICollidable {
    sprite: number
    damage: number;
    direction: number;
}

export class BulletPool {

    private _pool: Bullet[] = [];
    private _liveBullets: Bullet[] = [];

    constructor() {
        for (let i = 0; i < PoolSize; i++) {
            this._pool.push({
                type: CollisionType.BULLET,
                mask: 0,
                x: 0,
                y: 0,
                width: 16,
                height: 16,
                sprite: 0,
                damage: 1,
                direction: 0
            });
        }
    }

    public update(): void {
        let len = this._liveBullets.length;

        for (let i = len; i > 0; i--) {
            const bullet = this._liveBullets[i - 1];

            bullet.y += bullet.direction;

            if (this.isOutOfBounds(bullet)) {
                this._pool.push(bullet);
                this._liveBullets.splice(i - 1, 1);
            }
        }
    }

    public draw(): void {
        for (const bullet of this._liveBullets) {
            spr(bullet.sprite, bullet.x - 8, bullet.y - 8);
        }
    }

    public create(x: number, y: number, direction: number, sprite: number, mask: number): void {
        const bullet = this._pool.pop();

        if (bullet != undefined) {
            bullet.mask = mask;
            bullet.x = x;
            bullet.y = y;
            bullet.direction = direction;
            bullet.sprite = sprite;
            this._liveBullets.push(bullet);
        }
    }

    public checkCollision(target: ICollidable): boolean {
        let len = this._liveBullets.length;

        for (let i = len; i > 0; i--) {
            const bullet = this._liveBullets[i - 1];

            if (isColliding(bullet, target)) {
                this._pool.push(bullet);
                this._liveBullets.splice(i - 1, 1);
                return true;
            }
        }

        return false;
    }

    private isOutOfBounds(bullet: Bullet) {
        return bullet.x < 0 || bullet.x > 256 || bullet.y < 0 || bullet.y > 224;
    }
}
