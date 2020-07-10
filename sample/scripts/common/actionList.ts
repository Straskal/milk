// actionList.ts

/**
 * An action updates and returns true when finished, indicating that the action list should move forward.
 */
export interface Action<T> {
    enter(t: T): void;
    update(t: T): boolean;
    draw(t: T): void;
    exit(t: T): void;
}

/**
 * An action list moves through each of it's actions linearly.
 * Each action is responsible for indicating when the list should move forward.
 * 
 * Action lists are good for anything sequencial, like cutscenes.
 */
export class ActionList<T> {

    private _currentAction = 0;
    private _started = false;
    private _finished = false;

    constructor(private readonly _actions: Action<T>[]) {}

    /**
     * Moves the action list forward and returns true when the action list has completed.
     * @param t
     */
    public update(t: T): boolean {
        // Enter the first state.
        if (!this._started) {
            this._actions[this._currentAction].enter(t);
            this._started = true;
        }

        if (!this._finished && this._actions[this._currentAction].update(t)) {            
            this._actions[this._currentAction].exit(t);
            this._finished = ++this._currentAction == this._actions.length;
            
            if (!this._finished)                
                this._actions[this._currentAction].enter(t);
        }
        return this._finished;
    }

    public draw(t: T): void {
        if (!this._finished)
            this._actions[this._currentAction].draw(t);
    }
}