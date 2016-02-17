/**
* Sensor message (type 'S') 
*/
 
struct S_message
{
    float temp_reading;
    float humidity_reading;
    int battery;
    
  //S_message(void): temp_reading(0), voltage_reading(0), counter(next_counter++) {}
  //char* toString(void);
};

/**
 * Sensor message (type 'M')
 */

struct M_message
{
    int moisture_reading;
    int battery;
    //S_message(void): temp_reading(0), voltage_reading(0), counter(next_counter++) {}
    //char* toString(void);
};


/**
 * Sensor message (type 'G')
 */

struct G_message
{
    int battery;
    //S_message(void): temp_reading(0), voltage_reading(0), counter(next_counter++) {}
    //char* toString(void);
};
