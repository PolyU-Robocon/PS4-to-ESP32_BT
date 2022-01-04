#include <PS4Controller.h>

#define BT_MAC_ADDR "70:9c:d1:04:9e:75" //black
//#define BT_MAC_ADDR "c0:dc:da:55:e4:e4" //green
//#define BT_MAC_ADDR "e8:9e:b4:f3:66:40" //raman

int r = 255;
int g = 73;
int b = 91;

void esp32_receive() {
  if (PS4.Right()) Serial.println("Right Button");
  if (PS4.Down()) Serial.println("Down Button");
  if (PS4.Up()) Serial.println("Up Button");
  if (PS4.Left()) Serial.println("Left Button");
  
  if (PS4.Square()) Serial.println("Square Button");
  if (PS4.Cross()) Serial.println("Cross Button");
  if (PS4.Circle()) Serial.println("Circle Button");
  if (PS4.Triangle()) Serial.println("Triangle Button");
  
  if (PS4.UpRight()) Serial.println("Up Right");
  if (PS4.DownRight()) Serial.println("Down Right");
  if (PS4.UpLeft()) Serial.println("Up Left");
  if (PS4.DownLeft()) Serial.println("Down Left");
  
  if (PS4.L1()) Serial.println("L1 Button");
  if (PS4.R1()) Serial.println("R1 Button");
  
  if (PS4.Share()) Serial.println("Share Button");
  if (PS4.Options()) Serial.println("Options Button");
  if (PS4.L3()) Serial.println("L3 Button");
  if (PS4.R3()) Serial.println("R3 Button");
  
  if (PS4.PSButton()) Serial.println("PS Button");
  if (PS4.Touchpad()) Serial.println("Touch Pad Button");
  
  if (PS4.L2()) {
    Serial.printf("L2 button at %d\n", PS4.L2Value());
  }
  if (PS4.R2()) {
    Serial.printf("R2 button at %d\n", PS4.R2Value());
  }
  
  if (PS4.LStickX()) {
    Serial.printf("Left Stick x at %d\n", PS4.LStickX());
  }
  if (PS4.LStickY()) {
    Serial.printf("Left Stick y at %d\n", PS4.LStickY());
  }
  if (PS4.RStickX()) {
    Serial.printf("Right Stick x at %d\n", PS4.RStickX());
  }
  if (PS4.RStickY()) {
    Serial.printf("Right Stick y at %d\n", PS4.RStickY());
  }
  
  if (PS4.Charging()) Serial.println("The controller is charging");
  if (PS4.Audio()) Serial.println("The controller has headphones attached");
  if (PS4.Mic()) Serial.println("The controller has a mic attached");
  
  Serial.printf("Battery Level : %d\n", PS4.Battery());
  
  Serial.println();
  // This delay is to make the output more human readable
  // Remove it when you're not trying to see the output
}

// Calculates the next value in a rainbow sequence
void nextRainbowColor() {
  if (r > 0 && b == 0) {
    r--;
    g++;
  }
  if (g > 0 && r == 0) {
    g--;
    b++;
  }
  if (b > 0 && g == 0) {
    r++;
    b--;
  }
}

void esp32_sending() {
  PS4.setLed(r, g, b);
  nextRainbowColor();
  
  PS4.setFlashRate(PS4.LStickY() * 10, PS4.RStickY() * 10);
  
  PS4.setRumble(PS4.L2Value(), PS4.R2Value());

  PS4.sendToController();
  delay(10);
}

void setup() {
  Serial.begin(115200);
  while(!Serial){
  }
  PS4.begin(BT_MAC_ADDR);
  Serial.println("Ready.");
  
  //esp32_sending();
}

void loop() {
  // Below has all accessible outputs from the controller
  if (PS4.isConnected()) {
    esp32_receive();
    PS4.setLed(r, g, b);
    PS4.sendToController();
  } else {
    Serial.println("Disconnect!!");
  }
  delay(10);
}
