#include "enchanthighlighter.hh"
#include <enchant++.h>

EnchantHighlighter::EnchantHighlighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
    format.setUnderlineColor(QColor(Qt::red));
    format.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
}

EnchantHighlighter::~EnchantHighlighter() {}

void EnchantHighlighter::check(const QString &text, int begin, int i) {
    auto ba = QStringRef(&text, begin, i-begin).toUtf8();
    if(!dict->check(std::string {ba.data(), (std::string::size_type)ba.size()}))
        setFormat(begin, i-begin, format);
}

void EnchantHighlighter::highlightBlock(const QString& text) {
    if(!dict)
        return;
    int begin = 0;
    int i = 0;
    for(; i<text.length(); ++i) {
        auto ch = text.at(i);
        if(ch < 'A') {
            if(begin >= i)
                begin++;
            else if(begin<i) {
                check(text, begin, i);
                begin = i+1;
            }
        }
    }
    if(begin < i)
        check(text, begin, i);
}

bool EnchantHighlighter::setLanguage(const std::string &lang) {
    if(!enchant::Broker::instance()->dict_exists(lang))
        return false;
    dict.reset(enchant::Broker::instance()->request_dict(lang));
    return true;
}
