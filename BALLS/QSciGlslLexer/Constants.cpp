#include "Constants.hpp"

#include <QStringList>
#include <QColor>

namespace constants {
const char* LANGUAGE = "glsl";
const char* WORD_CHARS =
  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

const char* BLOCK_START_TOKENS = "{";
const char* BLOCK_END_TOKENS = "}";
const char* BLOCK_START_KEYWORDS = "case catch class default do else for if while";
const QStringList AUTOCOMPLETE_SEPARATORS = {"."};

const QColor DEFAULT_PAPER("#f9f9f9");

// Add more to these with space separation
}
