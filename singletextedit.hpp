#ifndef SINGLETEXTEDIT_HH
#define SINGLETEXTEDIT_HH

#include <QTextEdit>
#include <QKeyEvent>

struct SingleTextEdit : QTextEdit {
	SingleTextEdit(QWidget *parent = nullptr) : QTextEdit(parent) {
		setTabChangesFocus(true);
		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	}
	virtual void keyPressEvent(QKeyEvent *event) override {
		if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
			event->ignore();
		else
			QTextEdit::keyPressEvent(event);
	}
	virtual QSize minimumSizeHint () const override {
		return sizeHint();
	}
	virtual QSize sizeHint() const override {
		QFontMetrics fm(font());
		int h = qMax(fm.height(), 14) + 4;
		return QSize {100, h};
	}
	virtual void wheelEvent(QWheelEvent *e) override {
		e->ignore();
	}
private:
	Q_OBJECT
};

#endif /* SINGLETEXTEDIT_HH */
