//IR sensor implementation
    #include "QSerial.h"
    QSerial myIRx;
    QSerial myIRy;

//ISM transmitter and receiver implementation
    #include <SPI.h>
    #include <nRF24L01.h>
    #include <RF24.h>
    RF24 radio(9, 10); // CE, CSN
    const byte addresses [][6] = {"00001", "00002"};  //Setting the two addresses. One for transmitting and one for receiving
    char x = 0//IR 1
    char y = 0//IR 2
    char location[2] = x,y;


void setup() {
//IR sensor setup
    myIRx.attach(5,-1);
    myIRy.attach(4,-1);
//ISM transmitter and receiver setup
    radio.begin();                           //Starting the radio communication
    radio.openWritingPipe(addresses[1]);     //Setting the address at which we will send the data
    radio.openReadingPipe(1, addresses[0]);  //Setting the address at which we will receive the data
    radio.setPALevel(RF24_PA_MIN); //You can set it as minimum or maximum depending on the distance between the transmitter and receiver. 
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

  radio.stopListening();                             //This sets the module as transmitter
  x = myIRx.receive(500);
  y = myIRy.receive(500);
  char location[2] = x,y;
  radio.write(&location, sizeof(location));  //Sending the data
  delay(5);
  

}
