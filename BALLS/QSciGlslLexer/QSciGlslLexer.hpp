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
  const char* lexer() const override;
  int braceStyle() const override;
  int lexerId() const override;
  const char* blockStart(int *style) const override;
  const char* blockEnd(int *style) const override;
  QColor color(int style) const override;
  QColor paper(int style) const override;
  QFont font(int style) const override;
  QString description(int style) const override;
  bool eolFill(int style) const override;

  void styleText(int start, int end) override;
  QStringList autoCompletionWordSeparators() const override;
  const char* wordCharacters() const override;
signals:

public slots:
private:
  enum Style {
    Default,
    Macro,
    Keyword,
    Brace,
    Operator,
    Function,
    Literal,
    Type,
    Comment,
    BlockComment,
    Reserved,
    Count,
  };

  std::array<QsciStyle, Style::Count> _styles;
  QOpenGLShader::ShaderType _type;
  QMetaEnum _enum;
};

#endif // QSCIGLSLLEXER_HPP
