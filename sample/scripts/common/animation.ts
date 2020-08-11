import { Sprite } from "../sprite";

export interface Animation {
    frames: number[];
    speed: number;
}

export interface Animations {
    enabled: boolean;
    animations: Map<string, Animation>;
    current: Animation;
    currentFrame: number;
    timer: number;
}

export function setAnimation(animations: Animations, animName: string): void {
    let anim = animations.animations.get(animName);
    if (anim && anim != animations.current) {
        animations.current = anim;
        animations.currentFrame = 0;
        animations.timer = 0;
    }
}

export function animateSprites(sprites: Sprite[], ticks: number): void {
    for (let i = 0; i < sprites.length; i++) {
        const spr = sprites[i];
        const anim = spr.animations!;

        if (!anim.enabled)
            continue;

        if (ticks > anim.timer) {
            if (++anim.currentFrame > anim.current.frames.length - 1)
                anim.currentFrame = 0;

            spr.index = anim.current.frames[anim.currentFrame];
            anim.timer = ticks + anim.current.speed;
        }
    }
}
