import { Entity } from "../entity";
import { SystemBase } from "./base";

export class DrawSystem extends SystemBase {

    onEntityAdded(e: Entity): void {
        if (e.components.position && e.components.sprite)
            this.add(e);
    }
    onEntityRemoved(e: Entity): void {
        if (e.components.position && e.components.sprite)
            this.remove(e);
     }
    update(_: number): void {
        for (let i = 0; i < this.entities.length; i++) {
            const e = this.entities[i];
            const spr = e.components.sprite;
            const pos = e.components.position;

            sprite(spr.bmp, spr.sprite, pos.x, pos.y, spr.w, spr.h, 1, spr.flip);
        }
    }
}
