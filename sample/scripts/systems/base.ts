import { Entity } from "../entity";

export function addEntity(list: Entity[], e: Entity) {
    list.push(e);
}

export function removeEntity(list: Entity[], e: Entity) {
    const i = list.findIndex(el => el == e);
    const last = list.pop();

    if (last != e)
        list[i] = <Entity>last;
}

export abstract class SystemBase {

    entities: Entity[] = [];

    abstract onEntityAdded(e: Entity): void;
    abstract onEntityRemoved(e: Entity): void;

    protected add(e: Entity): void {
        this.entities.push(e);
    }

    protected remove(e: Entity): void {
        const i = this.entities.findIndex(el => el == e);
        const last = this.entities.pop();

        if (last != e)
            this.entities[i] = <Entity>last;
    }
}
