#pragma once

namespace utility
{
  unsigned int safeDecrement(unsigned int &value);

  unsigned int safeDecrement(unsigned int &value, unsigned int decrement);

  unsigned int safeIncrement(unsigned int &value, unsigned int max);

  unsigned int safeIncrement(unsigned int &value, unsigned int increment, unsigned int max);

  unsigned int rotate(unsigned int &value, unsigned int max);
}
