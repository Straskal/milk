import { EntityManager } from "./entityManager";
import { PlayerSystem } from "./systems/player";
import { AnimationSystem } from "./systems/animation";
import { DrawSystem } from "./systems/render";
import { newWorld } from "./libs/bump";

export class Level {

    bumpWorld = newWorld(16);
    entityManager = new EntityManager();
    player = new PlayerSystem();
    anim = new AnimationSystem();
    draw = new DrawSystem();
}
