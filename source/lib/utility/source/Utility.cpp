
#include "../include/Utility.h"

namespace utility
{
  unsigned int safeDecrement(unsigned int &value, unsigned int min)
  {
    if (value <= min)
    {
      value = min;
      return value;
    }
    return value -= 1;
  }

  unsigned int safeIncrement(unsigned int &value, unsigned int max)
  {
    return safeIncrement(value, 1, max);
  }

  unsigned int safeIncrement(unsigned int &value, unsigned int increment, unsigned int max)
  {
    if ((value + increment) > max)
    {
      value = max;
      return value;
    }
    if (value < max)
    {
      return value += increment;
    }
    return value;
  }

  unsigned int rotate(unsigned int &value, unsigned int max)
  {
    if (value++ >= max)
    {
      value = 0;
    }
    return value;
  }

  int rotate(int &value, int increment, int max)
  {
    value += increment;
    if (value < 0)
    {
      value = value + max;
      return value;
    }
    if (value >= max)
    {
      value = value - max;
    }
    return value;
  }

  int rotate(int &value, int increment, int min, int max)
  {
    value += increment;
    if (value < min)
    {
      value = max;
      return value;
    }
    if (value > max)
    {
      value = min;
      return value;
    }
    return value;
  }
}
