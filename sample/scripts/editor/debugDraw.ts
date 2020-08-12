import { GameState, Game } from "../game";
import { Level } from "../core/level";
import { Sprite } from "../core/sprite";

export class DebugDraw implements GameState {

    updateBelow = true;
    drawBelow = true;

    level: Level | null = null;

    enter(): void {
        this.level = Game.getState<Level>(Level);
    }

    update(): void {

    }

    draw(): void {
        const [items, len] = this.level!.bumpWorld.getItems();
        const drawRect = rect;

        for (let i = 0; i < len; i++) {
            const spr = <Sprite>items[i];
            const body = spr.body;
            if (body)
                drawRect(spr.x - this.level!.x, spr.y - this.level!.y, body.width, body.height, 0xffff0000);
        }
    }

    exit(): void {

    }

}
