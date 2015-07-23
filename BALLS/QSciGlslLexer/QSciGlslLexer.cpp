#include "QSciGlslLexer.hpp"
#include "Constants.hpp"

#include <vector>
#include <QStringList>

#include <Qsci/qsciscintilla.h>

QSciGlslLexer::QSciGlslLexer(QOpenGLShader::ShaderType type,
                             QObject* parent) noexcept :
QsciLexerCustom(parent),
                _type(type)
{
  _styles[Style::Default] = {
    -1,
    "Keyword",
    QColor(255, 0, 0),
    QColor(0, 0, 255),
    QFont("Monospace"),
    true
  };

  std::fill(_styles.begin(), _styles.end(), _styles[0]);

}

QStringList QSciGlslLexer::autoCompletionWordSeparators() const {
  return constants::AUTOCOMPLETE_SEPARATORS;
}

const char* QSciGlslLexer::language() const {
  return constants::LANGUAGE;
}

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

int QSciGlslLexer::lexerId() const {
  return _type;
}

const char* QSciGlslLexer::wordCharacters() const {
  return constants::WORD_CHARS;
}

QColor QSciGlslLexer::color(int style) const {
  return _styles[0].color();
}

QFont QSciGlslLexer::font(int style) const {
  return _styles[0].font();
}

QString QSciGlslLexer::description(int style) const {

  return _styles[0].description();
}

QColor QSciGlslLexer::paper(int style) const {
  return _styles[0].paper();
}

bool QSciGlslLexer::eolFill(int style) const {
  return _styles[0].eolFill();
}

void QSciGlslLexer::styleText(int start, int end) {
  char* range = new char[end - start + 1];
  editor()->SendScintilla(QsciScintilla::SCI_GETTEXTRANGE, start, end, range);
  QString text(range);
  delete range;

  startStyling(start);
  QStringList lines = text.split('\n');

  for (int i = 0; i < lines.size(); i++) {
    int style = Default;

    if (lines.at(i).startsWith("//")) {
      style = Comment;
    }

    if (lines.at(i).startsWith("#")) {
      style = Macro;
    }

    setStyling(lines.at(i).size(), style);

    if (i < lines.size() - 1) {
      setStyling(1, Default);
    }
  }
}
