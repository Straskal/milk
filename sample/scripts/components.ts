type Component<T> = T | null;

export interface Components {
    position: Position;
    sprite: Sprite;
    animations: Animations;
}

export interface Position {
    x: number;
    y: number
}

export interface Sprite {
    bmp: Bitmap;
    sprite: number;
    w: number
    h: number
    flip: number
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
