import { ICollidable, CollisionType, isColliding } from "./collision";

interface Bullet extends ICollidable {
    sprite: number
    damage: number;
    direction: number;
    isActive: boolean;
}

export class BulletPool {

    readonly POOL_SIZE = 10;

    private _pool: Bullet[] = [];

    constructor() {
        for (let i = 0; i < this.POOL_SIZE; i++) {
            this._pool.push({
                type: CollisionType.BULLET,
                mask: 0,
                x: 0,
                y: 0,
                width: 16,
                height: 16,
                sprite: 0,
                damage: 1,
                direction: 0,
                isActive: false
            });
        }
    }

    public update(): void {
        for (let i = 0; i < this.POOL_SIZE; i++) {
            const bullet = this._pool[i];
            if (!bullet.isActive)
                continue;
            bullet.y += bullet.direction
            if (this.isOutOfBounds(this._pool[i]))
                bullet.isActive = false;
        }
    }

    public draw(): void {
        for (let i = 0; i < this.POOL_SIZE; i++) {
            const bullet = this._pool[i];
            if (bullet.isActive)
                spr(bullet.sprite, bullet.x - 8, bullet.y - 8);
        }
    }

    public create(x: number, y: number, direction: number, sprite: number, mask: number): void {
        for (let i = 0; i < this.POOL_SIZE; i++) {
            const bullet = this._pool[i];
            if (bullet.isActive)
                continue;
            bullet.mask = mask;
            bullet.x = x;
            bullet.y = y;
            bullet.direction = direction;
            bullet.sprite = sprite;
            bullet.isActive = true;
            break;
        }
    }

    public checkCollision(target: ICollidable): boolean {
        for (let i = 0; i < this.POOL_SIZE; i++) {
            if (this._pool[i].isActive && isColliding(this._pool[i], target)) {
                this._pool[i].isActive = false;
                return true;
            }
        }
        return false;
    }

    private isOutOfBounds(bullet: Bullet) {
        return bullet.x < 0 || bullet.x > 256 || bullet.y < 0 || bullet.y > 224;
    }
}