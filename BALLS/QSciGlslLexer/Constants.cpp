#include "Constants.hpp"

#include <QStringList>

namespace constants {
const char* LANGUAGE = "glsl";
const char* WORD_CHARS =
  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

const char* BLOCK_START_TOKENS = "{";
const char* BLOCK_END_TOKENS = "}";
const QStringList AUTOCOMPLETE_SEPARATORS = {"."};

// Add more to these with space separation
}
