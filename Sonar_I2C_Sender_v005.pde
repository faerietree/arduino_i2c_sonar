#include <Wire.h>

#define SONAR_PULSE_IN_PIN 5                  // Pulse input pin for sonar if using PWM input signal
#define SONAR_ANALOG_IN_PIN 0                 // Analog input pin for sonar if using analog signal
#define LED_PIN 13                            // Standard LED PIN
#define I2CSonar_ADDRESS 0x02                 // Arbitary choice
#define MIN_RANGE 20                          // Based on specs from MB1200 data sheet
#define MAX_RANGE 765                         // Based on specs from MB1200 data sheet
#define US_PER_CM 58                          // 58 microsecs per cm Based on specs from MB1200 data sheet
#define SPIKE_EFFECT 4.0                      // used in spike filter - effectively this is the size of teh equivalent moving average filter
#define ARRAY_SIZE 5                          // size of median and mode sampling arrays. Needs to be an odd number

#define STBY_MODE 0                           // Startup Mode 
#define NORM_MODE 1                           // Normal Mode: Return latest raw sonar value on request 
#define MODE_MODE 2                           // Mode filter mode: Return the latest value from running 'mode' filter
#define MEDN_MODE 3                           // Median filter mode: Return the latest value from running 'median' filter
#define SPIK_MODE 4                           // Spike filter mode: Return value from running 'spike' filter

#define CMD_NORM_MODE 0x01                    // Command 1 Code
#define CMD_MODE_MODE 0x02                    // Command 2 Code
#define CMD_MEDN_MODE 0x03                    // Command 3 Code
#define CMD_SPIK_MODE 0x04                    // Command 4 Code

int val = 0;
byte* valPtr;
byte dataRx = 0;
boolean cmdReady = false;
int filtMode = STBY_MODE;

// For Mode / median filter variables needed to store values
// array to store the raw samples
int rawValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
// array to store the sorted samples
int srtValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
// Index for latest position in array
int idx = 0;

void setup()
{
  valPtr = (byte*) &val;                      // create a pointer to the two byte integer that holds the sonar value data
  Wire.begin(I2CSonar_ADDRESS);               // join i2c bus with address I2CSonar_ADDRESS
  Wire.onRequest(requestEvent);               // register event
  Wire.onReceive(receiveEvent);               // register event
  pinMode(LED_PIN, OUTPUT);                   // Setup LED pin 
  digitalWrite(LED_PIN, HIGH);                // set the LED on
  filtMode = MODE_MODE;                       // Start in modal filter mode 
}

void loop()
{
  int tmp=0;

  digitalWrite(13, LOW);                      // turn the LED off

  // process any commands from I2C master
  if (cmdReady)
  {
    switch (dataRx) {
    case CMD_NORM_MODE:
      filtMode=NORM_MODE;
      //Do NORMAL startup stuff here
      cmdReady = false;
      break;
    case CMD_MODE_MODE:
      filtMode=MODE_MODE;
      //Do MODE startup stuff here
      for(int i=0; i < ARRAY_SIZE; i++)       // Set all array value to Zeros
      {
        rawValues[i] = 0;
        srtValues[i] = 0;
      }
      idx = 0;                                // Set index to first place in array
      cmdReady = false;
      break;
    case CMD_MEDN_MODE:
      filtMode=MEDN_MODE;
      //Do MEDN startup stuff here
      for(int i=0; i < ARRAY_SIZE; i++)       // Set all array value to Zeros
      {
        rawValues[i] = 0;
        srtValues[i] = 0;
      }
      idx = 0;                                // Set index to first place in array
      cmdReady = false;
      break;
    case CMD_SPIK_MODE:
      filtMode=SPIK_MODE;
      //Do SPIK startup stuff here
      cmdReady = false;
      break;
    default:
      break;
    }
  }

  // Get the latest data from Sonar
  tmp = ReadSonarPulse();
  if (tmp> MAX_RANGE) tmp=MAX_RANGE; 
  if (tmp< MIN_RANGE) tmp=0; 

  // Process the Sonar data
  switch (filtMode) {
  case NORM_MODE:
    val=tmp;                                  // Set the value (val) to the latest data
    break;
  case SPIK_MODE:
    val = val * (1.0-(1.0/SPIKE_EFFECT)) + tmp * (1.0/SPIKE_EFFECT);          // implement a slow ramp / spike filter somewhat equiv to x-sample running average
    break;
  case MODE_MODE:
    //Do MODE stuff here
    rawValues[idx] = tmp;
    idx++;
    if (idx>=ARRAY_SIZE) 
      idx=0;
    for(int i=0; i < ARRAY_SIZE; i++)         // copy raw data to sorted data array
      srtValues[i]=rawValues[i];
    isort(srtValues,ARRAY_SIZE);              // sort the sorted data array
    val = mode(srtValues,ARRAY_SIZE);         // find the modal value and use it
    break;
  case MEDN_MODE:
    //Do MEDN stuff here
    rawValues[idx] = tmp;
    idx++;
    if (idx>=ARRAY_SIZE) 
      idx=0;
    for(int i=0; i < ARRAY_SIZE; i++)         // copy raw data to sorted data array
      srtValues[i]=rawValues[i];
    isort(srtValues,ARRAY_SIZE);              // sort the sorted data array
    val = median(srtValues,ARRAY_SIZE);       // find the median value and use it
    break;
  default:
    break;
  }

  delay(25);
}

//---------------------------------------
// Called when I2C master requests data from Slave
void requestEvent()
{
  digitalWrite(13, HIGH);                     // set the LED on to show we are sending data
  Wire.send(valPtr, 2);                       // Send two bytes starting at a pointer that points to the location of the Sonar data
}

//---------------------------------------
// Called when I2C master sends command data to Slave
void receiveEvent(int numBytes) 
{
  for (int i=0;i<numBytes;i++)                // only expecting one byte commands in this implementation but just to be sure lets read all of them
    dataRx = Wire.receive();
  cmdReady = true;
}


//---------------------------------------
//Sorting function
// sort function (Author: Bill Gentles, Nov. 12, 2010)
void isort(int *a, int n){
    for (int i = 1; i < n; ++i)
  {
    int j = a[i];
    int k;
    for (k = i - 1; (k >= 0) && (j < a[k]); k--)
    {
      a[k + 1] = a[k];
    }
    a[k + 1] = j;
  }

}

//---------------------------------------
//Mode function, returning the modal value (or median value if mode does not exist).
int mode(int *x,int n){

  int i = 0;
  int count = 0;
  int maxCount = 0;
  int mode = 0;
  int bimodal;
  int prevCount = 0;

  while(i<(n-1)){

    prevCount=count;
    count=0;

    while(x[i]==x[i+1]){

      count++;
      i++;
    }

    if(count>prevCount&count>maxCount){
      mode=x[i];
      maxCount=count;
      bimodal=0;
    }
    if(count==0){
      i++;
    }
    if(count==maxCount){                      //If the dataset has 2 or more modes.
      bimodal=1;
    }
    if(mode==0||bimodal==1){                  //Return the median if there is no mode.
      mode=x[(n/2)];
    }
    return mode;
  }

}

//---------------------------------------
//Mode function, returning the median.
int median(int *x,int n){

  return (x[(n/2)]);
}



















