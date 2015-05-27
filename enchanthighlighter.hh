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
	QTextCharFormat format;
	using dict_ptr = std::unique_ptr<enchant::Dict>;
	dict_ptr dict;
	std::vector<dict_ptr> inactives;
	unsigned nwords = 0;
	unsigned nerrors = 0;
	
public:
	explicit EnchantHighlighter(QTextDocument *parent = 0);
	virtual ~EnchantHighlighter();

	bool addLanguage(const std::string &language);
	virtual void highlightBlock(const QString &text) override;
};

#endif // ENCHANTHIGHLIGHTER_HH
