#include "Actors.h"

#include <assert.h>

milk::U32 milk::Actors::createActor(const std::string& name) {
	U32 index;
	if (freeIndeces_.size() > MAX_FREE) {
		index = freeIndeces_.front();
		freeIndeces_.pop();
	}
	else {
		generations_.push_back(0);
		index = generations_.size() - 1;
		assert(index <= MAX);
	}
	U32 id = index | (generations_[index] << GEN_BITS);
	if (id == INVALID) {
		++generations_[index];
		id = index | (generations_[index] << GEN_BITS);
	}

	names_.push_back(name);
	int nameidx = names_.size() - 1;
	nameidmap_.insert(std::make_pair(id, nameidx));
	nameidxmap_.insert(std::make_pair(nameidx, id));

	positions_.insert(std::make_pair(id, Vector2::zero()));
	return id;
}

void milk::Actors::destroyActor(U32 id) {
	U16 index = id & ~(1 << GEN_BITS);
	assert(index < generations_.size());
	++generations_[index];
	freeIndeces_.push(index);

	U32 lastidx = names_.size() - 1;

	if (names_.size() > 1) {
		// Swap the deleted element with last, then remove last element.
		U32 nameidx = nameidmap_.at(id);
		names_[nameidx] = names_[lastidx];
		nameidmap_.at(lastidx) = nameidx;
		nameidxmap_.at(nameidx) = nameidxmap_.at(lastidx);
	}

	names_.pop_back();
	nameidmap_.erase(id);
	nameidxmap_.erase(lastidx);
	destroyed_.push_back(id);
}

bool milk::Actors::isActorAlive(U32 id) {
	U16 index = id & ~(1 << GEN_BITS);
	assert(index < generations_.size());
	U16 generation = id >> IDX_BITS & ~(1 << GEN_BITS);
	return generations_[index] == generation;
}

std::string milk::Actors::getActorName(U32 id) {
	U32 nameidx = nameidmap_.at(id);
	return names_[nameidx];
}

void milk::Actors::setActorName(U32 id, const std::string& name) {
	U32 nameidx = nameidmap_.at(id);
	names_[nameidx] = name;
}

milk::Vector2 milk::Actors::getActorPosition(U32 id) {
	return positions_.at(id);
}

void milk::Actors::setActorPosition(U32 id, Vector2 position) {
	positions_.at(id) = position;
}

milk::U32 milk::Actors::getActorTags(U32 id) {
	std::unordered_map<U32, U32>::iterator tag = tags_.find(id);
	return tag == tags_.end() ? 0 : tag->second;
}

void milk::Actors::setActorTags(U32 id, U32 tags) {
	std::unordered_map<U32, U32>::iterator tag = tags_.find(id);
	if (tag != tags_.end()) {
		tag->second = tags;
	}
	else {
		tags_.insert(std::make_pair(id, tags));
	}
}
