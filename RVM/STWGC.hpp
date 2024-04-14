#pragma once

#include <algorithm>
#include <cstring>

#include "BaseGC.hpp"

class STWGC : public BaseGC {
public:
	STWGC() = default;

	void run(StackFrameManager&, std::shared_ptr<std::byte[]>&, stackType&) override;
};
