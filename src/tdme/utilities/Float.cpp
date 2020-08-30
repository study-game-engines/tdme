#include <tdme/utilities/Float.h>

#include <string>

using std::string;
using std::to_string;
using std::stof;
using std::isnan;

using tdme::utilities::Float;

constexpr float tdme::utilities::Float::MAX_VALUE;
constexpr float tdme::utilities::Float::MIN_VALUE;
constexpr float tdme::utilities::Float::NaN;
constexpr int32_t tdme::utilities::Float::SIZE;

float Float::parseFloat(const string& str) {
	if (str.length() == 0) return 0.0f;
	return stof(str);
}