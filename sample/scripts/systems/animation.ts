import { Entity } from "../entity";
import { SystemBase } from "./base";

export class AnimationSystem extends SystemBase {

    onEntityAdded(e: Entity): void {
        if (e.components.animations && e.components.sprite)
            this.add(e);
    }
    onEntityRemoved(e: Entity): void {
        if (e.components.animations && e.components.sprite)
            this.remove(e);
    }
    update(ticks: number): void {
        for (let i = 0; i < this.entities.length; i++) {
            const e = this.entities[i];
            const anim = e.components.animations!;

            if (!anim.enabled)
                continue;

            const sprite = e.components.sprite!;

            if (ticks > anim.timer) {
                if (++anim.currentFrame > anim.current.frames.length - 1)
                    anim.currentFrame = 0;

                sprite.sprite = anim.current.frames[anim.currentFrame];
                anim.timer = ticks + anim.current.speed;
            }
        }
    }
}
