#include "Memory.h"
#include <Psapi.h>
#include <Windows.h>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>

#pragma comment(lib, "Psapi.lib")

namespace {
const char* hex = "0123456789ABCDEF";

bool Compare(const std::vector<unsigned char>& bytes, const std::string& mask,
    const std::uintptr_t start)
{
    for (auto i = 0; i < bytes.size(); i++) {
        if (*reinterpret_cast<unsigned char*>(start + i) != bytes[i] && mask[i] == 'x') {
            return false;
        }
    }
    return true;
}

std::vector<std::string> Split(const std::string& str)
{
    std::istringstream buf(str);
    std::istream_iterator<std::string> beg(buf), end;

    return std::vector<std::string>(beg, end);
}
} // namespace

namespace Memory {
std::uintptr_t ResolveRelativeCall(const std::uintptr_t loc)
{
    return *reinterpret_cast<std::uintptr_t*>(loc + 1) + loc + 5;
}

std::uintptr_t Scan(const std::uintptr_t start, std::uintptr_t end,
    const std::string& pattern, const int flags,
    const int offset)
{
    std::vector<unsigned char> bytes;
    std::string mask;

    if (end == 0) {
        MODULEINFO mi;
        GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &mi,
            sizeof(MODULEINFO));
        end = reinterpret_cast<std::uintptr_t>(mi.lpBaseOfDll) + mi.SizeOfImage;
        if (flags & SCAN_REVERSE) {
            end -= mi.SizeOfImage;
        }
    }

    for (const auto& str : Split(pattern)) {
        if (str == "?") {
            bytes.push_back(0);
            mask += "?";
        } else if (str.length() == 2) {
            const auto low1 = std::lower_bound(hex, hex + 16, str[0]);
            const auto low2 = std::lower_bound(hex, hex + 16, str[1]);
            bytes.push_back(((low1 - hex) << 4) | (low2 - hex));
            mask += "x";
        }
    }

    std::uintptr_t result;

    if (flags & SCAN_REVERSE) {
        for (auto ptr = start; ptr > end; ptr--) {
            if (Compare(bytes, mask, ptr)) {
                result = ptr;
                break;
            }
        }
    } else {
        for (auto ptr = start; ptr < end; ptr++) {
            if (Compare(bytes, mask, ptr)) {
                result = ptr;
                break;
            }
        }
    }

    result += offset;

    if (flags & CALL_REL_32) {
        result = ResolveRelativeCall(result);
    }

    return result;
}

std::uintptr_t Scan(const std::string& pattern, const int flags,
    const int offset)
{
    MODULEINFO mi;
    GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &mi,
        sizeof(MODULEINFO));
    return Scan(reinterpret_cast<std::uintptr_t>(mi.lpBaseOfDll),
        reinterpret_cast<std::uintptr_t>(mi.lpBaseOfDll) + mi.SizeOfImage,
        pattern, flags, offset);
}
} // namespace Memory
