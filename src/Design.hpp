
#pragma once
#include "utils.hpp"
#include "Realm.hpp"
// #include "definitions.hpp"

ENUM(DesignId,
	TREE	
)

namespace design {

struct Node {
	virtual bool generate(uint seed, Realm* realm, pair position) = 0;
	virtual ~Node() = default;
	std::vector<std::unique_ptr<Node>> children = {};
	static std::array<std::unique_ptr<Node>, DesignId::count> templates;
	static void setTemplates(std::filesystem::path root);
};

struct Refrence : public Node {
	DesignId::value designId;
	Refrence(DesignId::value designId) : designId(designId) {}

	bool generate(uint seed, Realm* realm, pair position) override;
};

//* Flow

struct Sequence : public Node {
	bool generate(uint seed, Realm* realm, pair position) override;
};

struct Repeat : public Node {
	uint number;
	Repeat(uint num) : number(num) {}
	bool generate(uint seed, Realm* realm, pair position) override;
};

struct Choice : public Node {
	uint number;
	bool repeat;
	Choice(uint num = 1, bool rep = false) : number(num), repeat(rep) {}
	bool generate(uint seed, Realm* realm, pair position) override;
};

//* Modifier

struct Offset : public Node {
	pair xr, yr;
	Offset(pair xr, pair yr) : xr(xr), yr(yr) {}
	Offset(int x, int y) : xr(x, x + 1), yr(y, y + 1) {}

	bool generate(uint seed, Realm* realm, pair position) override;
};

struct Spread : public Node {
	vec radr, angr;
	Spread(vec radr, vec angr = vec(0, 2*M_PI)) : radr(radr), angr(angr) {}
	Spread(float r, vec angr = vec(0, 2*M_PI)) : radr(r, r), angr(angr) {}

	bool generate(uint seed, Realm* realm, pair position) override;
};

//* Leaf

struct Place : public Node {
	std::variant<ResourceId::value, StructureId::value, StationId::value> id;
	Place(ResourceId::value id) : id(id) {}
	Place(StructureId::value id) : id(id) {}
	Place(StationId::value id) : id(id) {}

	bool generate(uint seed, Realm* realm, pair position) override;
};


	
} // namespace design

