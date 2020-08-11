import { newWorld } from "../libs/bump";
import { Sprite, updateSpriteComponents, drawSprites } from "../core/sprite";
import { Game, GameState } from "../game";
import { animateSprites } from "../common/animation";
import { drawTilemap, clampScreenToMap, Tilemap } from "../core/tilemap";

export class Level implements GameState {

    static current: Level;

    updateBelow = false;
    drawBelow = false;

    x = 0;
    y = 0;

    bumpWorld = newWorld(16);
    map!: Tilemap;
    sprites: Sprite[] = [];
    updated: Sprite[] = [];
    animated: Sprite[] = [];

    enter(): void {
        Level.current = this;
        const collisions = this.map.collisions;
        const columns = this.map.width;
        if (collisions) {
            for (let i = 0; i < collisions.length; i++) {
                if (collisions[i] == 1) {
                    const row = i / columns;
                    const col = i % columns;
                    this.bumpWorld.add({}, col * 16, row * 16, 16, 16);
                }
            }
        }
    }

    exit(): void {

    }

    add(sprite: Sprite): void {
        this.sprites.push(sprite);
        if (sprite.behaviors)
            this.updated.push(sprite);
        if (sprite.animations)
            this.animated.push(sprite);
        if (sprite.body)
            this.bumpWorld.add(sprite, sprite.x, sprite.y, sprite.body.width, sprite.body.height);
    }

    moveAndCollide(sprite: Sprite, x: number, y: number): void {
        const [actualX, actualY] = this.bumpWorld.move(sprite, sprite.x + x, sprite.y + y);
        sprite.x = actualX;
        sprite.y = actualY;
    }

    update(): void {
        updateSpriteComponents(this.updated);
        animateSprites(this.animated, Game.ticks);
    }

    draw(): void {
        [this.x, this.y] = clampScreenToMap(this.map, this.x, this.y);
        drawTilemap(this.map, this.x, this.y);
        drawSprites(this.sprites, this.x, this.y);
    }
}
