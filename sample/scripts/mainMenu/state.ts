import { Stars } from "../common/stars";
import { Game, GameState } from "../game";
import { Menu } from "./menu";
import { TacoStand } from "../common/tacoStand";

const PRESENTED_BY = "It'sgood's"
const TITLE = "STAR GAME";
const SUBTITLE = "Return of The Bad Boiz";

export class MainMenuState implements GameState {

    public updateBelow = false;
    public drawBelow = false;

    private _starField = new Stars(0.1);
    private _tacoStand = new TacoStand();
    private _menu = new Menu();

    public enter(_: Game): void {
        loadspr("art/sprsheet.bmp");
        openstream(0, "sounds/12 HHavok-main.wav");
        playstream(0, 128, true);
    }

    public update(game: Game): void {
        this._starField.update();
        this._tacoStand.update(game.ticks);
        this._menu.update(game);
    }

    public draw(game: Game): void {
        clrs(0x00);

        this._starField.draw();
        this._tacoStand.draw();
        this._menu.draw(256 / 2, 150, game.ticks);

        this.drawTitle();
        this.drawSubtitle(game.ticks);
    }

    public exit(_: Game): void {
        closestream(0);
    }

    private drawTitle(): void {
        const length = string.len(TITLE);
        const halfLength = length / 2;
        const halfResolutionWidth = 256 / 2;

        font(0, halfResolutionWidth - halfLength * 16, 45, PRESENTED_BY, 1, 0x00ffffff);
        font(0, halfResolutionWidth - halfLength * 16, 60, TITLE, 2, 0x00ff000);
    }

    private drawSubtitle(ticks: number): void {
        const length = string.len(SUBTITLE);
        const halfLength = length / 2;
        const halfResolutionWidth = 256 / 2;
        const x = halfResolutionWidth - halfLength * 8;

        for (let i = 1; i <= length; i++) {
            const character = string.sub(SUBTITLE, i, i);
            const adjustedTime = ticks + i * 4;
            const y = 85 + math.sin(adjustedTime / 15) * 4;

            font(0, x + 8 * i, y, character, 1, 0x008751);
        }
    }
}
