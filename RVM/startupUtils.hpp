struct programOptions
{
	enum class programSubmodule
	{
		NONE = 0,
		EXECUTOR,
		CONVERTOR
	};

	enum class submoduleMode
	{
		NONE = 0,
		BINARY
	};

	enum class helpModes
	{
		NONE = 0,
		BYTECODE,
		GENERAL
	};

	enum class gcType
	{
		EPSILON = 0,
		STW
	};

	programSubmodule submodule = programSubmodule::NONE;
	submoduleMode submoduleMode = submoduleMode::NONE;
	gcType gcType = gcType::STW;
};
