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
    name: string;
    frames: number[];
}

export class Animations {
    animations: Map<string, Animation> = new Map();
    current!: Animation;
    currentFrame = 0;
    timer = 0;
}
