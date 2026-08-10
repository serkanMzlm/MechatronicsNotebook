#define REMOTEXY_MODE__ESP8266WIFI_LIB_POINT
#include <ESP8266WiFi.h>

#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "SM"
#define REMOTEXY_WIFI_PASSWORD "123456789"
#define REMOTEXY_SERVER_PORT 6377

#define HEADER_   27
#define FOOTHER_  71


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 77 bytes
  { 255,5,0,0,0,70,0,16,24,0,5,1,2,29,30,30,6,26,31,5,
  4,68,28,30,30,6,26,31,129,0,35,4,31,2,17,68,105,115,97,114,
  109,32,32,32,65,114,109,32,32,32,32,84,97,107,101,111,102,102,32,32,
  32,76,97,110,100,105,110,103,0,3,133,30,8,38,8,2,26 };

  
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  int8_t left_joy_x; // from -100 to 100  
  int8_t left_joy_y; // from -100 to 100  
  int8_t right_joy_x; // from -100 to 100  
  int8_t right_joy_y; // from -100 to 100  
  uint8_t button; // =0 if select position A, =1 if position B, =2 if position C, ... 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

uint8_t  buffer[7];
uint8_t data_lenght;
uint64_t prev_time;

void setup() 
{
  RemoteXY_Init (); 
  Serial.begin(9600);

  buffer[0] = HEADER_;
  buffer[6] = FOOTHER_;
  prev_time = millis();
  data_lenght = sizeof(buffer);
}

void loop() 
{ 
  RemoteXY_Handler ();
  buffer[1] = (uint8_t)map(RemoteXY.left_joy_x ,-100, 100, 0, 200);
  buffer[2] = (uint8_t)map(RemoteXY.left_joy_y ,-100, 100, 0, 200);
  buffer[3] = (uint8_t)map(RemoteXY.right_joy_x,-100, 100, 0, 200);
  buffer[4] = (uint8_t)map(RemoteXY.right_joy_y,-100, 100, 0, 200);


  switch(RemoteXY.button){
    case 1:
      buffer[5] = 1;
      break;
    case 2:
      buffer[5] = 2;
      break;
    case 3:
      buffer[5] = 3;
      break;
    case 4:
      buffer[5] = 4;
      break;
    default:  
      buffer[5] = 0;
      break;
  }

  if((millis() - prev_time) > 5){
    prev_time = millis();
    transmitBuffer();
  }
}

void transmitBuffer(){

  Serial.write(buffer, data_lenght);
}





