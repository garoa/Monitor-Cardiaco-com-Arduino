#define SAMPLES 50
#define MAXBPM 200

const int ledPin = 13;

//Shift Register pins
const int latchPin = 8;
const int clockPin = 12;
const int dataPin = 11;

//Display Cathodes
const int dispPin0 = 3;
const int dispPin1 = 4;
const int dispPin2 = 5;
const int dispPin3 = 6;

int bpm = 0, sensorValue;

int last[SAMPLES];
unsigned long lasttime = 0;
unsigned long i;

void setup()
{
  	Serial.begin(9600);
	pinMode(latchPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dispPin0, OUTPUT);
	pinMode(dispPin1, OUTPUT);
	pinMode(dispPin2, OUTPUT);
	pinMode(dispPin3, OUTPUT);
        memset(last, 0, sizeof(last));
}

void loop()
{
	digitalWrite(ledPin, LOW);
	sensorValue = analogRead(0);
	last[i % SAMPLES] = sensorValue;
	int delta = last[i % SAMPLES] - last[(i - 10) % SAMPLES];
        //Verify if is a heart beat
	if (60000.0 / MAXBPM < millis() - lasttime && delta * delta > 12 && delta * delta < 100) {
                //Turn on the led
		digitalWrite(ledPin, HIGH);
                unsigned long altualTime = millis();
                int delayMs = altualTime - lasttime;
                //Calculate the frequency in bpm / frequency in bpm = 1.0 / delay in ms * 60000
		bpm = 60000.0 / delayMs;
                Serial.println(bpm);
		lasttime = altualTime;
	}
        //Show the frequency in the display
        //Turn of all the segments
	setDisplay(-1);
        //Set the shift register
	registerWrite(getBit(bpm / 1000 % 10), HIGH);
        //Turn on the segment
	setDisplay(0);
	delay(1);
	setDisplay(-1);
	registerWrite(getBit(bpm / 100 % 10), HIGH);
	setDisplay(1);
	delay(1);
	setDisplay(-1);
	registerWrite(getBit(bpm / 10 % 10), HIGH);
	setDisplay(2);
	delay(1);
	setDisplay(-1);
	registerWrite(getBit(bpm % 10), HIGH);
	setDisplay(3);
	delay(1);
	++i;
}

//Set the atual display
void setDisplay(int disp){
	digitalWrite(dispPin0, disp == 0 ? 1 : 0);
	digitalWrite(dispPin1, disp == 1 ? 1 : 0);
	digitalWrite(dispPin2, disp == 2 ? 1 : 0);
	digitalWrite(dispPin3, disp == 3 ? 1 : 0);
}

//Return the bit to send to the shift register for a given number
int getBit(int number){
	switch (number) {
	case 0:
		return 189;
	case 1:
		return 132;
	case 2:
		return 217;
	case 3:
		return 205;
	case 4:
		return 228;
	case 5:
		return 109;
	case 6:
		return 125;
	case 7:
		return 133;
	case 8:
		return 255;
	case 9:
		return 237;
	}
}

//Send data to the shift register
void registerWrite(byte bitsToSend, int whichState) {
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, MSBFIRST, bitsToSend);
	digitalWrite(latchPin, HIGH);
}
