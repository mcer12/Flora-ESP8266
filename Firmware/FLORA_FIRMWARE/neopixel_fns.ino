
void initStrip() {
  strip.Begin();
  strip.ClearTo(RgbColor(0, 0, 0));
  strip_show();
}

void strip_show() {
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

void handleColon() {
  if (json["colon"].as<int>() == 1) { // 0 = off, 1 = always on, 2 = ON/OFF each second
    strip.ClearTo(colonColor);
    return;
  } else if (json["colon"].as<int>() == 2) {
    toggleSeconds = !toggleSeconds;
    if (toggleSeconds) {
      SetupAnimations(colonColor, RgbColor(0, 0, 0));
    }
    else {
      SetupAnimations(RgbColor(0, 0, 0), colonColor);
    }
    return;
  }
  strip.ClearTo(RgbColor(0, 0, 0));
}

struct stripeAnimationState
{
  RgbColor StartingColor;  // the color the animation starts at
  RgbColor EndingColor; // the color the animation will end at
};
stripeAnimationState animationState[PixelCount];

void AnimUpdate(const AnimationParam& param)
{
  if (param.state == AnimationState_Completed) {

  }
  currentColor = RgbColor::LinearBlend(
                   animationState[param.index].StartingColor,
                   animationState[param.index].EndingColor,
                   param.progress);
  // apply the color to the strip
  strip.ClearTo(currentColor);
}

void SetupAnimations(RgbColor StartingColor, RgbColor EndingColor)
{
  // fade all pixels providing a tail that is longer the faster
  // the pixel moves.

  animationState[0].StartingColor = StartingColor;
  animationState[0].EndingColor = EndingColor;
  animations.StartAnimation(0, 150, AnimUpdate);
}
