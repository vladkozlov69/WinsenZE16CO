#ifndef WinsenZE16CO_H_
#define WinsenZE16CO_H_

#if ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class WinsenZE16CO
{
private:
	Stream * _stream;
	uint8_t _index = 0;
	enum STATUS { STATUS_WAITING, STATUS_OK };
	uint8_t _payload[9];
	unsigned char _checksum;
	unsigned char _calculatedChecksum;
	unsigned long * _data;
	STATUS _status;
	void readLoop();
public:
	WinsenZE16CO(Stream * stream);
	bool readUntil(unsigned long * result, uint16_t timeout);
};




#endif