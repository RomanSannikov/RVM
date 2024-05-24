#pragma once

#include "exceptions.hpp"
#include "object.hpp"
#include "StackFrameManager.hpp"
#include "logging.hpp"

class BaseGC {
public:
	BaseGC() = default;

	virtual void run(StackFrameManager&, std::shared_ptr<std::byte[]>&, stackType&) = 0;
};
