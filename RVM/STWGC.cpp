#include "STWGC.hpp"

void STWGC::run(StackFrameManager& frames, std::shared_ptr<std::byte[]>& pool, stackType& poolPointer)
{
	size_t offset = 0;
	for (auto& frame : frames) {
		for (auto& variable : frame.variables) {
			auto objectType = reinterpret_cast<BaseObject*>(static_cast<std::byte*>(pool.get()) + variable)->getType();
			size_t objectSize = 0;
			switch (objectType) {
				case ObjectType::INT:
				case ObjectType::REF:
					objectSize = sizeof(Object<stackType>);
					break;
				case ObjectType::DOUBLE:
					objectSize = sizeof(Object<double>);
					break;
				default: throw GCError("Invalid object type");
			}
			std::memmove(static_cast<std::byte*>(pool.get()) + offset, static_cast<std::byte*>(pool.get()) + variable, objectSize);
			variable = offset;
			offset += objectSize;
		}
	}
	poolPointer = offset;
}
