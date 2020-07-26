import { Stars } from "../common/stars";
import { Player } from "../common/player";
import { Game, GameState } from "../game";
import { Bullets } from "../common/bullets";
import { Enemies } from "../common/enemies";
import { Explosions } from "../common/explosions";
import { isColliding } from "../common/collision";
import { PauseState } from "../pause/state";

const MusicStream = 0;
const ExplosionSound = 0;
const ShootSound = 1;

export class GameplayState implements GameState {

    starField = new Stars();
    player = new Player({ shootSound: ShootSound });
    explosions = new Explosions({ explosionSound: ExplosionSound });
    enemies = new Enemies();
    bullets = new Bullets();

    updateBelow = false;
    drawBelow = false;

    get bulletPool() {
        return this.bullets;
    }

    enter(_: Game): void {
        loadspr("art/sprsheet.bmp");
        openstream(MusicStream, "sounds/02 Underclocked (underunderclocked mix).wav");
        loadsnd(ShootSound, "sounds/shoot.wav");
        loadsnd(ExplosionSound, "sounds/explode.wav");
        playstream(MusicStream, 128, true);
    }

    update(game: Game): void {
        if (btnp(0))
            game.pushState(new PauseState());

        this.starField.update();
        this.player.update(game, this);
        this.enemies.update(game);
        this.bullets.update();
        this.explosions.update(game);

        this.handleBulletEnemyCollisions(game.ticks);
        this.handlePlayerEnemyCollisions(game.ticks);
    }

    draw(_: Game): void {
        clrs(0x00);

        this.starField.draw();
        this.bullets.draw();
        this.enemies.draw();
        this.explosions.draw();
        this.player.draw();
    }

    exit(_: Game): void {
        closestream(MusicStream);
        freesnd(ShootSound);
        freesnd(ExplosionSound);
    }

    // Will want to revisit this implementation.
    private handleBulletEnemyCollisions(ticks: number): void {
        for (const enemy of this.enemies.liveEnemies) {
            if (this.bullets.checkCollision(enemy)) {
                this.explosions.create(enemy.x, enemy.y, ticks);
                this.enemies.damage(enemy, 1);
            }
        }
    }

    // Will want to revisit this implementation.
    private handlePlayerEnemyCollisions(ticks: number): void {
        for (const enemy of this.enemies.liveEnemies) {
            if (isColliding(this.player, enemy)) {
                this.explosions.create(enemy.x, enemy.y, ticks);
                this.enemies.damage(enemy, 1);
            }
        }
    }
}
