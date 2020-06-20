/** @noSelfInFile */

declare let _init: () => void;
declare let _update: () => void;
declare let _draw: () => void;

declare function btn(button: number): boolean;
declare function btnp(button: number): boolean;
declare function loadspr(filePath: string): void;
declare function loadfont(filePath: string): void;
declare function clip(x: number, y: number, width: number, height: number): void;
declare function clrs(color: number): void;
declare function pset(x: number, y: number, color: number): void;
declare function line(x0: number, y0: number, x1: number, y1: number, color: number): void;
declare function rect(x: number, y: number, width: number, height: number, color: number): void;
declare function rectfill(x: number, y: number, width: number, height: number, color: number): void;
declare function spr(index: number, x: number, y: number, width?: number, height?: number, scale?: number, flip?: number): void;
declare function sprfont(x: number, y: number, text: string, scale?: number, color?: number): void;
declare function loadsnd(index: number, fileName: string): void;
declare function play(index: number, sampleSlot: number, volume: number): void;
