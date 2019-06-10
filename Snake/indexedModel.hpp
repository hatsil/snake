#pragma once

#include <vector>
#include <iterator>
#include <functional>

#include <glm/glm.hpp>

typedef unsigned int uint;

namespace snake {
	struct Weight {
		float prev, cur, next;
		Weight(float prev, float cur, float next) : prev(prev), cur(cur), next(next) {}
	};

	struct IndexedModel {
		bool isBox = false;
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> colors;
		std::vector<Weight> weights;
		std::vector<uint> indices;

		inline void Clear() {
			positions.clear();
			texCoords.clear();
			normals.clear();
			colors.clear();
			weights.clear();
			indices.clear();
		}

		inline void GetPrevs(std::vector<float>& prevs) const {
			GetWeightFields(prevs, [](const Weight& weight) -> const float& {
				return weight.prev;
			});
		}

		inline void GetCurs(std::vector<float>& curs) const {
			GetWeightFields(curs, [](const Weight& weight) -> const float& {
				return weight.cur;
			});
		}

		inline void GetNexts(std::vector<float>& nexts) const {
			GetWeightFields(nexts, [](const Weight& weight) -> const float& {
				return weight.next;
			});
		}

	private:
		inline void GetWeightFields(std::vector<float>& fields, const std::function<const float&(const Weight&)>& selector) const {
			class MyIterator : public std::iterator<std::input_iterator_tag, float> {
				std::vector<Weight>::const_iterator it;
				const std::function<const float&(const Weight&)>& selector;
			public:
				MyIterator(const std::vector<Weight>::const_iterator& it,
					const std::function<const float&(const Weight&)>& selector) : it(it), selector(selector) {}
				MyIterator(const MyIterator& other) : MyIterator(other.it, other.selector) {}

				MyIterator& operator++() { ++it; return *this; }
				MyIterator operator++(int) { MyIterator tmp(*this); operator++(); return tmp; }
				bool operator==(const MyIterator& rhs) const { return it == rhs.it; }
				bool operator!=(const MyIterator& rhs) const { return it != rhs.it; }
				const float& operator*() const { return selector(*it); }
			};

			MyIterator first(weights.cbegin(), selector);
			MyIterator last(weights.cend(), selector);
			fields.assign(first, last);
		}
	};
}