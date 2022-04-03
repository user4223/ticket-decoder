
#include "lib/include/PreProcessor.h"

#include <memory>

int main(int argc, char **argv)
{
   auto const processor = std::make_unique<PreProcessor>();
   processor->process();
   return 0;
}
