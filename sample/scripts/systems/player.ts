import { setAnimation } from "../components";
import { Entity, EntityFlags } from "../entity";
import { SystemBase } from "./base";

const isButtonDown = btn;
const abs = math.abs;

const PlayerSpeed = 1;

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
            const components = this.entities[i].components;
            const pos = components.position!;
            const spr = components.sprite!;
            const anims = components.animations!;

            let mvx = 0;
            let mvy = 0;

            if (isButtonDown(1)) {
                mvy -= PlayerSpeed;
            }
            if (isButtonDown(2)) {
                mvy += PlayerSpeed;
            }
            if (isButtonDown(3)) {
                mvx -= PlayerSpeed;
                spr.flip = 1;
            }
            if (isButtonDown(4)) {
                mvx += PlayerSpeed;
                spr.flip = 0;
            }

            pos.x += mvx;
            pos.y += mvy;
            anims.enabled = mvx != 0 || mvy != 0;

            if (abs(mvx) > 0)
                setAnimation(anims, "walkRight");
            else if (mvy > 0)
                setAnimation(anims, "walkDown");
            else if (mvy < 0)
                setAnimation(anims, "walkUp");
        }
    }
}
