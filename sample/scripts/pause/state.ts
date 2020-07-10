import { GameState, Game } from "../game";
import { TacoStand } from "../common/tacoStand";

export class PauseState implements GameState {

    public updateBelow = false;
    public drawBelow = true;

    private _ticksWhenPaused = 0;
    private _tacoStand = new TacoStand();

    public enter(game: Game): void {
        this._ticksWhenPaused = game.ticks;
        pause(-1);
        play(15, 15, 50);
    }

    public update(game: Game): void {
        if (btnp(0))
            game.popState(); // Pop self.

        this._tacoStand.update(game.ticks, false);
    }

    public draw(_: Game): void {
        this.drawOverlay();
        this._tacoStand.draw(false);

        const paused = "paused";
        const pausedLength = string.len(paused);
        const halfResolution = 256 / 2;
        const x = halfResolution - (pausedLength / 2) * 8;

        rectfill(x - 10, 95, pausedLength * 8 + 15, 18, 0x00);        
        sprfont(x, 100, paused)
    }

    public exit(game: Game): void {
        game.ticks = this._ticksWhenPaused;
        stop(15);
        resume(-1);
    }

    private drawOverlay(): void {
        for (let y = 0; y < 256; y++) {
            const xOffset = y % 2 == 0 ? 0 : 1;
            for (let x = xOffset; x < 256; x +=2 ) {
                pset(x, y, 0x00);
            }
        }
    }
}