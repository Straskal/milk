import { Game } from "../game";

interface IExplosion {
    x: number,
    y: number,
    frame: number,
    timer: number,
    isActive: boolean
}

export class ExplosionPool {

    readonly ANIM_START_SPRITE = 18;
    readonly NUM_FRAMES = 4;
    readonly ANIM_TIMER = 3;
    readonly POOL_SIZE = 10;

    private _pool: IExplosion[] = [];

    constructor() {
        for (let i = 0; i < this.POOL_SIZE; i++) {
            this._pool.push({
                x: 0,
                y: 0,
                frame: 0,
                timer: 0,
                isActive: false
            });
        }
    }

    public update(game: Game): void {
        for (let i = 0; i < this.POOL_SIZE; i++) {
            const explosion = this._pool[i];
            if (explosion.isActive && game.ticks > explosion.timer) {
                explosion.frame++;
                explosion.timer = game.ticks + this.ANIM_TIMER;
                if (explosion.frame > this.ANIM_START_SPRITE + this.NUM_FRAMES)
                    explosion.isActive = false;
            }
        }
    }

    public draw(): void {
        for (let i = 0; i < this.POOL_SIZE; i++) {
            const explosion = this._pool[i];
            if (explosion.isActive)
                spr(explosion.frame, explosion.x - 8, explosion.y - 8);
        }
    }

    public create(x: number, y: number, ticks: number): void {
        for (let i = 0; i < this.POOL_SIZE; i++) {
            const explosion = this._pool[i];
            if (!explosion.isActive) {
                explosion.isActive = true;
                explosion.x = x;
                explosion.y = y;
                explosion.frame = this.ANIM_START_SPRITE;
                explosion.timer = ticks + this.ANIM_TIMER;
                break;
            }
        }
    }
}
