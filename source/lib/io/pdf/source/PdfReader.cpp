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
        api::ReaderOptions options;

        Internal(api::ReaderOptions o)
            : renderer(poppler::page_renderer()), options(o)
        {
            renderer.set_render_hint(poppler::page_renderer::antialiasing, false);
            renderer.set_render_hint(poppler::page_renderer::text_antialiasing, false);
            renderer.set_render_hint(poppler::page_renderer::text_hinting, false);
            // renderer.set_line_mode(poppler::page_renderer::line_shape);
            if (options.grayscale)
            {
                renderer.set_image_format(poppler::image::format_gray8);
            }
        }
    };

    PdfReader::PdfReader(::utility::LoggerFactory &loggerFactory, api::ReaderOptions o)
        : logger(CREATE_LOGGER(loggerFactory)), internal(std::make_shared<Internal>(std::move(o)))
    {
    }

    std::vector<std::string> PdfReader::supportedExtensions() const
    {
        return {".pdf"};
    }

    std::vector<unsigned int> PdfReader::selectedPages(std::vector<unsigned int> const &pageIndexes, unsigned int pageCount)
    {
        if (pageCount == 0)
        {
            return {};
        }

        if (pageIndexes.empty())
        {
            auto pages = std::vector<unsigned int>(pageCount);
            std::iota(pages.begin(), pages.end(), 0);
            return pages;
        }

        auto pages = std::vector<unsigned int>();
        std::copy_if(pageIndexes.begin(), pageIndexes.end(), std::inserter(pages, pages.begin()), [&](auto const v)
                     { return v < pageCount; });
        return pages;
    }

    api::ReadResult PdfReader::read(std::filesystem::path path) const
    {
        Reader::validate(path, supportedExtensions());
        LOG_DEBUG(logger) << "Reading input: " << path;

        auto const *const document = poppler::document::load_from_file(path);
        auto const pages = selectedPages(internal->options.pageIndexes, document->pages());
        return api::ReadResult(std::accumulate(pages.cbegin(), pages.cend(), std::vector<cv::Mat>{},
                                               [internal = this->internal, document](auto &&result, auto index)
                                               {
            if (index >= document->pages()) 
            {
                return std::move(result);
            }
            auto *const page = document->create_page(index);
            auto const dpi = internal->options.dpi;
            auto image = internal->renderer.render_page(page, dpi, dpi);
            auto clone = cv::Mat{};
            auto const format = image.format();
            if (format == poppler::image::format_rgb24)
            {
                cv::Mat(image.height(), image.width(), CV_8UC3, image.data(), image.bytes_per_row()).copyTo(clone);
            }
            else if (format == poppler::image::format_argb32)
            {
                cv::Mat(image.height(), image.width(), CV_8UC4, image.data(), image.bytes_per_row()).copyTo(clone);
            }
            else if (format == poppler::image::format_gray8 || format == poppler::image::format_mono)
            {
                cv::Mat(image.height(), image.width(), CV_8UC1, image.data(), image.bytes_per_row()).copyTo(clone);
            }
            else
            {
                throw std::runtime_error("Unsupported pdf render result format detected: " + std::to_string(image.format()));
            }
            result.emplace_back(std::move(clone));
            return std::move(result); }));
    }
}
