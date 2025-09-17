// Include Serial
#include <SoftwareSerial.h>

// Define Out
#define TX_OUT 4
#define RX_IN 3

const SoftwareSerial	mySerial(RX_IN, TX_OUT);

void	setup()
{
	Serial.begin(9600);
	mySerial.begin(2400);
}

void	loop()
{
	String	msg;
	while (mySerial.available())
	{
		msg = mySerial.readString();
		Serial.println(msg);
	}
}
