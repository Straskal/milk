interface Star {
    x: number;
    y: number;
    pace: number;
    color: number;
}

export class StarField {

    readonly warpFactor = 3;

    private stars: Star[] = [];

    constructor() {
        for (let i = 0; i < 10; i++) {
            for (let j = 0; j < 10; j++) {
                this.stars.push({
                    x: math.random(256),
                    y: math.random(224),
                    pace: i,
                    color: math.random(500000)
                });
            }
        }
    }

    public update(): void {
        for (let i = 0; i < this.stars.length; i++) {
            const star = this.stars[i];
            star.y = star.y + star.pace * this.warpFactor / 10;
            if (star.y > 224) {
                star.y = 0;
                star.x = math.random(256);
            }
        }
    }

    public draw(): void {
        for (let i = 0; i < this.stars.length; i++) {
            pset(this.stars[i].x, this.stars[i].y, this.stars[i].color)
        }
    }
}
