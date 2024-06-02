
#pragma once
#include "ECS.hpp"
#include "utils.hpp"
#include "EntityFactory.hpp"

namespace generation {

	struct Dimensions {
		int right = 0;
		int top = 0;
		int left = 0;
		int bottom = 0;
	};

	struct Layout {
		pair offset;
		Dimensions dimensions;
		std::vector<std::unique_ptr<Layout>> children;

		virtual void generate(RealmId realmId, pair position) {
			for (auto& l : children) l->generate(realmId, position + offset);
		};
	};

	struct ResourceLayout : public Layout {
		ResourceId::value resourceId;
		ResourceLayout(ResourceId::value resourceId) : resourceId(resourceId) {}

		void generate(RealmId realmId, pair position) override {
			EntityFactory::createResource(resourceId, realmId, position + offset);
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
				layout->children.push_back(std::move(l));
			}
			return layout;
		}
	};
}