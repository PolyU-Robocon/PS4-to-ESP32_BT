long mapfloat(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

float soften_value(float axis, float soften, float max_v){
  return axis * pow(abs(axis/max_v), soften);
}
