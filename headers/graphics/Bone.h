#pragma once
#include <string>
#include <vector>

struct Bone {
	std::string name;
	size_t index;
	size_t parent;
	std::vector<size_t> children;
	Bone() {
		name = "";
		index = 0;
		parent = 0;
		children = std::vector<size_t>();
	};
};
