#ifndef METACPP_SEQUENTIAL_CONTAINER_HPP
#define METACPP_SEQUENTIAL_CONTAINER_HPP

#include <cassert>

#include "QualifiedType.hpp"
#include "Container.hpp"

namespace metacpp {
	class SequentialContainer : public Container {
	public:
		virtual QualifiedType ValuesType() const = 0;
		virtual size_t Size(void* container) const = 0;
		virtual void* At(void* container, size_t index) const = 0;
		virtual void PushBack(void* container, void* item_ptr) const = 0;
	};
}

#endif