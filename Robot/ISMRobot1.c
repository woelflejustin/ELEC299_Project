//IR sensor implementation
    #include "QSerial.h"
    QSerial myIRx;
    QSerial myIRy;

//ISM transmitter and receiver implementation
    #include <SPI.h>
    #include <nRF24L01.h>
    #include <RF24.h>
    RF24 radio(9, 10); // CE, CSN
    //the address or pipe the two arduinos will use to communicate
    //chooses which reciever to talk to 
    const byte address[6] = "00001";
    
    void setup() {
//IR sensor setup
    myIRx.attach(5,-1);
    myIRy.attach(4,-1);

//ISM transmitter and receiver setup
        pinMode(6, OUTPUT);
        Serial.begin(9600);
        //Initiate transmission signals
        radio.begin();
        // Open a reading pipe on radio
        radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
        radio.setPALevel(RF24_PA_MIN);       //Set power amiplifier level
        radio.startListening();              //This sets the module as receiver
    }

void loop() {
  //once IR sensors have received information, store in array
  char IRlocationx = myIRx.receive(500);
  char IRlocationy = myIRx.receive(500);
  char location[2] = IRlocationx,IRlocationy;
  //use ISM transciever and send to robot 2
  radio.write(&location, sizeof(location));
  delay(1000);
}
