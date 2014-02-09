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
	void keyPressEvent(QKeyEvent *event) {
		if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
			event->ignore();
		else
			QTextEdit::keyPressEvent(event);
	}
	virtual QSize minimumSizeHint () const {
		return sizeHint();
	}
	virtual QSize sizeHint() const {
		QFontMetrics fm(font());
		int h = qMax(fm.height(), 14) + 4;
		return QSize{100,h};
	}
	void wheelEvent(QWheelEvent *e) {
		e->ignore();
	}
};

#endif /* SINGLETEXTEDIT_HH */
