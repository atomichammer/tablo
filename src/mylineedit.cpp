#include "mylineedit.h"
#include "settingsstorage.h"

MyLineEdit::MyLineEdit(QWidget *parent)
{
    this->setParent(parent);
}

void MyLineEdit::keyPressEvent ( QKeyEvent * event )
{
    int vrPos, vrLength, vrSelStart, vrMaxLength;
    bool selected;

    vrLength = this->text().size();
    vrPos = this->text().indexOf(",");
    vrSelStart = this->cursorPosition();
    selected = this->hasSelectedText();

    if (vrPos == -1)
    {
        vrMaxLength = 5;
    } else
    {
        vrMaxLength = 6;
    }
		
    if (event->key() >= 0x30 && event->key() <= 0x39)
    {
        if ((vrLength != vrMaxLength) || selected)
        {
			QLineEdit::keyPressEvent(event);
		}
	}else if ((event->key() == Qt::Key_Backspace) || 
                (event->key() == Qt::Key_Delete) ||
                (event->key() == Qt::Key_Left) ||
                (event->key() == Qt::Key_Right) ||
                (event->key() == Qt::Key_Home) ||
                (event->key() == Qt::Key_End) ||
                (event->key() == Qt::Key_Return) ||
                (event->key() == Qt::Key_Enter))
    {
		QLineEdit::keyPressEvent(event);
	}else if ((event->key() == 0x2C) ||
                (event->key() == 0x2E))
    {
		if ((vrPos != -1) || 
			(vrSelStart == 0) ||
			(vrLength == 0) || 
            (vrSelStart == 5))
        {
				return;
        }
        else
        {
			QLineEdit::keyPressEvent(event);
			this->setText(this->text().replace(".",","));
		}
	}
}
