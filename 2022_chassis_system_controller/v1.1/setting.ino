void chassis_speed() {
  if (Xbox.getButtonClick(LB)) {
    //button[8] = !button[8];
    //button[8] = Xbox.getButtonClick(LB);
    //Serial.println("LB");
    
    if (speed_time > -5) {
      speed_time--;
//      Serial.print("speed Time:");
//      Serial.println(speed_time);
      max_speed = max_speed - 10;
      max_rotation_speed = max_rotation_speed - 5;
//      Serial.print("max_speed:");
//      Serial.println(max_speed);
    }
  }

  if (Xbox.getButtonClick(RB)) {
    //button[9] = !button[9];
    //button[9] = Xbox.getButtonClick(RB);
    //Serial.println("RB");

    if (speed_time < 5) {
      speed_time++;
//      Serial.print("speed Time:");
//      Serial.println(speed_time);
      max_speed = max_speed + 10;
      max_rotation_speed = max_rotation_speed + 5;
//      Serial.print("max_speed:");
//      Serial.println(max_speed);
    }
    
  } else if (speed_time == 0) {
    max_speed = 50;
    max_rotation_speed = 25;
//    Serial.print("max_speed:");
//    Serial.println(max_speed);
  }
}

void control_mac() {
  if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500 || Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500 || Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500 || Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
    if (Xbox.getAnalogHat(RightHatX) > 7500) {
      X_axis =  (int16_t)(soften_value(mapfloat(axis[2], 7500, 32767, zero, max_speed), softening, max_speed) * multiple);  //RightHatX
      
//      Serial.print("X_axis:  ");
//      Serial.print(X_axis);
//      Serial.print("RightHatX:  ");
//      Serial.println(Xbox.getAnalogHat(RightHatX));
    }

    if (Xbox.getAnalogHat(RightHatX) < -7500) {
      X_axis =  (int16_t)(soften_value(mapfloat(axis[2], -32767, -7500, -max_speed, zero), softening, max_speed) * multiple);  //RightHatX
      
//      Serial.print("X_axis:  ");
//      Serial.print(X_axis);
//      Serial.print("RightHatX:  ");
//      Serial.println(Xbox.getAnalogHat(RightHatX));
    }

    if (Xbox.getAnalogHat(RightHatY) > 7500) {
      Y_axis =  (int16_t)(soften_value(mapfloat(axis[3], 7500, 32767, zero, max_speed), softening, max_speed) * multiple); //RightHatY
//      Serial.print("Y_axis:  ");
//      Serial.print(Y_axis);
//      Serial.print("RightHatY:  ");
//      Serial.println(Xbox.getAnalogHat(RightHatY));
    }

    if (Xbox.getAnalogHat(RightHatY) < -7500) {
      Y_axis =  (int16_t)(soften_value(mapfloat(axis[3], -32767, -7500, -max_speed, zero), softening, max_speed) * multiple); //RightHatY
//      Serial.print("Y_axis:  ");
//      Serial.print(Y_axis);
//      Serial.print("RightHatY:  ");
//      Serial.println(Xbox.getAnalogHat(RightHatY));
    }

    if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500) {
      W_axis =  (int16_t)(soften_value(mapfloat(axis[0], 7500, 32767, zero, max_rotation_speed), softening, max_speed) * rotation_multiple);  //LeftHatX
//      Serial.print("W_axis:  ");
//      Serial.print(W_axis);
//      Serial.print("LightHatX:  ");
//      Serial.println(Xbox.getAnalogHat(LeftHatX));
    }

    if (Xbox.getAnalogHat(LeftHatX) < -7500) {
      W_axis =  (int16_t)(soften_value(mapfloat(axis[0], -32767, -7500, -max_rotation_speed, zero), softening, max_speed) * rotation_multiple);  //LeftHatX
//      Serial.print("W_axis:  ");
//      Serial.print(W_axis);
//      Serial.print("LightHatX:  ");
//      Serial.println(Xbox.getAnalogHat(LeftHatX));
    }
  } else {
    X_axis = 0;
    Y_axis = 0;
    W_axis = 0;
  }
}


void xbox_can_send() {
  Txmsg.can_id = analog_can_id;
  Txmsg.can_dlc = 6;
  Txmsg.data[0] = X_axis >> 8;
  Txmsg.data[1] = X_axis & 0xFF;
  Txmsg.data[2] = Y_axis >> 8;
  Txmsg.data[3] = Y_axis & 0xFF;
  Txmsg.data[4] = W_axis >> 8;
  Txmsg.data[5] = W_axis & 0xFF;
  mcp2515.sendMessage(&Txmsg);

  Txmsg2.can_id = btn_can_id;
  Txmsg2.can_dlc = 4;
  Txmsg2.data[0] = leftRumble >> 8;
  Txmsg2.data[1] = leftRumble & 0xFF;
  Txmsg2.data[2] = rightRumble >> 8;
  Txmsg2.data[3] = rightRumble & 0xFF;
  mcp2515.sendMessage(&Txmsg2);
}

void ButtonPress() {
  // Set rumble effect
  static uint16_t oldLTValue, oldRTValue;
  if (Xbox.getButtonPress(LT) != oldLTValue || Xbox.getButtonPress(RT) != oldRTValue) {
    oldLTValue = Xbox.getButtonPress(LT);
    oldRTValue = Xbox.getButtonPress(RT);
    leftRumble = map(oldLTValue, 0, 1023, 0, 255); // Map the trigger values into a byte
    rightRumble = map(oldRTValue, 0, 1023, 0, 255);
    if (leftRumble > 0 || rightRumble > 0) {
      Xbox.setRumbleOn(leftRumble, rightRumble, leftRumble, rightRumble);
    } else {
      Xbox.setRumbleOff();
    }
  }
}

void AnalogHat() {
  if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500 || Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500 || Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500 || Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
      if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500) {
        //LeftHatX
        axis[0] = Xbox.getAnalogHat(LeftHatX);//  Left: -, Right: +
//        Serial.print("LX:  ");
//        Serial.print(axis[0]);        
      }

      if (Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500) {
        //LeftHatY
        axis[1] = Xbox.getAnalogHat(LeftHatY);  //Forword: -, Back: +
//        Serial.print("LY:  ");
//        Serial.print(axis[1]);
      }

      if (Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500) {
        //RightHatX
        axis[2] = Xbox.getAnalogHat(RightHatX);
//        Serial.print("RX:  ");
//        Serial.print(axis[2]);
      }

      if (Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
        //RightHatY
        axis[3] = Xbox.getAnalogHat(RightHatY);
//        Serial.print("RY:  ");
//        Serial.print(axis[3]);
        
      }
    }
}

void ButtonClick() {
  if (Xbox.getButtonClick(UP)) {
    //Up
    button[0] = !button[0];
    button[0] = Xbox.getButtonClick(UP);
  }
      
  if (Xbox.getButtonClick(DOWN)) {
    //Down
    button[1] = !button[1];
    button[1] = Xbox.getButtonClick(DOWN);
  }

  if (Xbox.getButtonClick(LEFT)) {
    //Left
    button[2] = !button[2];
    button[2] = Xbox.getButtonClick(LEFT);
  }

  if (Xbox.getButtonClick(RIGHT)) {
    //Right
    button[3] = !button[3];
    button[3] = Xbox.getButtonClick(RIGHT);
  }

  if (Xbox.getButtonClick(A)) {
    button[4] = !button[4];
    button[4] = Xbox.getButtonClick(A);
  }
  
  if (Xbox.getButtonClick(B)) {
    button[5] = !button[5];
    button[5] = Xbox.getButtonClick(B);
  }

  if (Xbox.getButtonClick(X)) {
    button[6] = !button[6];
    button[6] = Xbox.getButtonClick(X);
  }

  if (Xbox.getButtonClick(Y)) {
    button[7] = !button[7];
    button[7] = Xbox.getButtonClick(Y);
  }
//
//  if (Xbox.getButtonClick(LB)) {
//    button[8] = !button[8];
//    button[8] = Xbox.getButtonClick(LB);
//    //Serial.println(Xbox.getButtonClick(LB));
//    Serial.println(button[8]);
//  }
//
//  if (Xbox.getButtonClick(RB)) {
//    button[9] = !button[9];
//    button[9] = Xbox.getButtonClick(RB);
//    Serial.println(button[9]);
//  }
  
  if (Xbox.getButtonClick(LT)) {
    button[10] = !button[10];
    button[10] = Xbox.getButtonClick(LT);
  }

  if (Xbox.getButtonClick(RT)) {
    button[11] = !button[11];
    button[11] = Xbox.getButtonClick(RT);
  }

  if (Xbox.getButtonClick(L3)) {
    button[12] = !button[12];
    button[12] = Xbox.getButtonClick(L3);
  }

  if (Xbox.getButtonClick(R3)) {
    button[13] = !button[13];
    button[13] = Xbox.getButtonClick(R3);
  }
  
  if (Xbox.getButtonClick(VIEW)) {
    button[14] = !button[14];
    button[14] = Xbox.getButtonClick(VIEW);
  }
  
  if (Xbox.getButtonClick(MENU)) {
    button[15] = !button[15];
    button[15] = Xbox.getButtonClick(MENU);
  }
  
  for(int i = 0; i <= 16; i++){
    btn_state = btn_state + (button[i] * (pow(2,i)));
    i++;
  }

  if (Xbox.getButtonClick(XBOX)) {
    Xbox.disconnect();
  }
}


void xbox_cal_chassis(){
  //  motor[0] = (int16_t) - ceil((Y_axis * cos(theta + (PI / 4))     - X_axis * sin(theta + (PI / 4))     + W_axis * radius_r) / radius_w);
  //  motor[1] = (int16_t)   ceil((Y_axis * cos(theta + (3 * PI / 4)) - X_axis * sin(theta + (3 * PI / 4)) - W_axis * radius_r) / radius_w);
  //  motor[2] = (int16_t) - ceil((Y_axis * cos(theta + (5 * PI / 4)) - X_axis * sin(theta + (5 * PI / 4)) + W_axis * radius_r) / radius_w);
  //  motor[3] = (int16_t)   ceil((Y_axis * cos(theta + (7 * PI / 4)) - X_axis * sin(theta + (7 * PI / 4)) - W_axis * radius_r) / radius_w);
    
  //  Serial.print("moter0: ");
  //  Serial.println(motor[0]);
  //  Serial.print("moter1: ");
  //  Serial.println(motor[1]);
  //  Serial.print("moter2:  ");
  //  Serial.println(motor[2]);
  //  Serial.print("moter3:  ");
  //  Serial.println(motor[3]);
}
