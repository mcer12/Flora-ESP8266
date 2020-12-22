
void initStrip() {
  strip.Begin();
  strip.ClearTo(RgbColor(0, 0, 0));
  strip_show();
}
void strip_show() {
  noInterrupts();
  strip.Show();
  interrupts();
}
void setTemporaryColonColor(int seconds, RgbColor color) {
  colonColor = color;
  onceTicker.once(seconds, resetColonColor);
}

void resetColonColor() {
  colonColor = colonColorDefault[bri];
}
/*
  void initColon() {
  colonTicker.attach_ms(1000, handleColon);
  }
*/
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
      strip.ClearTo(RgbColor(0, 0, 0));
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
  //strip.SetPixelColor(1, currentColor);
  //strip.SetPixelColor(2, currentColor);
  //strip.SetPixelColor(3, currentColor);
  strip.ClearTo(currentColor);
}
/*
  void LoopAnimUpdate(const AnimationParam& param)
  {
  // wait for this animation to complete,
  // we are using it as a timer of sorts
  if (param.state == AnimationState_Completed)
  {
    // done, time to restart this position tracking animation/timer
    animations.RestartAnimation(param.index);

    // pick the next pixel inline to start animating
    //
    frontPixel = (frontPixel + 1) % PixelCount; // increment and wrap
    if (frontPixel == 0)
    {
      // we looped, lets pick a new front color
      frontColor = RgbColor(random(360) / 360.0f, 1.0f, 0.25f);
    }

    uint16_t indexAnim;
    // do we have an animation available to use to animate the next front pixel?
    // if you see skipping, then either you are going to fast or need to increase
    // the number of animation channels
    if (animations.NextAvailableAnimation(&indexAnim, 1))
    {
      animationState[indexAnim].StartingColor = frontColor;
      animationState[indexAnim].EndingColor = RgbColor(0, 0, 0);
      animationState[indexAnim].IndexPixel = frontPixel;

      animations.StartAnimation(indexAnim, PixelFadeDuration, FadeOutAnimUpdate);
    }
  }
  }
*/
void SetupAnimations(RgbColor StartingColor, RgbColor EndingColor)
{
  // fade all pixels providing a tail that is longer the faster
  // the pixel moves.

  animationState[0].StartingColor = StartingColor;
  animationState[0].EndingColor = EndingColor;
  animations.StartAnimation(0, 150, AnimUpdate);
}
