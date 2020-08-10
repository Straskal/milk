import { Components } from "./components";

export enum EntityFlags {
    PLAYER      = 1 << 1,
    DAMAGEABLE  = 1 << 2
}

export class Entity {
    flags: number = 0;

    /** @ts-ignore because we only want typing for components. Every component does not need to exist on an entity. */
    components: Components = {};
}
