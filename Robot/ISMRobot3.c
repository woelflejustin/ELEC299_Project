 //IR sensor implementation
        #include "QSerial.h"
        QSerial myIRx;
        QSerial myIRy;

//ISM transmitter and receiver implementation
        #include <SPI.h>
        #include <nRF24L01.h>
        #include <RF24.h>
        RF24 radio(9, 10); // CE, CSN
        const byte address[6] = "00001";

    void setup() {
 //IR sensor setup
        myIRx.attach(5,-1);
        myIRy.attach(4,-1);
//ISM transmitter and receiver setup
        Serial.begin(9600);
        radio.begin();
        radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
        radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
        radio.startListening();              //This sets the module as receiver
    }
    void loop()
    {
//IR sensors receive robot location
      char IRlocationx = myIRx.receive(500);
      char IRlocationy = myIRx.receive(500);
//ISM transceiver recieves information from robot 1
      delay(5);
      radio.startListening();                            //This sets the module as receiver
      while(!radio.available());                         //Looking for incoming data
      radio.read(&location, sizeof(location)); //Reading the data
  //driving code in direction of location[]
      int vectorx = x - IRlocationx;
        //driving code in direction of vectorx
      int vectory = y - IRlocationy;
        //driving code in direction of vectorx
    }
