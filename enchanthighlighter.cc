#include "enchanthighlighter.hh"
#include <enchant++.h>
#include <QTextDocument>

EnchantHighlighter::EnchantHighlighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
    format.setUnderlineColor(QColor(Qt::red));
    format.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
}

EnchantHighlighter::~EnchantHighlighter() = default;


static inline bool isErrorWord(enchant::Dict *d, const QString &text, int begin, int i) {
    auto ba = QStringRef(&text, begin, i-begin).toUtf8();
    return !d->check(std::string {ba.data(), (std::string::size_type)ba.size()});
}


static double checkErrorProbability(enchant::Dict *d, const QString &text)
{
    int begin = 0;
    int i = 0;
    unsigned words = 0;
    unsigned errors = 0;
    for(; i < text.length(); ++i) {
        auto ch = text.at(i);
        if(ch < 'A') {
            if(begin >= i)
                begin++;
            else if(begin < i) {
                words++;
                if(isErrorWord(d,text,begin, i))
                    errors++;
                begin = i + 1;
            }
        }
    }
    if(begin < i) {
        words++;
        if(isErrorWord(d,text,begin, i))
            errors++;
    }
    return (double)errors/(double)(words?words:1);
}

void EnchantHighlighter::highlightBlock(const QString& text) {
    if(!dict)
        return;

    if(4*nerrors > nwords+6 && !inactives.empty()) {
        auto whole = document()->toPlainText();
        auto base = checkErrorProbability(dict.get(), whole);
        auto changed = false;
        for(auto&& d : inactives) {
            auto eprob = checkErrorProbability(d.get(), whole);
            if(eprob < base+0.01) {
                std::swap(dict, d);
                base = eprob;
                changed = true;
            }
        }
        if(changed) {
            nerrors = 0;
            nwords = 0;
			// When changing highlighting we want to rehighlight
			// everything. Calling 'rehighlight' would be logical
			// but due to Qt it crashes...
			// This workaround works without crashes.
			setDocument(document());
			return;
        }
    }

    int begin = 0;
    int i = 0;
    for(; i < text.length(); ++i) {
        auto ch = text.at(i);
        if(ch < 'A') {
            if(begin >= i)
                begin++;
            else if(begin < i) {
                nwords++;
                if(isErrorWord(dict.get(), text, begin, i)) {
                    nerrors++;
                    setFormat(begin, i-begin, format);
                }
                begin = i + 1;
            }
        }
    }
    if(begin < i && isErrorWord(dict.get(), text, begin, i)) {
        setFormat(begin, i-begin, format);
    }
}

bool EnchantHighlighter::addLanguage(const std::string &lang) {
    if(!enchant::Broker::instance()->dict_exists(lang))
        return false;
    if(!dict) {
        dict.reset(enchant::Broker::instance()->request_dict(lang));
        rehighlight();
        return true;
    }
    if(dict->get_lang() == lang)
        return true;
    inactives.emplace_back(enchant::Broker::instance()->request_dict(lang));
    return true;
}
