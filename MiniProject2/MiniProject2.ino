#define RX_IN 0

String msg;

void	setup()
{
	Serial.begin(9600);
}

void	loop()
{
	while (Serial.available())
	{
		msg = Serial.readString();
		Serial.println(msg);
	}
}
