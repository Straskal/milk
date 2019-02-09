#ifndef MILK_SPATIAL_PARTITION_GRID_H
#define MILK_SPATIAL_PARTITION_GRID_H

#include <vector>

#include "Collision.h"

namespace milk
{
    class BoxCollider;

    /// Efficiently locate objects by storing them in a data structure organized by their positions.
    /// http://gameprogrammingpatterns.com/spatial-partition.html
    class SpatialPartitionGrid
    {
    public:
        SpatialPartitionGrid();

        /// Adds the given collider to the collision grid.
        /// \param: The BoxCollider to add to the grid
        void add(BoxCollider* collider);

        /// Removes the given collider from the collision grid.
        /// \param: The BoxCollider to remove from the grid
        void remove(BoxCollider* collider);

        /// Updates the given BoxCollider's position in the grid.
        /// \param: The BoxCollider to update
        void move(BoxCollider* collider);

        // TODO: This should NOT be the responsibility of the grid. Physics system should take care of this by querying the grid.
        /// \returns all colliders that intersect with the given collider.
        std::vector<Collision> getCollisions(BoxCollider* collider);

        /// Clear all colliders from grid.
        void clear();

        // TODO: Make these static value data driven and constructor injected.
        static const int NUM_CELLS = 4;
        static const int CELL_SIZE = 160;
    private:
        BoxCollider* cells_[NUM_CELLS][NUM_CELLS];

        void getCollisionForCell(BoxCollider* collider, BoxCollider* cell, std::vector<Collision>* collisions);
    };
}

#endif
