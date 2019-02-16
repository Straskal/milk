#ifndef MILK_BOX_COLLIDER_H
#define MILK_BOX_COLLIDER_H

#include "scene/ActorComponent.h"

#include "math/Rectangle.h"
#include "math/Vector2.h"

#include "utilities/Alignment.h"

namespace milk
{
    class SpatialPartitionGrid;

    /// Actors with a BoxCollider are static collidable objects.
    /// Actors with a BoxCollider and Velocity will be dynamic collidable objects.
    class BoxCollider : public ActorComponent
    {
    public:
        static const ComponentType type;

        /// Updates the bounding box. If the Actor's position has changed, this method must be called in order to update the bounding box.
        void updateBBox();

        /// \returns the BoxCollider's bounding box.
        Rectangle rect() const;
        friend class SpatialPartitionGrid;

        /// \param actor: The BoxCollider's Actor
        explicit BoxCollider(Actor& actor, int width, int height);

        /// Initializes the BoxCollider so it can become one with the collision grid.
        /// \param grid: The SpatialPartitionGrid
        void init(SpatialPartitionGrid* grid);

        /// Center the collider's origin.
        void center();

        /// Offsets the collider
        /// \param xOffset
        /// \param yOffset
        void offset(int xOffset, int yOffset);

        /// \returns true if BoxCollider's bounding box is overlapping another collider's bounding box.
        bool overlaps(Rectangle otherRect);

        /// \returns true if BoxCollider's bounding box is overlapping another collider's bounding box, and outputs the depth rect.
        bool overlaps(Rectangle otherRect, Rectangle* result);

        /// Top of collider.
        int top();

        /// Bottom of collider.
        int bottom();

        /// Left of collider.
        int left();

        /// Right of collider;
        int right();

    private:
        Alignment origin_;

        Rectangle rect_;
        Rectangle oldRect_;

        Vector2 offset_;

        SpatialPartitionGrid* grid_;

        // TODO: Remove these. They exist because of the collision grid implementation, which needs to be changed.
        BoxCollider* prev_;
        BoxCollider* next_;
    };
}

#endif
