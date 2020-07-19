import { Game } from "../game";
import { actions, ActionContext } from "../cinematics/intro";
import { CinematicState } from "../common/cinematic";
import { Stars } from "../common/stars";
import { TacoStand } from "../common/tacoStand";
import { GameplayState } from "../gameplay/state";

interface MenuItem {
    text: string,
    execute: (game: Game) => void;
}

export class Menu {

    private _selectedMenuItem = 0;
    private _menuItems: MenuItem[] = [
        {
            text: "play",
            execute: (game: Game) => {
                let introCinematicContext = {
                    game: game,
                    starField: new Stars(0.1),
                    tacoStand: new TacoStand()
                };

                game.popState();
                game.pushState(new CinematicState<ActionContext>(
                    introCinematicContext, actions, new GameplayState()
                ));
            }
        },
        {
            text: "quit",
            execute: (_) => exit()
        }
    ];

    public update(game: Game): void {
        const menuItemCount = this._menuItems.length;

        if (btnp(1) && --this._selectedMenuItem < 0)
            this._selectedMenuItem = 0;
        if (btnp(2) && ++this._selectedMenuItem >= menuItemCount)
            this._selectedMenuItem = menuItemCount - 1;
        if (btnp(5))
            this._menuItems[this._selectedMenuItem].execute(game);
    }

    public draw(x: number, y: number, ticks: number): void {
        const menuItemCount = this._menuItems.length;

        for (let i = 0; i < menuItemCount; i++) {
            const menuItem = this._menuItems[i];
            const textLength = string.len(menuItem.text);
            const halfLength = textLength / 2;
            const xPosition = x - halfLength * 8;

            if (i == this._selectedMenuItem)
                spr(ticks % 64 < 48 ? 36 : 37, x + (halfLength * 8) + 4, y - 4);

            font(0, xPosition, y, menuItem.text);
            y += 16;
        }
    }
}
