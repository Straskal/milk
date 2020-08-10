import { setAnimation } from "../components";
import { Entity, EntityFlags } from "../entity";
import { SystemBase } from "./base";

export class PlayerSystem extends SystemBase {

    onEntityAdded(e: Entity): void {
        if ((e.flags & EntityFlags.PLAYER) == EntityFlags.PLAYER)
            this.add(e);
    }
    onEntityRemoved(e: Entity): void {
        if ((e.flags & EntityFlags.PLAYER) == EntityFlags.PLAYER)
            this.remove(e);
    }
    update(_: number): void {
        for (let i = 0; i < this.entities.length; i++) {
            const pos = this.entities[i].components.position;
            const spr = this.entities[i].components.sprite;
            const anims = this.entities[i].components.animations;

            let mvx = 0;
            let mvy = 0;

            if (btn(1)) {
                mvy -= 1;
            }
            if (btn(2)) {
                mvy += 1;
            }
            if (btn(3)) {
                mvx -= 1;
                spr.flip = 1;
            }
            if (btn(4)) {
                mvx += 1;
                spr.flip = 0;
            }

            pos.x += mvx;
            pos.y += mvy;

            mvx = math.abs(mvx);

            if (mvx == 0 && mvy == 0)
                anims.enabled = false;
            else
                anims.enabled = true;

            if (mvx > 0) {
                setAnimation(anims, "walkRight");
            }
            else if (mvy > 0){
                setAnimation(anims, "walkDown");
            }
            else if (mvy < 0){
                setAnimation(anims, "walkUp");
            }
        }
    }
}
