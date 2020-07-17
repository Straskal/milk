interface Star {
    x: number;
    y: number;
    pace: number;
    color: number;
}

export class Stars {

    private readonly _stars: Star[] = [];

    constructor(readonly warpFactor: number = 3) {
        for (let i = 0; i < 10; i++) {
            for (let j = 0; j < 10; j++) {
                this._stars.push({
                    x: math.random(256),
                    y: math.random(224),
                    pace: i,
                    color: math.random(500000)
                });
            }
        }
    }

    public update(): void {
        for (let i = 0; i < this._stars.length; i++) {
            const star = this._stars[i];
            star.y = star.y + star.pace * this.warpFactor / 10;
            if (star.y > 224) {
                star.y = 0;
                star.x = math.random(256);
            }
        }
    }

    public draw(): void {
        for (let i = 0; i < this._stars.length; i++) {
            pset(this._stars[i].x, this._stars[i].y, this._stars[i].color)
        }
    }
}
