#ifndef MILK_IDS_H
#define MILK_IDS_H

#include <assert.h>
#include <string>
#include <queue>
#include <vector>

#include "data/int.h"

namespace milk {
	class Ids {
	public:
		const static U32 INVALID{ 0 };

		const U8 IDX_BITS;
		const U8 GEN_BITS;
		const U16 MAX;
		const U16 MAX_FREE;

		Ids();
		Ids(U8 idxbits, U8 genbits, U16 max, U16 maxFree);

		U32 create();
		void remove(U32 id);
		bool valid(U32 id) const;

	private:
		std::queue<U16> freeIndeces_{};
		std::vector<U16> generations_{};
	};
}

#endif