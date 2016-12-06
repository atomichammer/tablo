#include <QMessageBox>
#include <QDateTime>

#include "data.h"
#include "settingsstorage.h"
#include <iostream>

QByteArray Convert::getStatic( QStringList* data )
{

	if (data->isEmpty())
		return 0;

        SettingsStorage *settings = SettingsStorage::Instance();

	int count = data->count();
	int length = 0;
	int j = 0;

	printf("data count = %d\n", count);

	//We should fill free cells in each entry with 0x0A
        for(int i=0; i<count; i++)
        {
		if(data->at(i).contains(",")){
                        length = settings->getLengths().at(j);
		} else {
                        length = settings->getLengths().at(j) - 1;
		}
		
		while ( data->at(i).size() < length){ //6 should be variable
			data->insert(i, data->takeAt(i).prepend(QChar(0x0A)));
		}
		
		//data->insert(i,data->at(i).rightJustified(length,QChar(0x0A),true));
		
		if(i%2) j++;	
	}

	QByteArray bytes;
	
	bytes = data->join(0).toAscii();	

	for( int i=0; i<bytes.size(); ){
		bytes[i] = bytes.at(i) & 0x0F;
		
		if ( bytes.at(i) == 0xC || bytes.at(i) == 0xE ){
			bytes[i-1] = bytes.at(i-1) | 0x10;
			bytes.remove(i, 1);
			continue;
		}
		i++;
	}
	
	return bytes;
}

QByteArray Convert::getDynamic( QStringList* data )
{
	if (data->isEmpty())
		return 0;

	int count = data->count();
	int length = 0;
	int j = 0;

	//We should fill free cells in each entry with 0x0A
	for(int i=0; i<count; i++){
		if(data->at(i).contains(",")){
			length = 6;
		} else {
			length = 5;
		}
		while ( data->at(i).size() < length){
			data->insert(i, data->takeAt(i).prepend(QChar(0x0A)));
		}
		if(i%2) j++;
		
	}
	

	QDateTime *dateTime = new QDateTime();
        data->prepend(dateTime->currentDateTime().toString("0dd,MM0HH,mm"));
	delete	dateTime;

	QByteArray bytes;
	
	bytes = data->join(0).toAscii();	

	for( int i=0; i<bytes.size(); ){
		bytes[i] = bytes.at(i) & 0x0F;
		
		if ( bytes.at(i) == 0xC || bytes.at(i) == 0xE ){
			bytes[i-1] = bytes.at(i-1) | 0x10;
			bytes.remove(i, 1);
			continue;
		}
		i++;
	}
	

        //int MinLowPos = bytes.size();
	
        count = ((bytes.size() / 10) + 1) * 10 - bytes.size();
        if(count < 10)
                count = ((bytes.size() / 10) + 1) * 10 - bytes.size();
        else
                count = (bytes.size() / 10) * 10 - bytes.size();

        for(int i=0; i<count; i++){
                bytes.append(0x0A);
        }

//	count = ((bytes.size() / 8) + 1) * 8 - bytes.size();
//	if(count < 8)
//		count = ((bytes.size() / 8) + 1) * 8 - bytes.size();
//	else
//		count = (bytes.size() / 8) * 8 - bytes.size();
//
//	for(int i=0; i<count; i++){
//		bytes.append(0x0A);
//	}
	
        //bytes.prepend(MinLowPos);

	return bytes;
		
}
