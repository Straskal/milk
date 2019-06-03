#ifndef MILK_NODE_TEMPLATE_LUA
#define MILK_NODE_TEMPLATE_LUA

#include <string>
#include <vector>

#include "math/Vector2.h"

namespace milk {
	struct ChildNodeTemplate {
		std::vector<ChildNodeTemplate> children;
		Vector2 position;
		std::string name;
		std::string script;
	};

	struct NodeTemplate {
		std::vector<ChildNodeTemplate> children;
		std::string script;
	};
}

#endif