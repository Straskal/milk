import { Behavior, Sprite } from "../core/sprite";
import { Level } from "../core/level";
import { setAnimation } from "../common/animation";

const isButtonDown = btn;
const abs = math.abs;

const PlayerSpeed = 1;

export class Player implements Behavior {

    update(sprite: Sprite): void {
        const anims = sprite.animations!;

        let mvx = 0;
        let mvy = 0;

        if (isButtonDown(1))
            mvy -= PlayerSpeed;
        if (isButtonDown(2))
            mvy += PlayerSpeed;
        if (isButtonDown(3)) {
            mvx -= PlayerSpeed;
            sprite.flip = 1;
        }
        if (isButtonDown(4)) {
            mvx += PlayerSpeed;
            sprite.flip = 0;
        }

        Level.current.moveAndCollide(sprite, mvx, mvy);
        Level.current.x = sprite.x - 384 / 2;
        Level.current.y = sprite.y - 216 / 2;

        anims.enabled = mvx != 0 || mvy != 0;

        if (abs(mvx) > 0)
            setAnimation(anims, "walkRight");
        else if (mvy > 0)
            setAnimation(anims, "walkDown");
        else if (mvy < 0)
            setAnimation(anims, "walkUp");
    }
}
