// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/pair.h>

#include "lib/infrastructure/include/Context.h"

#include "lib/api/include/DecoderFacade.h"

#include <boost/algorithm/string.hpp>

#include <memory>
#include <string>
#include <sstream>
#include <ranges>

class DecoderFacadeWrapper
{
    static std::weak_ptr<infrastructure::Context> context;

    static std::shared_ptr<infrastructure::Context> getContext()
    {
        auto existing = context.lock();
        if (existing)
        {
            return existing;
        }

        auto created = std::make_shared<infrastructure::Context>(infrastructure::LoggerFactory::createLazy(false, true));
        context = created;
        return created;
    }

    struct Instance
    {
        std::shared_ptr<infrastructure::Context> context;
        api::DecoderFacade facade;

        Instance(std::string uicPublicKeyXmlFile, std::string vdvCertificateLdifFile, bool const failOnDecoderError, bool const failOnInterpreterError)
            : context(getContext()),
              facade(api::DecoderFacade::create(*context)
                         .withUicPublicKeyXmlFile(std::move(uicPublicKeyXmlFile))
                         .withVdvCertificateLdifFile(std::move(vdvCertificateLdifFile))
                         .withFailOnDecoderError(failOnDecoderError)
                         .withFailOnInterpreterError(failOnInterpreterError)
                         .build())
        {
        }
    };

    std::shared_ptr<Instance> instance;

    api::DecoderFacade &get() { return instance->facade; }

public:
    DecoderFacadeWrapper(
        std::string uicPublicKeyXmlFile,
        std::string vdvCertificateLdifFile,
        bool const failOnDecoderError,
        bool const failOnInterpreterError)
        : instance(std::make_shared<Instance>(std::move(uicPublicKeyXmlFile), std::move(vdvCertificateLdifFile), failOnDecoderError, failOnInterpreterError)) {}

    DecoderFacadeWrapper(DecoderFacadeWrapper const &) = default;
    DecoderFacadeWrapper &operator=(DecoderFacadeWrapper const &) = default;

    std::string decodeBytes(std::vector<std::uint8_t> bytes, std::string const &origin)
    {
        return get().decodeRawBytesToJson(std::move(bytes), origin);
    }

    std::string decodeBase64(std::string const &base64, std::string const &origin)
    {
        return get().decodeRawBase64ToJson(base64, origin);
    }

    std::string decodeFiles(
        std::string const &path,
        int const rotationDegree,
        unsigned int const scalePercent,
        std::string const &splittingMode,
        unsigned int const flippingMode)
    {
        auto preProcessorOptions = std::make_optional(dip::PreProcessorOptions{rotationDegree, scalePercent, splittingMode, flippingMode});
        auto result = get().decodeImageFilesToJson(path, std::move(preProcessorOptions))
                    | std::views::transform([](auto &&pair) { return std::move(pair.second); });

        auto resultJson = std::vector<std::string>(std::begin(result), std::end(result));
        auto json = std::stringstream();
        json << "[" << boost::algorithm::join(resultJson, ",\n") << "]";
        return json.str();
    }
};

std::weak_ptr<infrastructure::Context> DecoderFacadeWrapper::context;

NB_MODULE(ticket_decoder, m)
{
    using namespace nanobind::literals;

    nanobind::register_exception_translator(
        [](const std::exception_ptr &p, void * /* unused */)
        {
            try
            {
                std::rethrow_exception(p);
            }
            catch (std::exception const &x)
            {
                auto message = std::stringstream();
                message << "Decoding failed with: " << x.what();
                PyErr_SetString(PyExc_RuntimeError, message.str().c_str());
            }
        });

    nanobind::class_<DecoderFacadeWrapper>(m, "DecoderFacade")
        .def(nanobind::init<std::string, std::string, bool, bool>(),
             "uic_public_key_xml_file"_a = "cert/UIC_PublicKeys.xml",
             "vdv_certificate_ldif_file"_a = "cert/VDV_Certificates.ldif",
             "fail_on_decoder_error"_a = false,
             "fail_on_interpreter_error"_a = true)
        .def("decode_bytes", &DecoderFacadeWrapper::decodeBytes,
             "bytes"_a,
             "origin"_a = "",
             "Decode raw barcode data into structured json")
        .def("decode_base64", &DecoderFacadeWrapper::decodeBase64,
             "base64"_a,
             "origin"_a = "",
             "Decode base64-encoded raw barcode data into structured json")
        .def("decode_files", &DecoderFacadeWrapper::decodeFiles,
             "path"_a,
             "rotationDegree"_a = dip::PreProcessorOptions::DEFAULT.rotationDegree,
             "scalePercent"_a = dip::PreProcessorOptions::DEFAULT.scalePercent,
             "splittingMode"_a = dip::PreProcessorOptions::DEFAULT.splittingMode,
             "flippingMode"_a = dip::PreProcessorOptions::DEFAULT.flippingMode,
             "Decode Aztec-Code and containing raw data from image/PDF file or files into structured json");
}
