#pragma once

#include "BaseGC.hpp"
#include "logging.hpp"

class EpsilonGC : public BaseGC {
public:
	EpsilonGC() = default;

	void run(StackFrameManager&, std::shared_ptr<std::byte[]>&, stackType&) override;
};
