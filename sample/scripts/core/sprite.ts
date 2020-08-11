import { Animations } from "../common/animation";

const drawSprite = sprite;

export abstract class Behavior {
    update(_: Sprite): void {}
}

export interface Body {
    width: number;
    height: number;
}

export interface Sprite {
    x: number
    y: number
    bmp: Bitmap;
    index: number
    width: number
    height: number
    flip: number
    behaviors?: Behavior[];
    animations?: Animations;
    body?: Body;
}

export function updateSpriteComponents(sprites: Sprite[]) {
    for (let i = 0; i < sprites.length; i++) {
        const sprite = sprites[i];
        const handlers = sprite.behaviors!;
        for (let c = 0; c < handlers.length; c++) {
            handlers[i].update(sprite);
        }
    }
}

export function drawSprites(sprites: Sprite[], xOffset: number, yOffset: number): void {
    for (let i = 0; i < sprites.length; i++) {
        const spr = sprites[i];
        drawSprite(spr.bmp!, spr.index, spr.x - xOffset, spr.y - yOffset, spr.width, spr.height, 1, spr.flip);
    }
}
