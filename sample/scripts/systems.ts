import { Entity } from "./entity";
import { Animations, Sprite, Position } from "./components";

export interface ISystem {
    onEntityAdded(e: Entity): void;
    onEntityRemoved(e: Entity): void;
    update(ticks: number): void;
}

interface AnimationSystemEntity {
    animations: Animations;
    sprite: Sprite;
}

export class AnimationSystem implements ISystem {

    map: Map<Entity, number> = new Map();
    entities: AnimationSystemEntity[] = [];

    onEntityAdded(e: Entity): void {
        const anim = e.getComponentOfType(Animations);
        const sprite = e.getComponentOfType(Sprite);

        if (anim && sprite) {
            const i = this.entities.push({
                animations: anim,
                sprite: sprite
            });
            this.map.set(e, i);
        }
    }
    onEntityRemoved(e: Entity): void {
        const i = this.map.get(e);
        if (i) {

        }
     }
    update(ticks: number): void {
        for (let i = 0; i < this.entities.length; i++) {
            const e = this.entities[i];
            const anim = e.animations;
            const sprite = e.sprite;

            if (ticks > anim.timer) {
                anim.timer = ticks + 6;
                anim.currentFrame++;
                if (anim.currentFrame > anim.current.frames.length) {
                    anim.currentFrame = 0;
                }
                sprite.sprite = anim.current.frames[anim.currentFrame];
            }
        }
    }
}

interface DrawSystemEntity {
    position: Position;
    sprite: Sprite;
}

export class DrawSystem implements ISystem {

    map: Map<Entity, number> = new Map();
    entities: DrawSystemEntity[] = [];

    onEntityAdded(e: Entity): void {
        const pos = e.getComponentOfType(Position);
        const sprite = e.getComponentOfType(Sprite);

        if (pos && sprite) {
            const i = this.entities.push({
                position: pos,
                sprite: sprite
            });
            this.map.set(e, i);
        }
    }
    onEntityRemoved(e: Entity): void {
        const i = this.map.get(e);
        if (i) {

        }
     }
    update(ticks: number): void {
        for (let i = 0; i < this.entities.length; i++) {
            const de = this.entities[i];
            sprite(de.sprite.bmp, de.sprite.sprite, de.position.x, de.position.y, de.sprite.w, de.sprite.h);
        }
    }
}
