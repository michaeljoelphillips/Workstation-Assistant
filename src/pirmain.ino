#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUDP.h>

byte sourceMac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte targetMac[] = {0xe0, 0x3f, 0x49, 0x49, 0xa6, 0x39};
IPAddress sourceIP(192, 168, 0, 177);
IPAddress destIP(255, 255, 255, 255);

unsigned int sourcePort = 8888;
unsigned int destPort = 50001;

char receiveBuffer[2];
byte payload[102] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xe0, 0x3f, 0x49, 0x49, 0xa6, 0x39,
	0xe0, 0x3f, 0x49, 0x49, 0xa6, 0x39,
	0xe0, 0x3f, 0x49, 0x49, 0xa6, 0x39,
	0xe0, 0x3f, 0x49, 0x49, 0xa6, 0x39,
	0xe0, 0x3f, 0x49, 0x49, 0xa6, 0x39,
	0xe0, 0x3f, 0x49, 0x49, 0xa6, 0x39,
	0xe0, 0x3f, 0x49, 0x49, 0xa6, 0x39,
	0xe0, 0x3f, 0x49, 0x49, 0xa6, 0x39,
	0xe0, 0x3f, 0x49, 0x49, 0xa6, 0x39,
	0xe0, 0x3f, 0x49, 0x49, 0xa6, 0x39,
	0xe0, 0x3f, 0x49, 0x49, 0xa6, 0x39,
	0xe0, 0x3f, 0x49, 0x49, 0xa6, 0x39,
	0xe0, 0x3f, 0x49, 0x49, 0xa6, 0x39,
	0xe0, 0x3f, 0x49, 0x49, 0xa6, 0x39,
	0xe0, 0x3f, 0x49, 0x49, 0xa6, 0x39,
	0xe0, 0x3f, 0x49, 0x49, 0xa6, 0x39 };

size_t payloadLen = sizeof(payload);

EthernetUDP udp;

const int ledPin = 9;
const int sensorPin = 8;
int sensorIn = 0;

char serialInput;
int mode = 1;

char serialBuffer;

void setup() {
	Ethernet.begin(sourceMac, sourceIP);
	udp.begin(sourcePort);

	pinMode(sensorPin, INPUT);
	pinMode(ledPin, OUTPUT);
	Serial.begin(9600);
}

void wakeOnLan() {
	udp.beginPacket(destIP, destPort);
	udp.write((uint8_t*)payload, payloadLen);
	udp.endPacket();
}

int getMode() {
	int packetSize = udp.parsePacket();
	if (packetSize) {
		udp.read(receiveBuffer, 2);

		int result = atoi(&receiveBuffer[0]);

		switch(result) {
			case 0:
				Serial.println("Mode 0");
				mode = 0;
				digitalWrite(ledPin, LOW);
				break;

			case 1:
				Serial.println("Mode 1");
				mode = 1;
				Serial.println("Changed Mode");
				break;

			case 2:
				Serial.println("Mode 2");
				mode = 2;
				Serial.println("Changed Mode");
				break;

			case 3:
				Serial.println("Mode 3");
				mode = 3;
				digitalWrite(ledPin, LOW);
				delay(30000);
				Serial.println("Changed Mode");
				break;
		}
	}
}

void loop() {
	sensorIn = digitalRead(sensorPin);

	getMode();

	if (mode == 0) {
		digitalWrite(ledPin, LOW);
	} else if (mode == 1) {
		digitalWrite(ledPin, HIGH);
	} else if (mode == 2) {
		digitalWrite(ledPin, LOW);
		if (sensorIn == 1) {
			digitalWrite(ledPin, HIGH);
			delay(5000);
			digitalWrite(ledPin, LOW);
		}
	} else if (mode == 3) {
		digitalWrite(ledPin, LOW);
		if (sensorIn == 1) {
			wakeOnLan();
			mode = 1;
		}
	}
}
