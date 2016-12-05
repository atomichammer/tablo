#ifndef _mylineedit_h_
#define _mylineedit_h_
#include <QEvent>
#include <QKeyEvent>
#include <QLineEdit>

class MyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    MyLineEdit(QWidget *parent = 0);
    void setIndex(unsigned int i) { index = i; }
    unsigned int getIndex(){ return index; }

private:
	unsigned int index;
protected:
	virtual void keyPressEvent ( QKeyEvent * event );
};
#endif
