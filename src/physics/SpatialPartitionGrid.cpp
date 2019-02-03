#include "SpatialPartitionGrid.h"

#include <iostream>

#include "BoxCollider.h"

#include "scene/Actor.h"

milk::SpatialPartitionGrid::SpatialPartitionGrid()
{
    // Initialize the grid.
    for (auto& cell : cells_)
    {
        for (auto& y : cell)
        {
            y = nullptr;
        }
    }
}

void milk::SpatialPartitionGrid::add(BoxCollider* collider)
{
    // Determine which grid cell it's in.
    int cellX = collider->rect().x / SpatialPartitionGrid::CELL_SIZE;
    int cellY = collider->rect().y / SpatialPartitionGrid::CELL_SIZE;

    // Add to the front of list for the cell it's in.
    collider->prev_ = nullptr;
    collider->next_ = cells_[cellX][cellY];

    cells_[cellX][cellY] = collider;

    if (collider->next_ != nullptr)
        collider->next_->prev_ = collider;
}

void milk::SpatialPartitionGrid::remove(BoxCollider* collider)
{
    int cellX = collider->oldRect_.x / SpatialPartitionGrid::CELL_SIZE;
    int cellY = collider->oldRect_.y / SpatialPartitionGrid::CELL_SIZE;

    // Unlink it from the list of its old cell.
    if (collider->prev_ != nullptr)
        collider->prev_->next_ = collider->next_;

    if (collider->next_ != nullptr)
        collider->next_->prev_ = collider->prev_;

    // If it's the head of a list, remove it.
    if (cells_[cellX][cellY] == collider)
        cells_[cellX][cellY] = collider->next_;
}

void milk::SpatialPartitionGrid::move(BoxCollider* collider)
{
    int oldCellX = collider->oldRect_.x / SpatialPartitionGrid::CELL_SIZE;
    int oldCellY = collider->oldRect_.y / SpatialPartitionGrid::CELL_SIZE;

    int cellX = collider->rect_.x / SpatialPartitionGrid::CELL_SIZE;
    int cellY = collider->rect_.y / SpatialPartitionGrid::CELL_SIZE;

    // If we didn't move cells, then return early.
    if (oldCellX == cellX && oldCellY == cellY)
        return;

    // Remove it from grid.
    remove(collider);

    // Add it back to the grid at its new cell.
    add(collider);
}

std::vector<milk::Collision> milk::SpatialPartitionGrid::getCollisions(BoxCollider* collider)
{
    std::vector<Collision> collisions;

    int cellX = collider->rect().x / SpatialPartitionGrid::CELL_SIZE;
    int cellY = collider->rect().y / SpatialPartitionGrid::CELL_SIZE;

    // Get collisions for current cell.
    getCollisionForCell(collider, cells_[cellX][cellY], &collisions);

    // If there is a neighboring cell, check it.

    // Upper left cell
    if (cellX > 0 && cellY > 0)
        getCollisionForCell(collider, cells_[cellX - 1][cellY - 1], &collisions);

    // Left cell
    if (cellX > 0)
        getCollisionForCell(collider, cells_[cellX - 1][cellY], &collisions);

    // Upper cell
    if (cellY > 0)
        getCollisionForCell(collider, cells_[cellX][cellY - 1], &collisions);

    // Bottom left cell
    if (cellX > 0 && cellY < NUM_CELLS - 1)
        getCollisionForCell(collider, cells_[cellX - 1][cellY + 1], &collisions);

    // Upper right cell
    if (cellX < NUM_CELLS - 1 && cellY > 0)
        getCollisionForCell(collider, cells_[cellX + 1][cellY - 1], &collisions);

    // Right cell
    if (cellX < NUM_CELLS - 1)
        getCollisionForCell(collider, cells_[cellX + 1][cellY], &collisions);

    // Bottom cell
    if (cellY < NUM_CELLS - 1)
        getCollisionForCell(collider, cells_[cellX][cellY + 1], &collisions);

    // Bottom right cell
    if (cellX < NUM_CELLS - 1 && cellY < NUM_CELLS - 1)
        getCollisionForCell(collider, cells_[cellX + 1][cellY + 1], &collisions);

    return collisions;
}

void milk::SpatialPartitionGrid::getCollisionForCell(BoxCollider* collider, BoxCollider* cell,
                                                     std::vector<Collision>* collisions)
{
    // TODO dont keep recalculating
    int oldTop = collider->oldRect_.y;
    int oldBottom = collider->oldRect_.y + collider->oldRect_.height;
    int oldLeft = collider->oldRect_.x;
    int oldRight = collider->oldRect_.x + collider->oldRect_.width;

    while (cell != nullptr)
    {
        if (collider != cell)
        {
            Rectangle intersectionDepth;

            if (collider->overlaps(cell->rect(), &intersectionDepth))
            {
                CollisionSide dir = CollisionSide::BOTTOM;

                if (oldRight <= cell->left() && collider->right() >= cell->left())
                    dir = CollisionSide::RIGHT;
                else if (oldLeft >= cell->right() && collider->left() < cell->right())
                    dir = CollisionSide::LEFT;
                else if (oldBottom < cell->top() && collider->bottom() >= cell->top())
                    dir = CollisionSide::BOTTOM;
                else if (oldTop >= cell->bottom() && collider->top() < cell->bottom())
                    dir = CollisionSide::TOP;

                collisions->emplace_back(*collider, *cell, dir, intersectionDepth);
            }
        }

        cell = cell->next_;
    }
}

void milk::SpatialPartitionGrid::clear()
{
    // Remove all colliders.
    for (auto& cell : cells_)
    {
        for (auto& y : cell)
        {
            y = nullptr;
        }
    }
}
