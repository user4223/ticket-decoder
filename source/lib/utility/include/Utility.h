#pragma once

namespace utility
{
  unsigned int safeDecrement(unsigned int &value, unsigned int min);

  unsigned int safeIncrement(unsigned int &value, unsigned int max);

  unsigned int safeIncrement(unsigned int &value, unsigned int increment, unsigned int max);

  unsigned int rotate(unsigned int &value, unsigned int max);

  int rotate(int &value, int increment, int max);
}
