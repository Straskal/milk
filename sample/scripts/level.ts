import { newWorld } from "./libs/bump";
import { Sprite } from "./sprite";
import { Game, GameState } from "./game";

const drawSprite = sprite;
const max = math.max;
const min = math.min;

function clamp(val: number, lower: number, upper: number): number {
    if (lower > upper) {
        lower = upper;
        upper = lower;
    }
    return max(lower, min(upper, val))
}

export interface Tilemap {
    bmp: Bitmap;
    data: number[];
    collisions: number[];
    width: number;
    cellWidth: number;
    cellHeight: number;
}

export class Level implements GameState {

    static current: Level;

    updateBelow = false;
    drawBelow = false;

    x = 0;
    y = 0;

    bumpWorld = newWorld(16);
    map!: Tilemap;
    sprites: Sprite[] = [];
    updated: Sprite[] = [];
    animated: Sprite[] = [];

    enter(): void {
        Level.current = this;
        const collisions = this.map.collisions;
        const columns = this.map.width;
        if (collisions) {
            for (let i = 0; i < collisions.length; i++) {
                if (collisions[i] == 1) {
                    const row = i / columns;
                    const col = i % columns;
                    this.bumpWorld.add({}, col * 16, row * 16, 16, 16);
                }
            }
        }
    }

    exit(): void {

    }

    add(sprite: Sprite): void {
        this.sprites.push(sprite);
        if (sprite.updateHandlers)
            this.updated.push(sprite);
        if (sprite.animations)
            this.animated.push(sprite);
        if (sprite.body)
            this.bumpWorld.add(sprite, sprite.x, sprite.y, sprite.body.width, sprite.body.height);
    }

    moveAndCollide(sprite: Sprite, x: number, y: number): void {
        const [actualX, actualY] = this.bumpWorld.move(sprite, sprite.x + x, sprite.y + y);
        sprite.x = actualX;
        sprite.y = actualY;
    }

    update(): void {
        // Update handlers
        for (let i = 0; i < this.updated.length; i++) {
            const sprite = this.updated[i];
            const handlers = sprite.updateHandlers!;

            for (let c = 0; c < handlers.length; c++) {
                handlers[i].update(sprite);
            }
        }

        // Animations
        for (let i = 0; i < this.animated.length; i++) {
            const spr = this.animated[i];
            const anim = spr.animations!;

            if (!anim.enabled)
                continue;

            if (Game.ticks > anim.timer) {
                if (++anim.currentFrame > anim.current.frames.length - 1)
                    anim.currentFrame = 0;

                spr.index = anim.current.frames[anim.currentFrame];
                anim.timer = Game.ticks + anim.current.speed;
            }
        }
    }

    draw(): void {
        const map = this.map;
        const max = this.map.width * 16 - 320;
        this.x = clamp(this.x, 0, max);
        this.y = clamp(this.y, 0, 0);
        tiles(map.bmp, map.data, -this.x, -this.y, map.cellWidth, map.cellHeight, map.width);
        for (let i = 0; i < this.sprites.length; i++) {
            const spr = this.sprites[i];
            drawSprite(spr.bmp!, spr.index, spr.x - this.x, spr.y - this.y, spr.width, spr.height, 1, spr.flip);
        }
    }
}
