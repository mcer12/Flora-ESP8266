
void ICACHE_RAM_ATTR shiftSetValue(uint8_t pin, bool value) {
  //(value) ? bitSet(bytes[pinsToRegisterMap[pin]], pinsToBitsMap[pin]) : bitClear(bytes[pinsToRegisterMap[pin]], pinsToBitsMap[pin]);
  (value) ? bitSet(bytes[pin / 8], pin % 8) : bitClear(bytes[pin / 8], pin % 8);

}

/*
  void ICACHE_RAM_ATTR setValue(uint8_t pin, bool value) {
  (value) ? bitSet(bytes[pin / 8], pin % 8) : bitClear(bytes[pin / 8], pin % 8);
  }
*/

void ICACHE_RAM_ATTR shiftSetAll(bool value) {
  for (int i = 0; i < registersCount * 8; i++) {
    //(value) ? bitSet(bytes[pinsToRegisterMap[i]], pinsToBitsMap[i]) : bitClear(bytes[pinsToRegisterMap[i]], pinsToBitsMap[i]);
    (value) ? bitSet(bytes[i / 8], i % 8) : bitClear(bytes[i / 8], i % 8);

  }
}

void ICACHE_RAM_ATTR shiftWriteBytes(byte a1, byte a2, byte a3, byte a4, byte a5, byte a6) {

  SPI.transfer(a1);
  SPI.transfer(a2);
  SPI.transfer(a3);
  SPI.transfer(a4);
  SPI.transfer(a5);
  SPI.transfer(a6);

  // set gpio through register manipulation, fast!
  GPOS = 1 << LATCH;
  GPOC = 1 << LATCH;
  /*
    digitalWrite(LATCH, HIGH);
    digitalWrite(LATCH, LOW);
  */
}

void ICACHE_RAM_ATTR TimerHandler()
{
  for (int i = 0; i < 6; i++) {
    for (int ii = 0; ii < 8; ii++) {
      if (dutyState < segmentBrightness[i][ii]) {
        shiftSetValue(digitPins[i][ii], true);
      } else {
        shiftSetValue(digitPins[i][ii], false);
      }
    }
  }

  shiftWriteBytes(bytes[5], bytes[4], bytes[3], bytes[2], bytes[1], bytes[0]); // Digits are reversed (first shift register = last digit etc.)

  if (dutyState >= pwmResolution) dutyState = 0;
  else dutyState++;
}


void initScreen() {
  pinMode(DATA, INPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(LATCH, OUTPUT);
  digitalWrite(LATCH, LOW);

  bri = json["bri"].as<int>();
  crossFadeTime = json["fade"].as<int>();

  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV16);

  ITimer.attachInterruptInterval(TIMER_INTERVAL_uS, TimerHandler);
  // 30ms slow 240ms, 20 => medium 160ms, 15 => quick 120ms

  if (crossFadeTime > 0) {
    fade_animation_ticker.attach_ms(crossFadeTime, handleFade); // handle dimming animation
  } else {
    fade_animation_ticker.attach_ms(20, handleFade); // handle dimming animation
  }

  blankAllDigits();
}

void enableScreen() {
  ITimer.attachInterruptInterval(TIMER_INTERVAL_uS, TimerHandler);
}
void disableScreen() {
  ITimer.detachInterrupt();
}
/*
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
*/
void handleFade() {
  for (int i = 0; i < 6; i++) {
    for (int ii = 0; ii < 8; ii++) {
      if (crossFadeTime > 0) {
        if (targetBrightness[i][ii] > segmentBrightness[i][ii]) {
          segmentBrightness[i][ii] += bri_vals[bri] / 8;
          if (segmentBrightness[i][ii] > bri_vals[bri]) segmentBrightness[i][ii] = bri_vals[bri];
        } else if (targetBrightness[i][ii] < segmentBrightness[i][ii]) {
          segmentBrightness[i][ii] -= bri_vals[bri] / 8;
          if (segmentBrightness[i][ii] < 0) segmentBrightness[i][ii] = 0;
        }
      } else {
        segmentBrightness[i][ii] = targetBrightness[i][ii];
      }
    }
  }
}

void setDigit(uint8_t digit, uint8_t value) {
  draw(digit, numbers[value]);
}

void setAllDigitsTo(uint16_t value) {
  for (int i = 0; i < 6; i++) {
    setDigit(i, value);
  }
}

void setDot(uint8_t digit, bool enable) {

  if (enable) {
    targetBrightness[digit][7] = bri_vals[bri];
  } else {
    targetBrightness[digit][7] = 0;
  }

}

void draw(uint8_t digit, uint8_t value[8]) {

  for (int i = 0; i < 8; i++) {
    if (value[i] == 1) {
      targetBrightness[digit][i] = bri_vals[bri];
    } else {
      targetBrightness[digit][i] = 0;
    }
  }

}


void blankDigit(uint8_t digit) {
  for (int i = 0; i < sizeof(digitPins[digit]); i++) {
    targetBrightness[digit][i] = 0;
  }
}

void blankAllDigits() {
  for (int i = 0; i < 6; i++) {
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
  strip_show();
  delay(delay_ms);
  strip.ClearTo(RgbColor(0, 0, 0));
  strip_show();
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
