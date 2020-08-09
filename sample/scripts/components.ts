export class Position {
    x = 0;
    y = 0;
}

export class Sprite {
    bmp!: Bitmap;
    sprite = 0;
    w = 0;
    h = 0;
}

export interface Animation {
    frames: number[];
    speed: number;
}

export class Animations {
    animations: Map<string, Animation> = new Map();
    current!: Animation;
    currentFrame = 0;
    timer = 0;
}

export function setAnimation(animations: Animations, animName: string): void {
    let anim = animations.animations.get(animName);
    if (anim && anim != animations.current) {
        animations.current = anim;
        animations.currentFrame = 0;
        animations.timer = 0;
    }
}
