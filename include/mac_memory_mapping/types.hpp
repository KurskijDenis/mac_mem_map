#pragma once


namespace MMap
{

enum class MMapSync {
	Sync,
	Async,
	Invalidate,
};

enum class MMapFileType {
	R,
	W,
	RW,
	P,
};

enum class OpenFileMode {
	R,
	RW,
};

enum class AccessMode: int {
	None = 0,
	UR = 1,
	UW = 1 << 1,
	GR = 1 << 2,
	GW = 1 << 3,
	OR = 1 << 4,
	OW = 1 << 5,
};

} // namespace mmap
