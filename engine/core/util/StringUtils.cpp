#include "StringUtils.h"

#include <codecvt>
#include <iterator>
#include <locale>

using namespace cg;

std::string StringUtils::toUTF8(wchar_t codepoint){
    std::wstring_convert<std::codecvt_utf8<wchar_t>,wchar_t> convert;
    return convert.to_bytes(codepoint);
}
