export enum EntityFlags {
    PLAYER = 1 << 1,
    DAMAGEABLE = 1 << 2
}

export class Entity {
    flags: number = 0;
    components: any[] = [];

    getComponentOfType<T>(TCtor: new (...args: any[]) => T): T | null {
        for (let b of this.components) {
            if (b instanceof TCtor) {
                return b as T;
            }
        }
        return null;
    }
}
