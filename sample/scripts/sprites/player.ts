import { UpdateHandler, Sprite, setAnimation } from "../sprite";
import { Level } from "../level";

const isButtonDown = btn;
const abs = math.abs;

const PlayerSpeed = 1;

export class Player implements UpdateHandler {

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
        Level.current.x = sprite.x - 144;
        Level.current.y = sprite.y - 96;

        anims.enabled = mvx != 0 || mvy != 0;

        if (abs(mvx) > 0)
            setAnimation(anims, "walkRight");
        else if (mvy > 0)
            setAnimation(anims, "walkDown");
        else if (mvy < 0)
            setAnimation(anims, "walkUp");
    }
}
