import { Game } from "../game";

const PoolSize = 10;
const ExplosionAnimStartFrame = 18;
const ExplosionAnimNumFrames = 4;
const ExplosionAnimFrameTime = 4;

interface Explosion {
    x: number,
    y: number,
    frame: number,
    timer: number
}

export interface ExplosionAssetInfo {
    explosionSound: number;
}

export class Explosions {

    private readonly _pool: Explosion[] = [];
    private readonly _liveExplosions: Explosion[] = [];

    constructor(private readonly _assetInfo: ExplosionAssetInfo) {
        for (let i = 0; i < PoolSize; i++) {
            this._pool.push({
                x: 0,
                y: 0,
                frame: 0,
                timer: 0
            });
        }
    }

    public update(game: Game): void {
        const len = this._liveExplosions.length;

        for (let i = len; i > 0; i--) {
            const explosion = this._liveExplosions[i - 1];

            if (game.ticks > explosion.timer) {
                explosion.frame++;
                explosion.timer = game.ticks + ExplosionAnimFrameTime;

                if (explosion.frame > ExplosionAnimStartFrame + ExplosionAnimNumFrames) {
                    this._pool.push(explosion);
                    this._liveExplosions.splice(i - 1, 1);
                }
            }
        }
    }

    public draw(): void {
        for (const explosion of this._liveExplosions) {
            spr(explosion.frame, explosion.x - 8, explosion.y - 8);
        }
    }

    public create(x: number, y: number, ticks: number): void {
        const explosion = this._pool.pop();

        if (explosion) {
            explosion.x = x;
            explosion.y = y;
            explosion.frame = ExplosionAnimStartFrame;
            explosion.timer = ticks + ExplosionAnimFrameTime;

            this._liveExplosions.push(explosion);
            play(this._assetInfo.explosionSound, 0, 80);
        }
    }
}
