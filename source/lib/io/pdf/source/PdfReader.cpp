#include "../include/PdfReader.h"

#include "lib/utility/include/Logging.h"

#include <numeric>

#include <opencv2/imgproc.hpp>

#include "poppler/cpp/poppler-document.h"
#include "poppler/cpp/poppler-page-renderer.h"
#include "poppler/cpp/poppler-page.h"

namespace io::pdf
{
    struct PdfReader::Internal
    {
        poppler::page_renderer renderer;
        unsigned int const dpi;

        Internal(unsigned int const _dpi)
            : renderer(poppler::page_renderer()), dpi(_dpi)
        {
            renderer.set_render_hint(poppler::page_renderer::text_antialiasing);
        }
    };

    PdfReader::PdfReader(::utility::LoggerFactory &loggerFactory, unsigned int const _dpi)
        : logger(CREATE_LOGGER(loggerFactory)), internal(std::make_shared<Internal>(_dpi))
    {
    }

    std::vector<std::string> PdfReader::supportedExtensions() const
    {
        return {"pdf"};
    }

    api::ReadResult PdfReader::read(std::filesystem::path path) const
    {
        return read(path, {0});
    }

    api::ReadResult PdfReader::read(std::filesystem::path path, std::vector<unsigned int> pageIndexes) const
    {
        Reader::validate(path, supportedExtensions());
        LOG_DEBUG(logger) << "Reading input: " << path;

        auto const *const document = poppler::document::load_from_file(path);
        auto const pagesCount = document->pages();

        return api::ReadResult(std::reduce(pageIndexes.cbegin(), pageIndexes.cend(), std::vector<cv::Mat>{},
                                           [this, document](std::vector<cv::Mat> result, auto const index)
                                           {
            auto *const page = document->create_page(index);
            auto image = internal->renderer.render_page(page, internal->dpi, internal->dpi);
            auto clone = cv::Mat{};
            auto const format = image.format();
            if (format == poppler::image::format_rgb24)
            {
                cv::Mat(image.height(), image.width(), CV_8UC3, image.data()).copyTo(clone);
            }
            else if (format == poppler::image::format_argb32)
            {
                cv::Mat(image.height(), image.width(), CV_8UC4, image.data()).copyTo(clone);
            }
            else if (format == poppler::image::format_gray8 || format == poppler::image::format_mono)
            {
                cv::Mat(image.height(), image.width(), CV_8UC1, image.data()).copyTo(clone);
            }
            else
            {
                throw std::runtime_error("Unsupported pdf render result format detected: " + std::to_string(image.format()));
            } 
            result.push_back(std::move(clone));
            return std::move(result); }));
    }
}
