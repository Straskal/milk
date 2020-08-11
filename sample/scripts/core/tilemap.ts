const max = math.max;
const min = math.min;

export interface Tilemap {
    bmp: Bitmap;
    data: number[];
    collisions: number[];
    width: number;
    height: number;
    cellWidth: number;
    cellHeight: number;
}

function clamp(val: number, lower: number, upper: number): number {
    return max(lower, min(upper, val))
}

/** @tupleReturn */
export function clampScreenToMap(tilemap: Tilemap, x: number, y: number): [number, number] {
    const xMax = tilemap.width * 16 - 384;
    const yMax = tilemap.height * 16 - 216;
    return [clamp(x, 0, xMax), clamp(y, 0, yMax)];
}

export function drawTilemap(tilemap: Tilemap, xOffset: number, yOffset: number): void {
    tiles(tilemap.bmp, tilemap.data, -xOffset, -yOffset, tilemap.cellWidth, tilemap.cellHeight, tilemap.width);
}
