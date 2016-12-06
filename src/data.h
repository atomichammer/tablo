#ifndef _data_h_
#define _data_h_
#include <QByteArray>
#include <QStringList>

class Convert
{
public:
	QByteArray getDynamic( QStringList* data );
	QByteArray getStatic( QStringList* data );

private:

};
#endif
