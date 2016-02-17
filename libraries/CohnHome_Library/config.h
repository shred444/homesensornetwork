

//DHT CONFIG
#define DHTPIN 6     // what pin we're connected to
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11


int garageInput = 3;
int sensorValue = 0;
#define server_ip "http://192.168.1.109:3030"
#define nodered_ip "http://192.168.1.109:1880"


//Moisture Sensor
#define plantPin 0

int pin = 13;

int debounceTime = 0;
int debounceCount = 0;
bool garageState = false;


// Structure of our payload
struct payload_t
{
  unsigned long ms;
  unsigned long counter;
  float temp;
};
