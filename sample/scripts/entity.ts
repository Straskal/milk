import { Components } from "./components";

export enum EntityFlags {
    PLAYER      = 1 << 1,
    DAMAGEABLE  = 1 << 2
}

export class Entity {
    flags: number = 0;
    components: Components = {};
}
