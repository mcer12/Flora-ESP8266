
void initStrip() {
  strip.Begin();
  strip.ClearTo(RgbColor(0, 0, 0));
  strip_show();
}

void strip_show() {
  if (!isPoweredOn) {
    strip.ClearTo(RgbColor(0, 0, 0));
  }
  if (!strip.IsDirty()) return;
  disableScreen();
  strip.Show();
  enableScreen();
}

void setTemporaryColonColor(int seconds, RgbColor color) {
  colonColor = color;
  onceTicker.once(seconds, resetColonColor);
}

void resetColonColor() {
  colonColor = colonColorDefault[bri];
}

void initRgbColon() {
  if (json["rgb"]["en"].as<int>() == 1) {
    // https://github.com/Makuna/NeoPixelBus/wiki/HsbColor-object-API
    float hue = json["rgb"]["h"].as<float>() / 360;
    float sat = json["rgb"]["s"].as<float>() / 100;
    float val[3] = {
      json["rgb"]["v"][0].as<float>() / 100,
      json["rgb"]["v"][1].as<float>() / 100,
      json["rgb"]["v"][2].as<float>() / 100,
    };
    
    // Lets update all colors according to set brightness!
    for (int i = 0; i < 3; i++) {
      colonColorDefault[i] = HsbColor(hue, sat, val[i]); // LOW
      red[i] = HsbColor(red[i].H, red[i].S, val[i]);
      green[i] = HsbColor(green[i].H, green[i].S, val[i]);
      blue[i] = HsbColor(blue[i].H, blue[i].S, val[i]);
      yellow[i] = HsbColor(yellow[i].H, yellow[i].S, val[i]);
      purple[i] = HsbColor(purple[i].H, purple[i].S, val[i]);
      azure[i] = HsbColor(azure[i].H, azure[i].S, val[i]);
    }
  }
  resetColonColor();
}



/*
  void startBlinking(int blinkSpeed, RgbColor color) {
  colonColor = color;
  colonTicker.attach_ms(blinkSpeed, handleBlinking);
  }

  void handleBlinking() {
  if (toggleSeconds) {
    strip.ClearTo(colonColor);
  } else {
    strip.ClearTo(RgbColor(0, 0, 0));
  }
  strip_show();

  toggleSeconds = !toggleSeconds;
  }
*/
void updateColonColor(RgbColor color) {
  RgbColor colorHigh = color;
  RgbColor colorMed = color;
  RgbColor colorLow = color;

  if (json["colon"].as<int>() == 3) {
    colorMed = RgbColor::LinearBlend(color, RgbColor(0, 0, 0), 0.5);
    colorLow = RgbColor::LinearBlend(color, RgbColor(0, 0, 0), 0.7);
  }

  // Gamma correction => linearize brightness
  colorHigh = colorGamma.Correct(colorHigh);
  colorMed = colorGamma.Correct(colorMed);
  colorLow = colorGamma.Correct(colorLow);

  strip.SetPixelColor(2, colorHigh);
  strip.SetPixelColor(3, colorHigh);
  strip.SetPixelColor(1, colorMed);
  strip.SetPixelColor(4, colorMed);
  strip.SetPixelColor(0, colorLow);
  strip.SetPixelColor(5, colorLow);

  strip.SetPixelColor(8, colorHigh);
  strip.SetPixelColor(9, colorHigh);
  strip.SetPixelColor(7, colorMed);
  strip.SetPixelColor(10, colorMed);
  strip.SetPixelColor(6, colorLow);
  strip.SetPixelColor(11, colorLow);
}

void handleColon() {
  // 0 = off, 1 = always on, 2 = ON/OFF each second, 3 = always on with gradient
  if (json["colon"].as<int>() == 0) {
    strip.ClearTo(RgbColor(0, 0, 0));
    return;
  } else if (json["colon"].as<int>() == 1 || json["colon"].as<int>() == 3) {
    updateColonColor(colonColor);
    return;
  } else if (json["colon"].as<int>() == 2) {
    if (toggleSeconds) {
      SetupAnimations(colonColor, RgbColor(0, 0, 0), 150);
    }
    else {
      SetupAnimations(RgbColor(0, 0, 0), colonColor, 150);
    }
    toggleSeconds = !toggleSeconds;
    return;
  }
}

struct stripeAnimationState
{
  RgbColor StartingColor;  // the color the animation starts at
  RgbColor EndingColor; // the color the animation will end at
};
stripeAnimationState animationState[PixelCount];

void AnimUpdate(const AnimationParam& param)
{
  currentColor = RgbColor::LinearBlend(
                   animationState[param.index].StartingColor,
                   animationState[param.index].EndingColor,
                   param.progress);

  // apply the color to the strip
  updateColonColor(currentColor);
}

void SetupAnimations(RgbColor StartingColor, RgbColor EndingColor, int duration)
{
  // fade all pixels providing a tail that is longer the faster
  // the pixel moves.

  animationState[0].StartingColor = StartingColor;
  animationState[0].EndingColor = EndingColor;
  animations.StartAnimation(0, duration, AnimUpdate);
}
