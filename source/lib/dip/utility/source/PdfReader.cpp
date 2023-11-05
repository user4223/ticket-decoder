#include "../include/PdfReader.h"

// #include "poppler/cpp/poppler-document.h"
// #include "poppler/cpp/poppler-page.h"

namespace dip::utility
{

    cv::Mat read(std::filesystem::path const &path)
    {
        // auto const * const document = poppler::document::load_from_file(path);
        // auto const pagesCount = document->pages();
        //
        // for (auto p = 0; p < pagesCount; ++p)
        // {
        //     document->create_page(p)->text().to_latin1().c_str();
        // }

        return cv::Mat{};
    }

}
