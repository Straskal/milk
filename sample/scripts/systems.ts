import { Entity, EntityFlags } from "./entity";
import { Animations, Sprite, Position, setAnimation } from "./components";

export interface ISystem {
    onEntityAdded(e: Entity): void;
    onEntityRemoved(e: Entity): void;
    update(ticks: number): void;
}

interface PlayerSystemEntity {
    position: Position;
    sprite: Sprite;
    animations: Animations;
}

export class PlayerSystem implements ISystem {

    entity: Entity | undefined;
    player: PlayerSystemEntity | undefined;

    onEntityAdded(e: Entity): void {
        if ((e.flags & EntityFlags.PLAYER) == EntityFlags.PLAYER) {
            this.player = {
                position: <Position>e.getComponentOfType(Position),
                sprite: <Sprite>e.getComponentOfType(Sprite),
                animations: <Animations>e.getComponentOfType(Animations)
            };
            this.entity = e;
        }
    }
    onEntityRemoved(e: Entity): void {
    }
    update(_: number): void {
        let mvx = 0;
        let mvy = 0;

        if (!this.player)
            return;

        if (btn(1)) {
            mvy -= 1;
        }
        if (btn(2)) {
            mvy += 1;
        }
        if (btn(3)) {
            mvx -= 1;
            this.player.sprite.flip = 1;
        }
        if (btn(4)) {
            mvx += 1;
            this.player.sprite.flip = 0;
        }

        this.player.position.x += mvx;
        this.player.position.y += mvy;

        mvx = math.abs(mvx);

        if (mvx == 0 && mvy == 0)
            this.player.animations.enabled = false;
        else
            this.player.animations.enabled = true;

        if (mvx > 0) {
            setAnimation(this.player.animations, "walkRight");
        }
        else if (mvy > 0){
            setAnimation(this.player.animations, "walkDown");
        }
        else if (mvy < 0){
            setAnimation(this.player.animations, "walkUp");
        }
    }
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
            if (!anim.enabled)
                continue;
            const sprite = e.sprite;
            if (ticks > anim.timer) {
                if (++anim.currentFrame > anim.current.frames.length - 1)
                    anim.currentFrame = 0;
                sprite.sprite = anim.current.frames[anim.currentFrame];
                anim.timer = ticks + anim.current.speed;
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
    update(_: number): void {
        for (let i = 0; i < this.entities.length; i++) {
            const de = this.entities[i];
            font(null, 10, 10, de.sprite.sprite.toString())
            sprite(de.sprite.bmp, de.sprite.sprite, de.position.x, de.position.y, de.sprite.w, de.sprite.h, 1, de.sprite.flip);
        }
    }
}
