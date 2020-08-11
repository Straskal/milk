export class AssetManager {

    static loaded = new Map<string, any>();

    static loadBitmap(path: string): Bitmap {
        return AssetManager.load(path, () => bitmap(path));
    }

    static unload(): void {
        AssetManager.loaded = new Map<string, any>();
        collectgarbage();
        collectgarbage();
    }

    private static load<T>(path: string, loaderFunction: (p: string) => T): T {
        if (!AssetManager.loaded.has(path))
            AssetManager.loaded.set(path, loaderFunction(path));

        return AssetManager.loaded.get(path);
    }
}
