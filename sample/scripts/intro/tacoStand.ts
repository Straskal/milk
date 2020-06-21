const SMOKE_START_FRAME = 31;
const SMOKE_NUM_FRAME = 4;

export class TacoStand {

    private _smokeFrame = SMOKE_START_FRAME;
    private _smokePosition = 20;

    public update(ticks: number): void {
        this.animateSmoke(ticks);
    }

    public draw(): void {
        this.drawSmoke();
        this.drawTacoPlace();
    }

    private animateSmoke(ticks: number): void {
        if (ticks % 12 == 6) {
            this._smokeFrame++;
            this._smokePosition = math.random(17, 24);

            if (this._smokeFrame > SMOKE_START_FRAME + SMOKE_NUM_FRAME)
                this._smokeFrame = SMOKE_START_FRAME;
        }
    }

    private drawTacoPlace(): void {
        spr(209, 16, 176, 16, 3);
    }

    private drawSmoke(): void {
        
        spr(this._smokeFrame, this._smokePosition, 183);
    }
}