/** @noSelfInFile */

declare let _init: () => void;
declare let _update: () => void;
declare let _draw: () => void;

interface Bitmap {}
interface Wave {}
interface Stream {}

declare function btn(button: number): boolean;
declare function btnp(button: number): boolean;
declare function clip(x: number, y: number, width: number, height: number): void;
declare function clrs(color?: number): void;
declare function pset(x: number, y: number, color: number): void;
declare function line(x0: number, y0: number, x1: number, y1: number, color: number): void;
declare function rect(x: number, y: number, width: number, height: number, color: number): void;
declare function rectfill(x: number, y: number, width: number, height: number, color: number): void;
declare function bitmap(filePath: string): Bitmap;
declare function sprite(bmp: Bitmap, spriteId: number, x: number, y: number, width?: number, height?: number, scale?: number, flip?: 0 | 1 | 2 | 3, color?: number, mode?: 0 | 1 | 2): void;
declare function tiles(bmp: Bitmap, data: number[], x: number, y: number, width: number, height: number, pitch: number): void;
declare function font(bmp: Bitmap, x: number, y: number, text: string, scale?: number, color?: number): void;
declare function wave(filePath: string): Wave;
declare function play(wave: Wave, slotId: number, volume: number): void;
declare function stop(slotId: number): void;
declare function pause(slotId: number): void;
declare function resume(slotId: number): void;
declare function stream(filePath: string): Stream;
declare function playstream(stream: Stream, volume: number, loop?: boolean): void;
declare function stopstream(): void;
declare function pausestream(): void;
declare function resumestream(): void;
declare function exit(): void;
