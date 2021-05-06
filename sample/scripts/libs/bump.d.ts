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

export type FilterFunction = (item: any, other: any) => false | "touch" | "cross" | "slide" | "bounce";

interface BumpWorld {

    /*************************************************
     * BASIC API
     ************************************************/

    add(item: any, x: number, y: number, w: number, h: number): void;

    remove(item: any): void;

    update(item: any, x: number, y: number, w?: number, h?: number): void;

    /** @tupleReturn */
    move(item: any, goalX: number, goalY: number, filter?: FilterFunction): [number, number, BumpCollision[], number];

    /** @tupleReturn */
    check(item: any, goalX: number, goalY: number, filter?: FilterFunction): [number, number, BumpCollision[], number];

    /*************************************************
     * INTERMEDIATE API
     ************************************************/

    /** @tupleReturn */
    queryPoint(x: number, y: number, filter?: FilterFunction): [any[], number];

    /** @tupleReturn */
    queryRect(x: number, y: number, w: number, h: number, filter?: FilterFunction): [any[], number];

    /** @tupleReturn */
    querySegment(x1: number, y1: number, x2: number, y2: number, filter?: FilterFunction): [any[], number];

    /*************************************************
     * ADVANCED API
     ************************************************/

    /** @tupleReturn */
    getItems(): [any[], number];
}

export function newWorld(cellSize: number): BumpWorld;
