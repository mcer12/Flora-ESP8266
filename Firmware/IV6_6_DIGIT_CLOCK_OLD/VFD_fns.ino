
void initScreen() {
  pwm_ticker.attach_ms(2, handlePWM); // Handle the soft pwm
  fade_animation_ticker.attach_ms(32, handleFade); // handle dimming animation
  blankAllDigits();
}

void handlePWM() {

  for (int i = 0; i < 6; i++) {
    for (int ii = 0; ii < 8; ii++) {
      if (dutyState < segmentBrightness[i][ii]) {
        shift.setNoUpdate(digitPins[i][ii], 1);
      } else {
        shift.setNoUpdate(digitPins[i][ii], 0);
      }
    }
  }
  shift.updateRegisters();

  if (dutyState >= bri_vals[2]) dutyState = 0;
  else dutyState++;

}

void handleFade() {
  for (int i = 0; i < 6; i++) {
    for (int ii = 0; ii < 8; ii++) {
      if (targetBrightness[i][ii] > segmentBrightness[i][ii]) {
        segmentBrightness[i][ii]++;
      } else if (targetBrightness[i][ii] < segmentBrightness[i][ii]) {
        segmentBrightness[i][ii]--;
      }
    }
  }
}

void setDigit(uint8_t digit, uint8_t value) {
  if (digit > 5) return; 

  for (int i = 0; i < 8; i++) {
    //shift.setNoUpdate(digitPins[5 - digit][i], numbers[value][i]);
    if (numbers[value][i] == 1) {
      targetBrightness[digit][i] = bri_vals[bri];
    } else {
      targetBrightness[digit][i] = 0;
    }
  }

}

void setAllDigitsTo(uint16_t value) {
  for (int i = 0; i < 6; i++) {
    setDigit(i, value);
  }
}

void setDot(uint8_t digit, bool enable) {
  targetBrightness[digit][7] = bri_vals[bri];

}

void draw(uint8_t digit, uint8_t value[8]) {
  for (int ii = 0; ii < 8; ii++) {
    targetBrightness[digit][ii] = value[ii];
  }
}


void blankDigit(uint8_t digit) {
  for (int i = 0; i < sizeof(digitPins[digit]); i++) {
    //shift.set(digitPins[5 - digit][i], 0);
    targetBrightness[digit][i] = 0;
  }
}

void blankAllDigits() {
  for (int i = 0; i < 4; i++) {
    blankDigit(i);
  }
}

void showTime() {
  int hours = hour();
  if (hours > 12 && json["t_format"].as<int>() == 0) { // 12 / 24 h format
    hours -= 12;
  }
  int splitTime[6] = {
    (hours / 10) % 10,
    hours % 10,
    (minute() / 10) % 10,
    minute() % 10,
    (second() / 10) % 10,
    second() % 10,
  };

  for (int i = 0; i < 6; i++) {
    if (i == 0 && splitTime[0] == 0 && json["zero"].as<int>() == 0) {
      blankDigit(i);
      continue;
    }
    setDigit(i, splitTime[i]);
  }
}

void cycleDigits() {
  strip.ClearTo(colorStartupDisplay);
  strip.Show();
  for (int i = 0; i < 10; i++) {
    setDigit(0, i);
    setDigit(1, i);
    setDigit(2, i);
    setDigit(3, i);
    setDigit(4, i);
    setDigit(5, i);
    delay(1000);
  }
  strip.ClearTo(RgbColor(0, 0, 0));
  strip.Show();
}

void showIP(int delay_ms) {
  IPAddress ip_addr = WiFi.localIP();
  setDigit(0, 1);
  draw(1, letter_p);

  blankDigit(2);

  if ((ip_addr[3] / 100) % 10 == 0) {
    blankDigit(3);
  } else {
    setDigit(3, (ip_addr[3] / 100) % 10);
  }
  if ((ip_addr[3] / 10) % 10 == 0) {
    blankDigit(4);
  } else {
    setDigit(4, (ip_addr[3] / 10) % 10);
  }
  setDigit(5, (ip_addr[3]) % 10);

  strip.ClearTo(colorStartupDisplay);
  strip.Show();
  delay(delay_ms);
  strip.ClearTo(RgbColor(0, 0, 0));
  strip.Show();
}

void toggleNightMode() {
  if (json["nmode"].as<int>() == 0) return;
  if (hour() >= 22 || hour() <= 6) {
    bri = 0;
    return;
  }
  bri = json["bri"].as<int>();
}
/*
  void healingCycle() {
  strip.ClearTo(RgbColor(0, 0, 0)); // red
  strip.Show();

  for (int i = 0; i < 4; i++) {
    setDigit(i, healPattern[i][healIterator]);
  }
  healIterator++;
  if (healIterator > 9) healIterator = 0;
  }
*/
