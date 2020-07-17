import { StarField } from "../common/starField";
import { Player } from "../common/player";
import { Game, GameState } from "../game";
import { BulletPool } from "../common/bullets";
import { EnemyPool } from "../common/enemies";
import { ExplosionPool } from "../common/explosions";
import { isColliding } from "../common/collision";
import { PauseState } from "../pause/state";

export const ExplosionSound = 0;
export const ShootSound = 1;

export class GameplayState implements GameState {

    private _starField = new StarField();
    private _player = new Player();
    private _enemyPool = new EnemyPool();
    private _bulletPool = new BulletPool();
    private _explosionPool = new ExplosionPool();
    private _score = 0;

    updateBelow = false;
    drawBelow = false;

    get bulletPool() {
        return this._bulletPool;
    }

    addToScore(amount: number): void {
        this._score += amount;
    }

    enter(_: Game): void {
        openstream(0, "sounds/02 Underclocked (underunderclocked mix).wav");
        loadsnd(ShootSound, "sounds/shoot.wav");
        loadsnd(ExplosionSound, "sounds/explode.wav");
        playstream(0, 128, true);
    }

    update(game: Game): void {
        if (btnp(0))
            game.pushState(new PauseState());

        this._starField.update();
        this._player.update(game, this);
        this._enemyPool.update(game);
        this._bulletPool.update();
        this._explosionPool.update(game);

        this.handleBulletEnemyCollisions(game.ticks);
        this.handlePlayerEnemyCollisions(game.ticks);
    }

    draw(_: Game): void {
        clrs(0x00);

        this._starField.draw();
        this._bulletPool.draw();
        this._enemyPool.draw();
        this._explosionPool.draw();
        this._player.draw();

        this.drawScore();
    }

    exit(_: Game): void {
        closestream(0);
        freesnd(1);
        freesnd(2);
    }

    private handleBulletEnemyCollisions(ticks: number): void {
        for (const enemy of this._enemyPool.liveEnemies) {
            if (this._bulletPool.checkCollision(enemy)) {
                this._explosionPool.create(enemy.x, enemy.y, ticks);
                this._enemyPool.destroy(enemy);
                this._score++;

                play(ExplosionSound, 0, 80);
            }
        }
    }

    private handlePlayerEnemyCollisions(ticks: number): void {
        for (const enemy of this._enemyPool.liveEnemies) {
            if (isColliding(this._player, enemy)) {
                this._explosionPool.create(enemy.x, enemy.y, ticks);
                this._enemyPool.destroy(enemy);

                play(ExplosionSound, 0, 80);
            }
        }
    }

    private drawScore(): void {
        font(0, 10, 10, `SCORE:${this._score}`);
    }
}
