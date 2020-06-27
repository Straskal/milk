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
    }

    public update(context: ActionContext): boolean {
        context.state._tacoStand.update(context.game.ticks, true);
        this._explosionPool.update(context.game);
        if (context.game.ticks % 15 == 0) {
            this._explosionPool.create(math.random(10, 50), math.random(170, 200), context.game.ticks);
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
        this._timer = context.game.ticks + 300;
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

export class IntroState implements GameState {    

    public updateBelow = false;
    public drawBelow = false;
    public _starField = new StarField(0.1);
    public _tacoStand = new TacoStand();

    private _actionList = new ActionList([
        new ExplosionAction(),
        new DestroyedAction()
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
