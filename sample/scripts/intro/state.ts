import { StarField } from "../common/starField";
import { Game, GameState } from "../game";
import { TacoStand } from "../common/tacoStand";
import { ExplosionPool } from "../common/explosions";
import { GameplayState } from "../gameplay/state";
import { Action, ActionList } from "../common/actionList";

interface ActionContext {
    game: Game;
    state: IntroState;
}

class ExplosionAction implements Action<ActionContext> {

    private _explosionPool = new ExplosionPool();
    private _timer = 0;

    public enter(context: ActionContext): void {
        this._timer = context.game.ticks + 200;
        context.state._tacoStand.panic();
    }

    public update(context: ActionContext): boolean {
        context.state._tacoStand.update(context.game.ticks, true);
        this._explosionPool.update(context.game);
        if (context.game.ticks % 15 == 0) {
            this._explosionPool.create(math.random(10, 200), math.random(170, 200), context.game.ticks);
            this._explosionPool.create(math.random(10, 50), math.random(170, 200), context.game.ticks);
        }
        return context.game.ticks > this._timer;
    }

    public draw(context: ActionContext): void {
        context.state._tacoStand.draw(true);
        this._explosionPool.draw();
    }

    public exit(t: ActionContext): void {}
}

class DestroyedAction implements Action<ActionContext> {

    private _timer = 0;

    public enter(context: ActionContext): void {
        this._timer = context.game.ticks + 100;
    }

    public update(context: ActionContext): boolean {
        context.state._tacoStand.update(context.game.ticks, false);
        return context.game.ticks > this._timer;
    }

    public draw(context: ActionContext): void {
        context.state._tacoStand.draw(false);
    }

    public exit(t: ActionContext): void {}
}

class SatanEntranceAction implements Action<ActionContext> {

    private _satanPosition = -10;

    public enter(_: ActionContext): void { }

    public update(context: ActionContext): boolean {
        this._satanPosition += 1;
        context.state._tacoStand.update(context.game.ticks, false);

        return this._satanPosition >= 100;
    }

    public draw(context: ActionContext): void {
        context.state._tacoStand.draw(false);
        spr(49, 128, this._satanPosition);
    }

    public exit(t: ActionContext): void {}
}

class HailSatanAction implements Action<ActionContext> {

    private _timer = 0;
    private _satanFrame = 49;

    public enter(context: ActionContext): void {
        this._timer = context.game.ticks + 200;
    }

    public update(context: ActionContext): boolean {
        this._satanFrame = context.game.ticks % 12 > 6 ? 49 : 50;
        context.state._tacoStand.update(context.game.ticks, false);
        return context.game.ticks > this._timer;
    }

    public draw(context: ActionContext): void {
        context.state._tacoStand.draw(false);
        spr(this._satanFrame, 128, 100);
        font(0, 20, 50, "Hail Satan, motherfuckers!!!");
    }

    public exit(t: ActionContext): void {}
}

class ScreenWipeAction implements Action<ActionContext> {

    private _satanFrame = 49;
    private _rectPos = 256;

    public enter(context: ActionContext): void {
    }

    public update(context: ActionContext): boolean {
        this._satanFrame = context.game.ticks % 12 > 6 ? 49 : 50;
        context.state._tacoStand.update(context.game.ticks, false);
        this._rectPos -= 2;
        return this._rectPos == 0;
    }

    public draw(context: ActionContext): void {
        context.state._tacoStand.draw(false);
        spr(this._satanFrame, 128, 100);
        rectfill(this._rectPos, 0, 265 - this._rectPos, 224, 0x00);
    }

    public exit(t: ActionContext): void {}
}

export class IntroState implements GameState {

    public updateBelow = false;
    public drawBelow = false;
    public _starField = new StarField(0.1);
    public _tacoStand = new TacoStand();

    private _actionList = new ActionList([
        new ExplosionAction(),
        new DestroyedAction(),
        new SatanEntranceAction(),
        new HailSatanAction(),
        new ScreenWipeAction()
    ]);

    private _actionContext!: ActionContext;

    public enter(game: Game): void {
        loadsnd(2, "sounds/explode.wav");

        this._actionContext = {
            game: game,
            state: this
        };
    }

    public update(game: Game): void {
        this._starField.update();

        if (this._actionList.update(this._actionContext)) {
            game.popState();
            game.pushState(new GameplayState());
        }
    }

    public draw(game: Game): void {
        clrs(0x00);

        this._starField.draw();
        this._actionList.draw(this._actionContext);
    }

    public exit(_: Game): void {}
}
