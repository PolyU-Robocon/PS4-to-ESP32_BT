//Xbox
#include <XBOXONESBT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statement in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the XBOXONESBT class in two ways */
// This will start an inquiry and then pair with the Xbox One S controller - you only have to do this once
// You will need to hold down the Sync and Xbox button at the same time, the Xbox One S controller will then start to blink rapidly indicating that it is in pairing mode
XBOXONESBT Xbox(&Btd, PAIR);
// After that you can simply create the instance like so and then press the Xbox button on the device
//XBOXONESBT Xbox(&Btd);

//CAN
#include <mcp2515.h>

#define analog_can_id 0xA0
#define btn_can_id    0xF0

struct can_frame Txmsg;
struct can_frame Txmsg2;

MCP2515 mcp2515(7);

bool button[16];
uint16_t btn_state;
int32_t axis[6];

int8_t zero = 1;
int8_t max_speed = 50;
int8_t multiple = -100;

int8_t max_rotation_speed = 25;
int16_t rotation_multiple = -100;

int8_t speed_time = 0;

int8_t softening = 0.2;

uint16_t X_axis;
uint16_t Y_axis;
uint16_t W_axis;

uint8_t leftRumble;
uint8_t rightRumble;
/*
int btstate = 0;
bool button[20];
int32_t axis[6];
int16_t motor[4];

//Kaneaitic
int8_t max_speed = 3;
//float PI = 3.14159265358979323846;
float radius_w = 0.1524; 
float radius_r = 0.41233;
float theta = PI / 2;  
*/

void setup() {
  // put your setup code here, to run once:
  //Xbox
  Serial.begin(115200);
  #if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  #endif
  
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  
  Serial.print(F("\r\nXbox One S Bluetooth Library Started"));

  //CAN
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
}


void loop() {
  // put your main code here, to run repeatedly:
  Usb.Task();

  if (Xbox.connected()) {
    AnalogHat();
    ButtonPress();
    xbox_cal_chassis();
    ButtonClick();
    
    chassis_speed();
    control_mac();
    xbox_can_send();

  } else {
    Serial.println("Disconnect!!");
    delay(1);
  }
}
