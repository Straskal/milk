#ifndef MILK_BOX_COLLIDER_H
#define MILK_BOX_COLLIDER_H

#include "scene/ActorComponent.h"

#include "math/Rectangle.h"
#include "math/Vector2d.h"

#include "utilities/Alignment.h"

namespace milk
{
    class SpatialPartitionGrid;

    class BoxCollider : public ActorComponent
    {
    public:
        static const ComponentType type;

        friend class SpatialPartitionGrid;

        /// Actors with a box collider will be static collidable objects.
        /// Actors with a box collider and velocity will be dynamic collidable objects.
        /// \param actor: The BoxCollider's Actor
        explicit BoxCollider(Actor& actor);

        ~BoxCollider() override = default;

        /// Initializes the collider so it can become one with the collision grid.
        /// \param grid: The SpatialPartitionGrid
        void init(SpatialPartitionGrid* grid);

        /// Center the collider's origin.
        void center();

        /// Updates the bounding box. If the actor's position is changed, this method must be called in order to update the collider's bounding box.
        void updateBBox();

        /// \returns the BoxCollider's bounding box.
        Rectangle rect() const;

        /// Set the width of the collider.
        /// \param width
        void width(int width);

        /// Set the height of the collider.
        /// \param height
        void height(int height);

        /// Offsets the collider
        /// \param xOffset
        /// \param yOffset
        void offset(int xOffset, int yOffset);

        /// \returns true if collider's bounding box is overlapping another collider's bounding box.
        bool overlaps(Rectangle otherRect);

        /// \returns true if collider's bounding box is overlapping another collider's bounding box, and outputs the depth rect.
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

        Vector2d offset_;

        SpatialPartitionGrid* grid_;

        BoxCollider* prev_;
        BoxCollider* next_;
    };
}

#endif
