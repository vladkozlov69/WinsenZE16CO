#include <WinsenZE16CO.h>

WinsenZE16CO::WinsenZE16CO(Stream * stream)
{
	_stream = stream;
}

bool WinsenZE16CO::readUntil(unsigned long * result, uint16_t timeout)
{
    _data = result;
    uint32_t start = millis();
    do
    {
      readLoop();
      if (_status == STATUS_OK) break;
    } while (millis() - start < timeout);

    return _status == STATUS_OK;
}

void WinsenZE16CO::readLoop()
{
	_status = STATUS_WAITING;
	if (_stream->available())
	{
		uint8_t ch = _stream->read();
		
		Serial.print(" [");Serial.print(_index);Serial.print("]=");Serial.print(ch, HEX);
		
		switch (_index)
	    {
	    	case 0:
	      	if (ch != 0xFF)
	      	{
	        	return;
	      	}
	      	_calculatedChecksum = 0;
	      	break;
	    case 1:
	      	if (ch != 0x04)
	      	{
	        	_index = 0;
	        	return;
	      	}
	      	_calculatedChecksum += ch;
	      	break;

	    default:
	      	if (_index < 8)
	      	{
	        	_calculatedChecksum += ch;
	      	}
	      	else
	      	{
	        	_checksum = ch;
	      	}
	      	break;
	    } // switch

	    if (_index == 8)
	    {
			
	        _calculatedChecksum = (~_calculatedChecksum) + 1;

	        *_data = _payload[4] * 256 + _payload[5];

	        if (_calculatedChecksum == _checksum)
	        {
	          _status = STATUS_OK;
	        }
			
			 Serial.println();
			// Serial.println(_calculatedChecksum);
			// Serial.println(_checksum);
			// Serial.println();

	        _index = 0;
	        return;
	    }
	    else
	    {
	      	_payload[_index] = ch;
	    }
 
	    _index++;
	}
}