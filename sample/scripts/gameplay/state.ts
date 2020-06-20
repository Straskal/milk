import { GameState } from "../gameState";
import { StarField } from "../common/starField";
import { Player } from "./player";
import { Game } from "../game";
import { BulletPool } from "./bullets";
import { EnemyPool } from "./enemies";
import { ExplosionPool } from "./explosions";
import { isColliding } from "./collision";

export class GameplayState implements GameState {

    private _starField = new StarField();
    private _player = new Player();
    private _enemyPool = new EnemyPool();
    private _bulletPool = new BulletPool();
    private _explosionPool = new ExplosionPool();
    private _score = 0;

    public get bulletPool() {
        return this._bulletPool;
    }

    public enter(_: Game): void {
        loadspr("art/sprsheet.bmp");
        loadsnd(0, "sounds/music.wav");
        loadsnd(1, "sounds/shoot.wav");
        loadsnd(2, "sounds/explode.wav");
        play(0, 0, 128);
    }

    public update(game: Game): void {
        this._starField.update();
        this._player.update(game, this);
        this._enemyPool.update(game);
        this._bulletPool.update();
        this._explosionPool.update(game);

        this.handleBulletEnemyCollisions(game.ticks);
        this.handlePlayerEnemyCollisions(game.ticks);
    }

    public draw(_: Game): void {
        clrs(0x00);

        this._starField.draw();
        this._bulletPool.draw();
        this._enemyPool.draw();
        this._explosionPool.draw();
        this._player.draw();

        this.drawScore();
    }

    public exit(_: Game): void { }

    private handleBulletEnemyCollisions(ticks: number): void {
        for (const enemy of this._enemyPool.liveEnemies) {
            if (this._bulletPool.checkCollision(enemy)) {
                this._explosionPool.create(enemy.x, enemy.y, ticks);
                this._enemyPool.destroy(enemy);
                this._score++;
            }
        }
    }

    private handlePlayerEnemyCollisions(ticks: number): void {
        for (const enemy of this._enemyPool.liveEnemies) {
            if (isColliding(this._player, enemy)) {
                this._explosionPool.create(enemy.x, enemy.y, ticks);
                this._enemyPool.destroy(enemy);
            }
        }
    }

    private drawScore(): void {
        sprfont(10, 10, `SCORE:${this._score}`);
    }
}