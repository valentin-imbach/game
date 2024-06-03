
#pragma once
#include "ECS.hpp"
#include "utils.hpp"
#include "EntityFactory.hpp"
#include "ResourceTemplates.hpp"
#include "StructureTemplates.hpp"

namespace generation {

	struct Shadow {
		std::set<pair> set;

		Shadow() {}

		Shadow(pair size) {
			for (int x = 0; x < size.x; x++) {
				for (int y = 0; y < size.y; y++) {
					pair p(x, y);
					set.insert(p);
				}
			}
		}

		void add(Shadow& other, pair offset) {
			for (auto p: other.set) {
				pair np = p + offset;
				if (set.find(np) == set.end()) {
					set.insert(np);
				} else {
					WARNING("Adding overlapping Shadows");
				}
			}
		}
	};

	struct Layout {
		pair offset;
		Shadow shadow;
		std::vector<std::unique_ptr<Layout>> children;

		virtual void generate(RealmId realmId, pair position) {
			for (auto& l : children) l->generate(realmId, position + offset);
		};

		void add(std::unique_ptr<Layout> layout) {
			shadow.add(layout->shadow, layout->offset);
			children.push_back(std::move(layout));
		}
	};

	struct ResourceLayout : public Layout {
		ResourceId::value resourceId;
		ResourceLayout(ResourceId::value resourceId) : resourceId(resourceId) {
			pair size = ResourceTemplate::templates[resourceId].size;
			shadow = Shadow(size);
		}

		void generate(RealmId realmId, pair position) override {
			EntityFactory::createResource(resourceId, realmId, position + offset);
		}
	};

	struct StructureLayout : public Layout {
		StructureId::value structureId;
		StructureLayout(StructureId::value structureId) : structureId(structureId) {
			pair size = StructureTemplate::templates[structureId].size;
			shadow = Shadow(size);
		}

		void generate(RealmId realmId, pair position) override {
			EntityFactory::createStructure(structureId, realmId, position + offset);
		}
	};


	struct Node {
		virtual std::unique_ptr<Layout> build() = 0;
	};

	struct ResourceNode : public Node {
		ResourceId::value resourceId;
		ResourceNode(ResourceId::value resourceId) : resourceId(resourceId) {}

		std::unique_ptr<Layout> build() override {
			return std::make_unique<ResourceLayout>(resourceId);
		}
	};

	struct StructureNode : public Node {
		StructureId::value structureId;
		StructureNode(StructureId::value structureId) : structureId(structureId) {}

		std::unique_ptr<Layout> build() override {
			return std::make_unique<StructureLayout>(structureId);
		}
	};

	struct SurroundNode : public Node {
		Node* centre;
		Node* surround;
		uchar distance;
		uchar number;

		SurroundNode(Node* center, Node* surround, uchar number, uchar distance) : centre(center), surround(surround), number(number), distance(distance) {}

		std::unique_ptr<Layout> build() override {
			auto layout = std::make_unique<Layout>();
			layout->children.push_back(centre->build());
			for (int i = 0; i < number; i++) {
				float angle = i * 2 * M_PI / number;
				auto l = surround->build();
				l->offset = vec::round(vec::polar(angle, distance));
				layout->add(std::move(l));
			}
			return layout;
		}
	};
}