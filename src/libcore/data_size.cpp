#include <fstore/core.h>

#include <array>
#include <cmath>
#include <iomanip>

namespace fstore::core {
    static constexpr unsigned int unit_max = 1024;
    static constexpr int precision = 3;
    static const std::array<byte_multiple, 5> multiples {
        B, KiB, MiB, GiB, TiB
    };

    data_size data_size::convert_to(long double size, byte_multiple multiple) {
        auto i = 0u;

        while (size >= unit_max && multiples[i] != multiple) {
            size /= unit_max;
            i++;
        }

        return data_size(size, multiples[i]);
    }

    data_size::data_size(long double size, byte_multiple multiple) :
        size(size),
        multiple(multiple)
    {}

    data_size data_size::format(uintmax_t bytes) {
        return convert_to(bytes, multiples[multiples.size() - 1]);
    }

    std::ostream& operator<<(std::ostream& os, byte_multiple multiple) {
        switch (multiple) {
            case   B: os <<   "B"; break;
            case KiB: os << "KiB"; break;
            case MiB: os << "MiB"; break;
            case GiB: os << "GiB"; break;
            case TiB: os << "TiB"; break;
        }

        return os;
    }

    std::ostream& operator<<(std::ostream& os, const data_size& ds) {
        os
            << std::setprecision(precision) << ds.size
            << " "
            << ds.multiple;

        return os;
    }

    std::string to_string(const data_size& ds) {
        std::ostringstream os;
        os << ds;
        return os.str();
    }
}
