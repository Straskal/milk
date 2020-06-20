import { GameState } from "../gameState";
import { StarField } from "../common/starField";
import { Game } from "../game";
import { GameplayState } from "../gameplay/state";

export class IntroState implements GameState {

    private readonly title = "STAR GAME";
    private readonly subTitle = "Returns of The Bad Boiz";
    private readonly prompt = "press Z";

    private starField = new StarField();

    public enter(_: Game): void {
        loadsnd(0, "sounds/intro_music.wav");
        play(0, 0, 128);
    }

    public update(game: Game): void {
        this.starField.update();

        if (btnp(5)) {
            game.popState();
            game.pushState(new GameplayState());
        }
    }

    public draw(game: Game): void {
        clrs(0x00);
        
        this.starField.draw();
        this.drawTitle();
        this.drawSubtitle(game.ticks);
        this.drawPrompt(game.ticks);
    }

    public exit(_: Game): void { }

    private printCenteredHorizontally(text: string, y: number, scale: number, color: number): void {
        const length = string.len(text);
        const halfLength = length / 2;
        const halfResolutionWidth = 256 / 2;
        const characterSize = 8 * scale;

        sprfont(halfResolutionWidth - halfLength * characterSize, y, text, scale, color);
    }

    private drawTitle(): void {
        this.printCenteredHorizontally(this.title, 60, 2, 0x00ff000);
    }

    private drawSubtitle(ticks: number): void {
        const length = string.len(this.subTitle);
        const halfLength = length / 2;
        const halfResolutionWidth = 256 / 2;
        const x = halfResolutionWidth - halfLength * 8;

        for (let i = 1; i <= length; i++) {
            const character = string.sub(this.subTitle, i, i);
            const adjustedTime = ticks + i * 4;
            const y = 85 + math.sin(adjustedTime / 15) * 4;

            sprfont(x + 8 * i, y, character, 1, 0x008751);
        }
    }

    private drawPrompt(ticks: number): void {
        if (ticks % 64 < 48) {
            this.printCenteredHorizontally(this.prompt, 164, 1, 0xffffff);
        }
    }
}