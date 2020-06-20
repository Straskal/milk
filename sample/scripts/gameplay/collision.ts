export enum CollisionType {
    PLAYER  = 1 << 0,
    ENEMY   = 1 << 1,
    BULLET  = 1 << 2
}

export interface ICollidable {
    type: CollisionType;
    mask: CollisionType;
    x: number;
    y: number;
    width: number;
    height: number;
}

function canCollide(c0: ICollidable, c1: ICollidable): boolean {
    return (c0.type & c1.mask) == c1.mask || (c1.type & c0.mask) == c0.mask;
}

export function isColliding(c0: ICollidable, c1: ICollidable): boolean {
    const right0 = c0.x + c0.width;
    const bottom0 = c0.y + c0.height;
    const right1 = c1.x + c1.width;
    const bottom1 = c1.y + c1.height;

    return canCollide(c0, c1) && c0.x < right1 && right0 > c1.x && c0.y < bottom1 && bottom0 > c1.y;
}