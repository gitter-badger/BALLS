#include "QSciGlslLexer.hpp"
#include "Constants.hpp"

#include <vector>
#include <QStringList>

#include <Qsci/qsciscintilla.h>

template<class T, class U, class V>
T clamp(T t, U min, V max) noexcept {
  if (t < min) { return min; }

  if (t > max) { return max; }

  return t;
}

// TODO: Adopt the style used on https://www.opengl.org/wiki
// See https://www.opengl.org/wiki/Core_Language_(GLSL)
QSciGlslLexer::QSciGlslLexer(QOpenGLShader::ShaderType type,
                             QObject* parent) noexcept :
QsciLexerCustom(parent),
                _type(type)
{
}

QStringList QSciGlslLexer::autoCompletionWordSeparators() const {
  return constants::AUTOCOMPLETE_SEPARATORS;
}

const char* QSciGlslLexer::language() const {
  return constants::LANGUAGE;
}

/*
const char* QSciGlslLexer::lexer() const {
  // TODO: Figure out why the hell I can't use ShaderType's QMetaEnum
  switch (_type) {
  case QOpenGLShader::Vertex:
    return "Vertex";

  case QOpenGLShader::Fragment:
    return "Fragment";

  case QOpenGLShader::Geometry:
    return "Geometry";

  case QOpenGLShader::TessellationControl:
    return "TessellationControl";

  case QOpenGLShader::TessellationEvaluation:
    return "TessellationEvaluation";

  case QOpenGLShader::Compute:
    return "Compute";

  default:
    return "";
  }
}
*/

const char* QSciGlslLexer::keywords(int set) const
{
  if (set == 1) {
    return ("in out varying uniform");
  }

  return (0);
}

int QSciGlslLexer::braceStyle() const {
  return Style::Brace;
}

const char* QSciGlslLexer::blockStart(int* style) const {
  if (style) {
    *style = Style::Brace;
  }

  return constants::BLOCK_START_TOKENS;
}

const char* QSciGlslLexer::blockEnd(int* style) const {
  if (style) {
    *style = Style::Brace;
  }

  return constants::BLOCK_END_TOKENS;
}

const char* QSciGlslLexer::wordCharacters() const {
  return constants::WORD_CHARS;
}

const char* QSciGlslLexer::blockStartKeyword(int* style) const
{
  if (style) {
    *style = Style::Control;
  }

  return constants::BLOCK_START_KEYWORDS;
}

QString QSciGlslLexer::description(int style) const {
  return styles()[clamp(style, 0, styles().size() - 1)].description();
}

QColor QSciGlslLexer::defaultColor(int style) const {
  return styles()[clamp(style, 0, styles().size() - 1)].color();
}

QColor QSciGlslLexer::defaultPaper(int style) const {
  return styles()[clamp(style, 0, styles().size() - 1)].paper();
}

QFont QSciGlslLexer::defaultFont(int style) const {
  return styles()[clamp(style, 0, styles().size() - 1)].font();
}

bool QSciGlslLexer::defaultEolFill(int style) const {
  return styles()[clamp(style, 0, styles().size() - 1)].eolFill();
}

void QSciGlslLexer::styleText(int start, int end) {
  char* range = new char[end - start + 1];
  editor()->SendScintilla(QsciScintilla::SCI_GETTEXTRANGE, start, end, range);
  QString text(range);
  delete range;

  qDebug() << text;

  startStyling(start);
  QStringList lines = text.split('\n');

  for (int i = 0; i < lines.size(); i++) {
    int style = Default;
    const QString& line = lines[i];

    if (line.startsWith("//")) {
      style = Comment;
    }
    else if (line.startsWith("#")) {
      style = Preprocessor;
    }

    setStyling(line.size(), style);

    if (i < lines.size() - 1) {
      setStyling(1, Default);
    }
  }
}

const std::array<QsciStyle, QSciGlslLexer::Style::Count>&
QSciGlslLexer::styles() noexcept {
  static std::array<QsciStyle, Style::Count> _styles;
  static bool initialized = false;

  if (Q_UNLIKELY(!initialized)) {
    initialized = true;

    QFont monospace("Monospace");
    monospace.setPointSize(9);
    QFont bold(monospace);
    QFont italic(monospace);
    QFont strike(monospace);

    bold.setBold(true);
    italic.setItalic(true);
    strike.setStrikeOut(true);

    _styles[Style::Default] = {
      Style::Default,
      "Keyword",
      QColor("#000000"),
      constants::DEFAULT_PAPER,
      monospace,
      true
    };

    _styles[Style::Type] = {
      Style::Type,
      "Type",
      QColor("#000066"),
      constants::DEFAULT_PAPER,
      bold,
      true
    };

    _styles[Style::Specifier] = {
      Style::Specifier,
      "Specifier",
      QColor("#333399"),
      constants::DEFAULT_PAPER,
      bold,
      true
    };

    _styles[Style::Specifier] = {
      Style::Property,
      "Property",
      QColor("#006600"),
      constants::DEFAULT_PAPER,
      monospace,
      true
    };

    _styles[Style::Literal] = {
      Style::Literal,
      "Literal",
      QColor("#0000ff"),
      constants::DEFAULT_PAPER,
      monospace,
      true
    };

    _styles[Style::Brace] = {
      Style::Brace,
      "Brace",
      QColor("#000066"),
      constants::DEFAULT_PAPER,
      monospace,
      true
    };

    _styles[Style::Function] = {
      Style::Function,
      "Function",
      QColor("#993333"),
      constants::DEFAULT_PAPER,
      bold,
      true
    };

    _styles[Style::Operator] = _styles[Style::Brace];

    _styles[Style::Comment] = {
      Style::Comment,
      "Comment",
      QColor("#666666"),
      constants::DEFAULT_PAPER,
      italic,
      true
    };

    _styles[Style::Predefined] = {
      Style::Predefined,
      "Predefined",
      QColor("#551111"),
      constants::DEFAULT_PAPER,
      monospace,
      true
    };

    _styles[Style::Preprocessor] = {
      Style::Preprocessor,
      "Preprocessor",
      QColor("#009900"),
      constants::DEFAULT_PAPER,
      monospace,
      true
    };

    _styles[Style::Control] = {
      Style::Control,
      "Control",
      QColor("#000000"),
      constants::DEFAULT_PAPER,
      bold,
      true
    };

    _styles[Style::Reserved] = {
      Style::Reserved,
      "Reserved",
      QColor("#FF0000"),
      QColor("#000000"),
      strike,
      true
    };

    _styles[Style::BlockComment] = _styles[Style::Comment];

    qDebug() << "Initialized default GLSL styles";
  }

  return _styles;
}
