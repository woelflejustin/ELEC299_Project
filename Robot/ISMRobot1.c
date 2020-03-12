    #include <SPI.h>
    #include <nRF24L01.h>
    #include <RF24.h>
    
    RF24 radio(9, 10); // CE, CSN

    //the address or pipe the two arduinos will use to communicate
    //chooses which reciever to talk to 
    const byte address[6] = "00001";
    boolean button_state = 0;
    int led_pin = 3;
    
    void setup() {
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
  const char text[] = "Hello World";
  radio.write(&text, sizeof(text));
  delay(1000);
}
