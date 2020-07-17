import { StarField } from "../common/starField";
import { Game, GameState } from "../game";
import { TacoStand } from "../common/tacoStand";
import { ExplosionPool } from "../common/explosions";
import { GameplayState } from "../gameplay/state";
import { Action, ActionList } from "../common/actionList";

const ExplosionSound = 0;
const EvilLaughSound = 1;

export interface ActionContext {
    game: Game;
    starField: StarField;
    tacoStand: TacoStand;
}

class Load implements Action<ActionContext> {

    enter(_: ActionContext): void {
        loadsnd(ExplosionSound, "sounds/explode.wav");
        loadsnd(EvilLaughSound, "sounds/laugh.wav");
    }
    update(t: ActionContext): boolean {
        return true;
    }
    draw(t: ActionContext): void { clrs(); }
    exit(t: ActionContext): void {}
}

class Free implements Action<ActionContext> {

    enter(_: ActionContext): void {
        freesnd(ExplosionSound);
        freesnd(EvilLaughSound);
    }
    update(t: ActionContext): boolean {
        return true;
    }
    draw(t: ActionContext): void { clrs(); }
    exit(t: ActionContext): void {}
}

class Explosions implements Action<ActionContext> {

    private _explosionPool = new ExplosionPool();
    private _timer = 0;

    enter(context: ActionContext): void {
        this._timer = context.game.ticks + 200;
        context.tacoStand.panic();
    }
    update(context: ActionContext): boolean {
        context.starField.update();
        context.tacoStand.update(context.game.ticks, true);
        this._explosionPool.update(context.game);
        if (context.game.ticks % 15 == 0) {
            this._explosionPool.create(math.random(10, 200), math.random(170, 200), context.game.ticks);
            this._explosionPool.create(math.random(10, 50), math.random(170, 200), context.game.ticks);
            play(ExplosionSound, 0, 128);
        }
        return context.game.ticks > this._timer;
    }
    draw(context: ActionContext): void {
        clrs();
        context.starField.draw();
        context.tacoStand.draw(true);
        this._explosionPool.draw();
    }
    exit(t: ActionContext): void {}
}

class TacoStandDestroyedDelay implements Action<ActionContext> {

    private _timer = 0;

    enter(context: ActionContext): void {
        this._timer = context.game.ticks + 100;
    }
    update(context: ActionContext): boolean {
        context.starField.update();
        context.tacoStand.update(context.game.ticks, false);
        return context.game.ticks > this._timer;
    }
    draw(context: ActionContext): void {
        clrs();
        context.starField.draw();
        context.tacoStand.draw(false);
    }
    exit(t: ActionContext): void {}
}

class SatanEnter implements Action<ActionContext> {

    private _satanPosition = -10;

    enter(_: ActionContext): void {}
    update(context: ActionContext): boolean {
        context.starField.update();
        this._satanPosition += 1;
        context.tacoStand.update(context.game.ticks, false);

        return this._satanPosition >= 100;
    }
    draw(context: ActionContext): void {
        clrs();
        context.starField.draw();
        context.tacoStand.draw(false);
        spr(49, 128, this._satanPosition);
    }
    exit(t: ActionContext): void {}
}

class HailSatanAction implements Action<ActionContext> {

    private _timer = 0;
    private _satanFrame = 49;

    enter(context: ActionContext): void {
        this._timer = context.game.ticks + 200;
        play(EvilLaughSound, 0, 128);
    }
    update(context: ActionContext): boolean {
        context.starField.update();
        this._satanFrame = context.game.ticks % 12 > 6 ? 49 : 50;
        context.tacoStand.update(context.game.ticks, false);
        return context.game.ticks > this._timer;
    }
    draw(context: ActionContext): void {
        clrs();
        context.starField.draw();
        context.tacoStand.draw(false);
        spr(this._satanFrame, 128, 100);
        font(0, 20, 50, "Hail Satan, motherfuckers!!!");
    }
    exit(t: ActionContext): void { }
}

class ScreenWipeAction implements Action<ActionContext> {

    private _satanFrame = 49;
    private _rectPos = 256;

    enter(context: ActionContext): void {}
    update(context: ActionContext): boolean {
        context.starField.update();
        this._satanFrame = context.game.ticks % 12 > 6 ? 49 : 50;
        context.tacoStand.update(context.game.ticks, false);
        this._rectPos -= 2;
        return this._rectPos == 0;
    }
    draw(context: ActionContext): void {
        clrs();
        context.starField.draw();
        context.tacoStand.draw(false);
        spr(this._satanFrame, 128, 100);
        rectfill(this._rectPos, 0, 265 - this._rectPos, 224, 0x00);
    }
    exit(t: ActionContext): void {}
}

export const actions = [
    new Load(),
    new Explosions(),
    new TacoStandDestroyedDelay(),
    new SatanEnter(),
    new HailSatanAction(),
    new ScreenWipeAction(),
    new Free(),
];
