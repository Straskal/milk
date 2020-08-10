import { Entity } from "../entity";

export abstract class SystemBase {

    entities: Entity[] = [];

    abstract onEntityAdded(e: Entity): void;
    abstract onEntityRemoved(e: Entity): void;

    protected add(e: Entity): void {
        const i = this.entities.push(e);
    }

    protected remove(e: Entity): void {
        const i = this.entities.findIndex(el => el == e);
        const last = this.entities.pop();

        if (last != e)
            this.entities[i] = <Entity>last;
    }
}
