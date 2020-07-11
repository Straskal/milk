/** @noSelfInFile */

declare let _init: () => void;
declare let _update: () => void;
declare let _draw: () => void;

declare function btn(button: number): boolean;
declare function btnp(button: number): boolean;
declare function loadspr(filePath: string): void;
declare function loadfont(filePath: string): void;
declare function clip(x: number, y: number, width: number, height: number): void;
declare function clrs(color?: number): void;
declare function pset(x: number, y: number, color: number): void;
declare function line(x0: number, y0: number, x1: number, y1: number, color: number): void;
declare function rect(x: number, y: number, width: number, height: number, color: number): void;
declare function rectfill(x: number, y: number, width: number, height: number, color: number): void;
declare function spr(spriteId: number, x: number, y: number, width?: number, height?: number, scale?: number, flip?: 0 | 1 | 2 | 3): void;
declare function sprfont(x: number, y: number, text: string, scale?: number, color?: number): void;
declare function loadsnd(soundId: number, fileName: string): void;
declare function freesnd(soundId: number): void;
declare function play(soundId: number, slotId: number, volume: number): void;
declare function stop(slotId: number): void;
declare function pause(slotId: number): void;
declare function resume(slotId: number): void;
declare function openstream(streamId: number, filePath: string): void;
declare function playstream(streamId: number, volume: number, loop?: boolean): void;
declare function stopstream(): void;
declare function pausestream(): void;
declare function resumestream(): void;
declare function closestream(streamId: number): void;
declare function exit(): void;
