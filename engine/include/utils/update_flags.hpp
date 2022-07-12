#pragma once

namespace Utils
{
    using UpdateFlag = unsigned int;
    static constexpr UpdateFlag UpdateFlag_Editing = 1 << 0;
    static constexpr UpdateFlag UpdateFlag_Gaming = 1 << 1;
}