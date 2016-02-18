

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

#include <SimpleTimer.h>
#include "S_message.h"

//LED Config
const int ledArduino = 13;
const int ledRed =  5; 
const int ledGreen =  4; 
const int ledBlue =  3; 
int ledState = LOW;  

#define MAX_NODES 20

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

// nRF24L01(+) radio attached using Getting Started board
RF24 radio(9, 10);

// Network uses that radio
RF24Network network(radio);

SimpleTimer ledTimer;

// Address of our node
const uint16_t this_node = 0;

// Address of the other node
const uint16_t other_node = 1;

// Structure of our payload
struct payload_t
{
  unsigned long ms;
  unsigned long counter;
};

// System configuration
struct K_message_t
{
  uint8_t revision; /* Which rev of the current system config? */
  uint8_t sleep_cycles; /* How many 8s sleep cycles to wait between transmissions */
};

K_message_t system_config = { 0, 0 };
int nodes[MAX_NODES];
int nodeCount = 0;

void setup(void)
{
  Serial.begin(9600);
  Serial.println("RF24Network/examples/helloworld_rx/");

  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);

  Serial.println("ready");

	//setup leds
	pinMode(ledRed, OUTPUT);
	pinMode(ledGreen, OUTPUT);
	pinMode(ledBlue, OUTPUT);
	digitalWrite(ledRed, LOW);
	digitalWrite(ledGreen, LOW);
	digitalWrite(ledBlue, LOW);


	inputString.reserve(200);
	printMenu();
	//setup timers
	//ledTimer.setInterval(500, ledDone);  
}

void loop(void)
{
	// Pump the network regularly
	network.update();
	// Is there anything ready for us?
	while ( network.available() )
	{
		RF24NetworkHeader header;
		network.peek(header);
		addNode(header.from_node);
		int color = ledGreen;

		if(header.from_node == 1)
			color = ledGreen;
		else
			color = ledRed;
			
		//toggle the LED
		if (ledState == LOW)
			ledState = HIGH;
		else
			ledState = LOW;
		
		digitalWrite(color, ledState);
		
		//printf_P(PSTR("%lu: APP Received #%u type %c from 0%o\n\r"), millis(), header.id, header.type, header.from_node);
		Serial.println("Message Type: " + String(header.type));

		
		if ( header.type == 'S' ) {
			Serial.println("=== S message received ===");
			// child is requesting a configuration from us
			S_message message;
			network.read(header, &message, sizeof(message));
			Serial.println("Node: " + String(header.from_node));
			Serial.println("Temp: " + String(message.temp_reading));
			Serial.println("Voltage: " + String(message.voltage_reading));
			Serial.println("Humidity: " + String(message.humidity_reading));
			Serial.println("Lost: " + String(message.lost_packets));
	
		}else if ( header.type == 'k' ){
			// child is requesting a configuration from us
			Serial.println("=== k configuration ===");
			network.read(header, NULL, 0);
			RF24NetworkHeader response_header(/*to node*/ header.from_node, /*type*/ 'K');
			network.write(response_header, &system_config, sizeof(system_config));
			printf_P(PSTR("%lu: APP Sending new config #%i: %i to 0%o\n\r"), millis(), system_config.revision, system_config.sleep_cycles, header.from_node);
			Serial.println("Sending new config to node " + String(header.from_node));
		
		} else {
		
			Serial.println(String(header.type) + " Packet Received");
		
		}
		
	
	}

}

void printMenu(){
	Serial.println("config : Send Configs");
	Serial.println("nodes  : Print Nodes");
}

void printNodes(){
	for(int i=0; i<nodeCount; i++){
		if(nodes[i] > 0){
			Serial.println("Node " + String(nodes[i]));
		}
	}
	Serial.println("Total: " + String(nodeCount) + " nodes");
}

void addNode(int nodeId){
	bool nodeExists = false;
	for(int i=0; i<nodeCount; i++){
		if(nodes[i] == nodeId)
			nodeExists = true;
	}

	if(!nodeExists){
		//add node to nodes array
		nodes[nodeCount] = nodeId;
		nodeCount++;
		Serial.println("Node " + String(nodeId) + " added. Total: " + String(nodeCount));
	}
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    if (inChar == '\n') {
    	//remove the termination character
    	inputString.replace("\n","");
    	//Serial.println("Command: " + String(inputString));
    	if(inputString == "config"){
    		Serial.println("Send configs");
    	}else if(inputString == "nodes"){
    		printNodes();
    	}else{
    		Serial.println("Unknown command");
    	}
    			
    
      stringComplete = true;
      inputString = "";
    }
  }
}
