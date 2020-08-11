export interface Sprite {
    x: number
    y: number
    bmp: Bitmap;
    index: number
    width: number
    height: number
    flip: number

    updateHandlers?: UpdateHandler[];
    animations?: Animations;
    body?: Body;
}

export interface UpdateHandler {
    update(sprite: Sprite): void;
}

export interface Body {
    width: number;
    height: number;
}

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
