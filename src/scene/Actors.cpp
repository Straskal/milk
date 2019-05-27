#include "Actors.h"

#include <assert.h>

namespace milk {
	namespace {
		U32 makeId(
			std::vector<U16>& generations,
			std::queue<U16>& freeIndeces
		) {
			U16 index;
			if (freeIndeces.size() > Actors::MAX_FREE) {
				index = freeIndeces.front();
				freeIndeces.pop();
			}
			else {
				generations.push_back(0);
				index = generations.size() - 1;
				assert(index <= Actors::MAX);
			}
			U32 id = index | (generations[index] << Actors::GEN_BITS);
			if (id == Actors::INVALID) {
				++generations[index];
				id = index | (generations[index] << Actors::GEN_BITS);
			}
			return id;
		}

		void deleteId(
			U32 id,
			std::vector<U16>& generations,
			std::queue<U16>& freeIndeces,
			std::vector<U32> destroyed
		) {
			U16 index = id & ~(1 << Actors::GEN_BITS);
			assert(index < generations.size());
			++generations[index];
			freeIndeces.push(index);
			destroyed.push_back(id);
		}

		bool validId(
			U32 id,
			std::vector<U16>& generations
		) {
			U16 index = id & ~(1 << Actors::GEN_BITS);
			assert(index < generations.size());
			U16 generation = id >> Actors::IDX_BITS & ~(1 << Actors::GEN_BITS);
			return generations[index] == generation;
		}

		void insertName(
			U32 id,
			std::vector<std::string>& names,
			std::unordered_map<U32, int>& nameidmap,
			std::unordered_map<int, U32>& nameidxmap,
			const std::string& name
		) {
			names.push_back(name);
			int nameidx = names.size() - 1;
			nameidmap.insert(std::make_pair(id, nameidx));
			nameidxmap.insert(std::make_pair(nameidx, id));
		}

		void deleteName(
			U32 id,
			std::vector<std::string>& names,
			std::unordered_map<U32, int>& nameidmap,
			std::unordered_map<int, U32>& nameidxmap
		) {
			size_t lastidx = names.size() - 1;
			if (names.size() > 1) {
				// Swap the deleted element with last, then remove last element.
				U32 nameidx = nameidmap.at(id);
				names[nameidx] = names[lastidx];
				nameidmap.at(lastidx) = nameidx;
				nameidxmap.at(nameidx) = nameidxmap.at(lastidx);
			}
			names.pop_back();
			nameidmap.erase(id);
			nameidxmap.erase(lastidx);
		}

		void updateName(
			U32 id,
			std::vector<std::string>& names,
			std::unordered_map<U32, int>& nameidmap,
			const std::string& name
		) {
			int nameidx = nameidmap.at(id);
			names[nameidx] = name;
		}

		std::string queryNamesById(
			U32 id,
			std::vector<std::string>& names,
			std::unordered_map<U32, int>& nameidmap
		) {
			int nameidx = nameidmap.at(id);
			return names[nameidx];
		}

		void insertPosition(
			U32 id,
			std::unordered_map<U32, Vector2>& positions,
			Vector2 position
		) {
			positions.insert(std::make_pair(id, position));
		}

		void updatePosition(
			U32 id,
			std::unordered_map<U32, Vector2>& positions,
			Vector2 position
		) {
			positions.at(id) = position;
		}

		Vector2 queryPositionsById(
			U32 id,
			std::unordered_map<U32, Vector2>& positions
		) {
			return positions.at(id);
		}

		void insertOrUpdateTag(
			U32 id,
			std::unordered_map<U32, U32>& tags,
			U32 mask
		) {
			std::unordered_map<U32, U32>::iterator tag = tags.find(id);
			if (tag != tags.end()) {
				tag->second = mask;
			}
			else {
				tags.insert(std::make_pair(id, mask));
			}
		}

		U32 queryTagsById(
			U32 id,
			std::unordered_map<U32, U32>& tags
		) {
			std::unordered_map<U32, U32>::iterator tag = tags.find(id);
			return tag == tags.end() ? 0 : tag->second;
		}
	}
}

const int milk::Actors::MAX = 20000;
const int milk::Actors::MAX_FREE = 1024;
const milk::U32 milk::Actors::IDX_BITS = 16;
const milk::U32 milk::Actors::GEN_BITS = 16;
const milk::U32 milk::Actors::INVALID = 0;

milk::U32 milk::Actors::createActor(const std::string& name, Vector2 position) {
	U32 id = makeId(generations_, freeIndeces_);
	insertName(id, names_, nameidmap_, nameidxmap_, name);
	insertPosition(id, positions_, position);
	return id;
}

void milk::Actors::destroyActor(U32 id) {
	if (!validId(id, generations_)) {
		// TODO: Trying to access destroyed actor. Log warning.
	}
	deleteId(id, generations_, freeIndeces_, destroyed_);
	deleteName(id, names_, nameidmap_, nameidxmap_);
}

bool milk::Actors::isActorAlive(U32 id) {
	return validId(id, generations_);
}

std::string milk::Actors::getActorName(U32 id) {
	if (!validId(id, generations_)) {
		// TODO: Trying to access destroyed actor. Log warning.
	}
	return queryNamesById(id, names_, nameidmap_);
}

void milk::Actors::setActorName(U32 id, const std::string& name) {
	if (!validId(id, generations_)) {
		// TODO: Trying to access destroyed actor. Log warning.
	}
	updateName(id, names_, nameidmap_, name);
}

milk::Vector2 milk::Actors::getActorPosition(U32 id) {
	if (!validId(id, generations_)) {
		// TODO: Trying to access destroyed actor. Log warning.
	}
	return queryPositionsById(id, positions_);
}

void milk::Actors::setActorPosition(U32 id, Vector2 position) {
	if (!validId(id, generations_)) {
		// TODO: Trying to access destroyed actor. Log warning.
	}
	updatePosition(id, positions_, position);
}

milk::U32 milk::Actors::getActorTags(U32 id) {
	if (!validId(id, generations_)) {
		// TODO: Trying to access destroyed actor. Log warning.
	}
	return queryTagsById(id, tags_);
}

void milk::Actors::setActorTags(U32 id, U32 mask) {
	if (!validId(id, generations_)) {
		// TODO: Trying to access destroyed actor. Log warning.
	}
	insertOrUpdateTag(id, tags_, mask);
}
