#ifndef MILK_TRANFORMS_H
#define MILK_TRANFORMS_H

#include <array>
#include <vector>

#include "Actors.h"
#include "common/int.h"
#include "math/Matrix3.h"
#include "math/Vector2.h"

namespace milk 
{
	struct Transform 
	{
		U32 actorId = 0;
	};

	struct Transforms
	{
		std::vector<Transform*> actorIdMap{};
		std::vector<Vector2> localPositions{};
		std::vector<Vector2> worldPositions{};
		std::vector<Matrix3> matrices{};
		std::vector<int> parents{};
		std::vector<std::vector<int>> children{};

		void add(Actor actor);
		void remove(Actor actor);
		Transform get(Actor actor);
	};
}

#endif