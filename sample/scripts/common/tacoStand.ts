const SMOKE_START_FRAME = 31;
const SMOKE_NUM_FRAME = 4;
const SMOKE_ANIM_TIMER = 20;
const PERSON_START_FRAME = 192;
const PERSON_NUM_FRAME = 2;
const PERSON_ANIM_TIMER = 10;

interface Smoke {
    x: number;
    y: number;
    frame: number;
    timer: number;
}

interface Person {
    x: number;
    frame: number;
    timer: number;
    alive: boolean;
    direction: number;
    speed: number;
}

export class TacoStand {

    private _smokeFrame = SMOKE_START_FRAME;
    private _smokePosition = 20;
    private _bigSmokeFrames: Smoke[] = [];
    private _people: Person[] = [];

    constructor() {
        for (let i = 0; i < 20; i++){
            this._bigSmokeFrames[i] = {
                x: math.random(15, 40),
                y: math.random(180, 190),
                frame: math.random(SMOKE_START_FRAME, SMOKE_START_FRAME + SMOKE_NUM_FRAME - 1),
                timer: 0
            }
        }

        for (let i = 0; i < 3; i++){
            this._people[i] = {
                x: math.random(60, 176),
                frame: math.random(PERSON_START_FRAME, PERSON_START_FRAME + PERSON_NUM_FRAME - 1),
                timer: math.random(0, 20),
                alive: true,
                direction: i % 2 == 0 ? -1 : 1,
                speed: i % 2 == 0 ? 0.5 : 0.8
            }
        }
    }

    public init(ticks: number): void {
        for (let i = 0; i < 20; i++){
            this._bigSmokeFrames[i].timer = ticks + math.random(0, SMOKE_ANIM_TIMER);
        }
    }

    public update(ticks: number, isEverythingFine: boolean = true): void {
        if (isEverythingFine)
            this.updateEverythingIsFine(ticks);
        else
            this.updateEverythingIsNotFine(ticks);
    }

    public draw(isEverythingFine: boolean = true): void {
        if (isEverythingFine)
            this.drawEverythingIsFine();
        else
            this.drawEverythingIsNotFine();
    }

    public panic(): void {
        for (const person of this._people)
            person.speed = 1.5;
    }

    private updateEverythingIsFine(ticks: number): void {
        this.animateSmoke(ticks);
        this.animatePeople(ticks);
    }

    private updateEverythingIsNotFine(ticks: number): void {
        this.animateBigSmoke(ticks);
    }

    private animateBigSmoke(ticks: number): void {
        for (const smoke of this._bigSmokeFrames) {
            if (ticks > smoke.timer) {
                smoke.x = math.random(15, 40);

                if (++smoke.frame > SMOKE_START_FRAME + SMOKE_NUM_FRAME)
                    smoke.frame = SMOKE_START_FRAME;

                smoke.timer = ticks + SMOKE_ANIM_TIMER;
            }
        }
    }

    private animateSmoke(ticks: number): void {
        if (ticks % 12 == 6) {
            this._smokeFrame++;
            this._smokePosition = math.random(17, 24);

            if (this._smokeFrame > SMOKE_START_FRAME + SMOKE_NUM_FRAME)
                this._smokeFrame = SMOKE_START_FRAME;
        }
    }

    private animatePeople(ticks: number): void {
        for (const person of this._people) {
            person.x += person.direction * person.speed;
            if (ticks > person.timer) {
                if (person.x < 20 || person.x > 200)
                    person.direction *= -1;

                if (++person.frame > PERSON_START_FRAME + PERSON_NUM_FRAME)
                    person.frame = PERSON_START_FRAME;

                person.timer = ticks + PERSON_ANIM_TIMER;
            }
        }
    }

    private drawEverythingIsFine(): void {
        spr(this._smokeFrame, this._smokePosition, 183);
        spr(209, 16, 176, 16, 3);
        spr(160, 16, 176, 4, 2);

        for (const person of this._people)
            spr(person.frame, person.x, 200, 1, 1, 1, person.direction == -1 ? 0 : 1);
    }

    private drawEverythingIsNotFine(): void {
        for (const smoke of this._bigSmokeFrames)
            spr(smoke.frame, smoke.x, smoke.y);

        spr(209, 16, 176, 16, 3);
        spr(164, 16, 176, 4, 2);

        for (const person of this._people)
            spr(195, person.x, 204, 1, 1, 1, person.direction == -1 ? 0 : 1);
    }
}
