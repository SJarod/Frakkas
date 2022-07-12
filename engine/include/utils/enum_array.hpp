#pragma once

#include <array>

// class found here : https://stackoverflow.com/questions/35936571/enum-class-as-array-index

/**
 * @brief An EnumArray is useful to create static std::array and use int-enum-class as index.
 * @tparam CEnum The enum class to use as index.
 * @tparam TType The array type.
 * @tparam SSize The array size. Define a MAX_VALUE into your enum class and send it as size.
 */
template<class CEnum, typename TType, size_t SSize>
class EnumArray
{
public:
	TType& operator[](CEnum e)
	{
		return arr[static_cast<int>(e)];
	}

	const TType& operator[](CEnum e) const
	{
		return arr[static_cast<int>(e)];
	}

    /**
     * @return The size of the array, never changed.
     */
	[[nodiscard]] int size() const { return SSize; }

private:

	std::array<TType, SSize> arr;
};