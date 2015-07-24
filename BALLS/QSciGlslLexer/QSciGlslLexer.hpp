#ifndef QSCIGLSLLEXER_HPP
#define QSCIGLSLLEXER_HPP

#include <array>

#include <QtCore/QMetaEnum>
#include <QtGui/QOpenGLShader>

#include <Qsci/qscilexercustom.h>
#include <Qsci/qscistyle.h>

class QStringList;

class QSciGlslLexer : public  QsciLexerCustom {
  Q_OBJECT
public:
  explicit QSciGlslLexer(QOpenGLShader::ShaderType, QObject* parent = 0) noexcept;

  const char* language() const override;
  //const char* lexer() const override;
  int braceStyle() const override;
  const char* blockStart(int* style) const override;
  const char* blockEnd(int* style) const override;
  const char* blockStartKeyword(int *style) const override;
  QColor defaultColor(int style) const override;
  QColor defaultPaper(int style) const override;
  QFont defaultFont(int style) const override;
  QString description(int style) const override;
  bool defaultEolFill(int style) const override;

  void styleText(int start, int end) override;
  QStringList autoCompletionWordSeparators() const override;
  const char* wordCharacters() const override;
  const char* keywords(int set) const override;
signals:

public slots:
private:
  enum Style {
    Default, // Whitespace
    Type,
    Specifier,
    Property,
    Function,
    Literal,
    Operator,
    Comment,
    Brace,
    Predefined,
    Preprocessor,
    Control,
    BlockComment,
    Reserved,
    Count,
  };

  QOpenGLShader::ShaderType _type;
  QMetaEnum _enum;

  static const std::array<QsciStyle, Style::Count>& styles() noexcept;
};

#endif // QSCIGLSLLEXER_HPP
