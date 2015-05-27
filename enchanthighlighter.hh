#ifndef ENCHANTHIGHLIGHTER_HH
#define ENCHANTHIGHLIGHTER_HH

#include <memory>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

typedef struct str_enchant_dict   EnchantDict;
namespace enchant {
	class Dict;
}

class EnchantHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT
	std::unique_ptr<enchant::Dict> dict;
	QTextCharFormat format;
	void check(const QString &ref, int, int);
public:
	explicit EnchantHighlighter(QTextDocument *parent = 0);
	virtual ~EnchantHighlighter();

	bool setLanguage(const std::string &language);
	virtual void highlightBlock(const QString &text) override;
};

#endif // ENCHANTHIGHLIGHTER_HH
