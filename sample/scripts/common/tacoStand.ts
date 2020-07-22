const SmokeStartFrame = 31;
const SmokeNumFrames = 4;
const SmokeAnimationTimer = 20;
const NumSmokeForCloud = 20;

const PersonStartFrame = 192;
const PersonNumFrames = 2;
const PersonAnimationTimer = 10;
const DeadPersonFrame = 195;
const NumPeople = 3;

const TacoStandLowerHalf = 241;
const TacoStandUpperHalf = 160;
const TacoStandUpperHalfDestroyed = 164;

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
    direction: number;
    speed: number;
}

export class TacoStand {

    private _tacoStandUpperHalfSprite = TacoStandUpperHalf;
    private _chimneySmoke: Smoke[] = [];
    private _smokeClouds: Smoke[] = [];
    private _people: Person[] = [];
    private _deadPeoplePositions: number[] = [];

    constructor() {
        this._chimneySmoke.push({
            x: 0,
            y: 0,
            frame: SmokeStartFrame,
            timer: 0
        });

        for (let i = 0; i < NumPeople; i++) {
            this._people[i] = {
                x: math.random(60, 176),
                frame: math.random(PersonStartFrame, PersonStartFrame + PersonNumFrames - 1),
                timer: math.random(0, 20),
                direction: i % 2 == 0 ? -1 : 1,
                speed: i % 2 == 0 ? 0.5 : 0.8
            }
        }
    }

    panic(): void {
        for (const person of this._people) {
            person.speed = 1.5;
        }
    }

    destroy(): void {
        this._tacoStandUpperHalfSprite = TacoStandUpperHalfDestroyed;

        for (let i = 0; i < NumSmokeForCloud; i++) {
            this._smokeClouds[i] = {
                x: math.random(15, 40),
                y: math.random(180, 190),
                frame: math.random(SmokeStartFrame, SmokeStartFrame + SmokeNumFrames - 1),
                timer: 0
            }
        }
        for (const person of this._people) {
            this._deadPeoplePositions.push(person.x);
        }

        this._people = [];
        this._chimneySmoke = [];
    }

    update(ticks: number): void {
        for (const chimneySmoke of this._chimneySmoke) {
            if (ticks % 12 == 6) {
                chimneySmoke.frame++;
                chimneySmoke.x = math.random(17, 24);

                if (chimneySmoke.frame > SmokeStartFrame + SmokeNumFrames)
                    chimneySmoke.frame = SmokeStartFrame;
            }
        }
        for (const smoke of this._smokeClouds) {
            if (ticks > smoke.timer) {
                smoke.x = math.random(15, 40);

                if (++smoke.frame > SmokeStartFrame + SmokeNumFrames)
                    smoke.frame = SmokeStartFrame;

                smoke.timer = ticks + SmokeAnimationTimer;
            }
        }
        for (const person of this._people) {
            person.x += person.direction * person.speed;

            if (ticks > person.timer) {
                if (person.x < 20 || person.x > 200)
                    person.direction *= -1;

                if (++person.frame > PersonStartFrame + PersonNumFrames)
                    person.frame = PersonStartFrame;

                person.timer = ticks + PersonAnimationTimer;
            }
        }
    }

    draw(): void {
        spr(TacoStandLowerHalf, 16, 208, 14, 1);
        spr(this._tacoStandUpperHalfSprite, 16, 176, 4, 2);

        for (const person of this._people) {
            spr(person.frame, person.x, 200, 1, 1, 1, person.direction == -1 ? 0 : 1);
        }
        for (const person of this._deadPeoplePositions) {
            spr(DeadPersonFrame, person, 204);
        }
        for (const smoke of this._smokeClouds) {
            spr(smoke.frame, smoke.x, smoke.y);
        }
    }
}
