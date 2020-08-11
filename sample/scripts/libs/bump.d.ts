interface BumpVector {
    x: number;
    y: number;
}

interface BumpRect {
    x: number;
    y: number;
    w: number;
    h: number;
}

interface BumpCollision {
    item: any;
    other: any;
    type: string;
    overlaps: boolean;
    ti: number;
    move: BumpVector;
    normal: BumpVector;
    touch: BumpVector;
    itemRect: BumpRect;
    otherRect: BumpRect;
}

interface BumpWorld {

    add(item: any, x: number, y: number, w: number, h: number): void;

    remove(item: any): void;

    update(item: any, x: number, y: number, w?: number, h?: number): void;

    /** @tupleReturn */
    move(item: any,
        goalX: number,
        goalY: number,
        filter?: (item: any, other: any) => false | "touch" | "cross" | "slide" | "bounce"): [number, number, BumpCollision[], number];

        /** @tupleReturn */
    check(item: any,
        goalX: number,
        goalY: number,
        filter?: (item: any, other: any) => false | "touch" | "cross" | "slide" | "bounce"): [number, number, BumpCollision[], number];
}

export function newWorld(cellSize: number): BumpWorld;
