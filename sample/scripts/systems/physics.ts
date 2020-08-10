import { SystemBase } from "./base";
import { Entity } from "../entity";

export class PhysicsSystem extends SystemBase {

    solids: Entity[] = [];
    dynamics: Entity[] = [];
    moving: Entity[] = [];

    onEntityAdded(e: Entity): void {
    }
    onEntityRemoved(e: Entity): void {
    }
    update(_: number): void {
    }
}
