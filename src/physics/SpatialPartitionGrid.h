// http://gameprogrammingpatterns.com/spatial-partition.html

#ifndef MILK_SPATIAL_PARTITION_GRID_H
#define MILK_SPATIAL_PARTITION_GRID_H

#include <vector>

#include "Collision.h"

namespace milk
{
    class BoxCollider;

    // The spatial partition grid divides the scene into cells.
    // Each cell contains collidable actors.
    // When checking for collisions with a given collider, it only queries the current and neighboring cells.
    class SpatialPartitionGrid
    {
    public:
        SpatialPartitionGrid();

        // Adds a collider to the collision grid.
        void add(BoxCollider* collider);

        // Removes a collider from the collision grid.
        void remove(BoxCollider* collider);

        // Removes a collider from the collision grid.
        void move(BoxCollider* collider);

        // Get all colliders than intersect with a given collider.
        std::vector<Collision> getCollisions(BoxCollider* collider);

        // Clear all colliders from grid.
        void clear();

        // TODO make these data driven
        // TODO NUM_HORIZONTAL_CELLS & NUM_VERTICAL_CELLS
        static const int NUM_CELLS = 4;
        static const int CELL_SIZE = 160;
    private:
        BoxCollider* cells_[NUM_CELLS][NUM_CELLS];

        void getCollisionForCell(BoxCollider* collider, BoxCollider* cell, std::vector<Collision>* collisions);
    };
}

#endif
