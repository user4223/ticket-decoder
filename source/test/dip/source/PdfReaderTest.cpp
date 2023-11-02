
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "test/support/include/Loader.h"

#include "lib/dip/utility/include/PdfReader.h"

namespace dip::utility
{

    std::filesystem::path getPath(std::string file) 
    {
        return support::Loader::getExecutableFolderPath() / "etc" / "dip" / file;
    }

    TEST(PdfReader, initial)
    {
        auto const input = read(getPath("Muster 918-9 Normalpreis.pdf"));
    }

}
