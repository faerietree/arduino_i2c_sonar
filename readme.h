// REFERENCES
// http://www.arduino.cc/playground/Main/MaxSonar
// http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1247476819#8
// http://giesler.biz/~bjoern/blog/?p=201


//
//FROM http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1288290319#2
//

/*
Hello,

I saw your forum post and have review your issue.

Causes of Incorrect range readings (with a properly installed and properly powered sensor)

Multiple Sensor Applications (Cross Talk)
Ultrasonic Sensors output and receive sound information when taking a range reading.  If multiple sensors are operational in the same environment this may cause one sensor to receive information sent by another sensor.  To eliminate interference, we recommend running sensors in sequence.  Refer to the examples of the three recommended ways to chain the sensors together.
http://www.maxbotix.com/uploads/Chaining_Application_Notes__AN_Output_Commanded_...
http://www.maxbotix.com/uploads/Chaining_Application_Notes__AN_Output_Constantly...
http://www.maxbotix.com/uploads/Chaining_Application_Notes__AN_Output_Simultaneo...

External acoustic noise
External acoustic noise sources may cause false range readings, that make phantom objects appear (i.e. readings are too close).  

Soft or angled targets
Also soft targets or angled targets can be missed occasionally.  That is, there must be a return of sufficient amplitude for detection and if enough energy is not returned then the target is missed.  This missed target issue can be exacerbated up close, where the returning energy must be detected in the presence of very high energy ring-down, so soft targets, or off axis targets, may not be detected easily.  (We will set the sensor to optimize for this... but the rules of physics will still apply.)

Filtering in General
For filtering, do not average.  Do not average.  Do not average.  (Did I say, do not average?)  We have a lot of internal filtering in our products and if you receive readings that you feel must be filtered, do not use averaging.  Instead, throwing out the incorrect range values will produce the best results.  Averaging valid and invalid readings together will provide incorrect data.  

Filtering
The filtering that works best, is either a Mode filter or a Median filter.  (Did I forget to say, "Do not average the readings"?)
 
The Median Filter
The median filter would take the last group of readings and first sort them and then pull out the center reading.  Here one might take three or more readings (up to say about 11 for people sensing) and after sorting the readings in order of range, pull out and use only the middle (median) reading.  Fairly good filtering.

The Mode Filter
The mode filter would take the largest group of the same readings out of a set of even larger readings.  Very robust filtering.
Filtering for most applications, the Very Simple Mode Filter

The simplest mode filter, is simple as this question, "Do the last two readings agree?" (with in a certain distance, and the certain distance depends upon how much variation one expects in the actual use.  Even so most use the logic of "are these readings exactly the same")?  If so, use the readings, otherwise throw away to oldest reading, and compare the next reading with the most current reading and do this again.  Obviously with very noisy data, one might have to sample a larger group of readings, and pull out the most common reading, but for most applications, this simple two reading filter works very well.


Even so, for specific cases, one has to try and find out what works best.

Please let us know if you have any further questions. I request that you email me any questions at scott@maxbotix.com.


Best regards,

Scott Wielenberg
Technical Support & Sales
of MaxBotix Inc.
Phone: (218) 454-0766 Ext. 2
Fax: (218) 454-0768
Email: scott@maxbotix.com
Web: www.maxbotix.com
*/


/*
* USTest - test program for Ultrasound rangers of the Maxbotix EZ range
*
* This uses serial communication for noise-free transfer. Since the Maxbotix
* sensors use RS232 logic but TTL levels, I had to write my own implementation
* of serial communications, which borrows from Arduino's SoftwareSerial code.
*/
/*
#define PIN_USTX 25
#define PIN_USBW 24
#define PIN_SERIAL 15

void setup() {
Serial.begin(115200);
pinMode(PIN_USTX, OUTPUT);
pinMode(PIN_USBW, OUTPUT);
pinMode(PIN_SERIAL, INPUT);
digitalWrite(PIN_USBW, 1);
digitalWrite(PIN_USTX, 0);
}

int bitPeriod = 1000000/9600;

int read() {
char buf[6];
buf[5] = 0;
for(int i=0; i<5; i++) {
int val = 0;
int bitDelay = bitPeriod - clockCyclesToMicroseconds(50);
while(digitalRead(PIN_SERIAL) == LOW);

if(digitalRead(PIN_SERIAL) == HIGH) {
delayMicroseconds(bitDelay/2 - clockCyclesToMicroseconds(50));
for(int offset = 0; offset < 8; offset++) {
delayMicroseconds(bitDelay);
int pinval = digitalRead(PIN_SERIAL);
if(pinval == LOW) val |= 1 << offset;
}
}

buf[i] = val;
delayMicroseconds(bitPeriod);
}

if(buf[0] != 'R' || buf[4] != 13) return -1;
buf[4] = 0;
return atoi(&(buf[1]));
}

void loop() {
int val;

// command ranging (pull TX pin up)
digitalWrite(PIN_USTX, 1);
delayMicroseconds(30);
digitalWrite(PIN_USTX, 0);

// wait >40ms for measurement and processing
delay(50);

// command output over serial and read
digitalWrite(PIN_USBW, 0);
int val = read();
digitalWrite(PIN_USBW, 1);

Serial.print("Range: "); Serial.println(val);

delay(100);
}
*/
