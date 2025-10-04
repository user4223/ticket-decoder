
#include "../include/InteractionController.h"

#include "lib/infrastructure/include/Context.h"

#include "lib/ui/include/KeyMapper.h"
#include "lib/ui/include/Window.h"

#include "lib/utility/include/DebugController.h"
#include "lib/utility/include/Utility.h"

#include "lib/io/api/include/SourceManager.h"

#include "lib/api/include/DecoderFacade.h"
#include "lib/dip/filtering/include/PreProcessor.h"

#include <tclap/CmdLine.h>

#include <filesystem>

int main(int argc, char **argv)
{
    auto cmd = TCLAP::CmdLine("ticket-analyzer", ' ', "v0.13");
    auto const verboseArg = TCLAP::SwitchArg(
        "v", "verbose",
        "More verbose debug logging",
        cmd, false);
    auto const inputFolderPathArg = TCLAP::ValueArg<std::string>(
        "i", "input-folder",
        "Path to folder containing input files with aztec codes to be processed",
        false, "images/", "Directory path to input files [pdf, png, jpeg]", cmd);
    auto const outputFolderPathArg = TCLAP::ValueArg<std::string>(
        "o", "output-folder",
        "Path to folder to take intermediate image and raw data files and json result files",
        false, "out/", "Directory path", cmd);
    auto const publicKeyFilePathArg = TCLAP::ValueArg<std::string>(
        "k", "keys-file",
        "Path to file containing public keys from UIC for signature validation",
        false, "cert/UIC_PublicKeys.xml", "File path [xml]", cmd);
    auto const cameraEnabledArg = TCLAP::SwitchArg(
        "c", "camera-enabled",
        "Enable camera at start and try to detect aztec codes in delivered images",
        cmd, false);
    auto const imageRotationArg = TCLAP::ValueArg<int>(
        "", "rotate-image",
        "Rotate input image before processing for the given amount of degrees (default 0)",
        false, 0, "Integer value", cmd);
    auto const imageSplitArgContraintValues = std::vector<std::string>{"11", "21", "22", "41", "42", "43", "44"};
    auto imageSplitArgContraint = TCLAP::ValuesConstraint<std::string>(imageSplitArgContraintValues);
    auto const imageSplitArg = TCLAP::ValueArg<std::string>(
        "", "split-image",
        "Split input image, 1st number specifies the no of parts to split, 2nd is the part used for processing, clockwise from top/left (default 42)",
        false, "42", &imageSplitArgContraint, cmd);

    try
    {
        cmd.parse(argc, argv);
    }
    catch (TCLAP::ArgException const &e)
    {
        std::cerr << e.what() << std::endl
                  << cmd.getOutput();
        return -1;
    }

    auto const outputFolderPath = std::filesystem::path(outputFolderPathArg.getValue());
    auto const inputFolderPath = std::filesystem::path(inputFolderPathArg.getValue());
    auto const executableFolderPath = std::filesystem::canonical(std::filesystem::current_path() / argv[0]).parent_path();

    auto context = infrastructure::Context(::utility::LoggerFactory::create(verboseArg.getValue()));

    auto interactionController = InteractionController(context,
                                                       io::api::SinkManager::create(context)
                                                           .useDestinationPath(outputFolderPath)
                                                           .build());

    auto decoderFacade = api::DecoderFacade::create(context)
                             .withAsynchronousLoad(true)
                             .withPublicKeyFile(publicKeyFilePathArg.getValue())
                             .withImageRotation(imageRotationArg.getValue())
                             .withImageSplit(imageSplitArg.getValue())
                             .withDetector(dip::detection::api::DetectorType::NOP_FORWARDER)
                             .withClassifierFile(executableFolderPath / "etc" / "dip" / "haarcascade_frontalface_default.xml") // TODO: This is an example only, provide properly trained classification file 2 detect aztec codes!
                             .withPreProcessorResultVisitor(std::bind(&InteractionController::handlePreProcessorResult, &interactionController, std::placeholders::_1))
                             .withDetectorResultVisitor(std::bind(&InteractionController::handleDetectorResult, &interactionController, std::placeholders::_1))
                             .withDecoderResultVisitor(std::bind(&InteractionController::handleDecoderResult, &interactionController, std::placeholders::_1))
                             .withInterpreterResultVisitor(std::bind(&InteractionController::handleInterpreterResult, &interactionController, std::placeholders::_1))
                             .build();

    auto &preProcessor = decoderFacade.getPreProcessor();
    auto &debugController = context.getDebugController();

    auto sourceManager = io::api::SourceManager::create(context,
                                                        decoderFacade.loadSupportedFiles(inputFolderPath),
                                                        [&](bool cameraEnabled)
                                                        { preProcessor.enable(!cameraEnabled); });

    interactionController
        .addParameterSupplier(sourceManager)
        .addParameterSupplier(decoderFacade);

    if (cameraEnabledArg.getValue())
    {
        sourceManager.toggleCamera();
    }

    auto const detectorIndexMax = decoderFacade.getSupportetDetectorTypes().size() - 1;
    auto detectorIndex = dip::detection::api::toInt(decoderFacade.getDetectorType());

    auto const keyMapper = ui::KeyMapper(
        context,
        {{'i', [&]()
          { return "image step: " + std::to_string(debugController.incrementAs<unsigned int>("squareDetector.imageProcessing.step")); }},
         {'I', [&]()
          { return "IMAGE step: " + std::to_string(debugController.decrementAs<unsigned int>("squareDetector.imageProcessing.step")); }},
         {'g', [&]()
          { return "gaussian smooth: " + std::to_string(debugController.incrementRotateAs<int>("squareDetector.imageProcessing.smooth", 2)); }},
         {'c', [&]()
          { return "contour step: " + std::to_string(debugController.incrementAs<unsigned int>("squareDetector.contourDetector.step")); }},
         {'C', [&]()
          { return "CONTOUR step: " + std::to_string(debugController.decrementAs<unsigned int>("squareDetector.contourDetector.step")); }},
         {'f', [&]()
          { return "file: " + sourceManager.next(); }},
         {'F', [&]()
          { return "FILE: " + sourceManager.previous(); }},
         {' ', [&]()
          { return "camera: " + sourceManager.toggleCamera(); }},
         {'r', [&]()
          { return "rotate: " + preProcessor.rotateCCW(); }},
         {'R', [&]()
          { return "ROTATE: " + preProcessor.rotateCW(); }},
         {'2', [&]()
          { return "split 2: " + preProcessor.toggleSplit2(); }},
         {'4', [&]()
          { return "split 4: " + preProcessor.toggleSplit4(); }},
         {'s', [&]()
          { return "scale: " + preProcessor.scaleUp(); }},
         {'S', [&]()
          { return "SCALE: " + preProcessor.scaleDown(); }},
         {'x', [&]()
          { return "flip: " + preProcessor.toggleFlipping(); }},
         {'0', [&]()
          { return "reset: " + preProcessor.reset(); }},
         {'d', [&]()
          { return "detector: " + decoderFacade.setDetectorType(dip::detection::api::fromInt(::utility::rotate(detectorIndex, detectorIndexMax))); }},
         {'p', [&]()
          { return "decoder pure: " + std::to_string(debugController.toggle("aztecDecoder.pure")); }},
         {'b', [&]()
          { return "decoder binarize: " + std::to_string(debugController.toggle("aztecDecoder.binarize")); }},
         {'h', [&]()
          { return "decoder harder: " + std::to_string(debugController.toggle("aztecDecoder.tryHarder")); }},
         {'D', [&]()
          { return "dump results: " + std::to_string(::utility::rotate(interactionController.dumpResults, 1, 0, 2)); }},
         {'o', [&]()
          { return "overlay image: " + std::to_string(interactionController.overlayImage = !interactionController.overlayImage); }},
         {'t', [&]()
          { return "overlay text: " + std::to_string(interactionController.overlayText = !interactionController.overlayText); }}});

    keyMapper.handle([&](bool const keyHandled)
                     {
        auto source = sourceManager.getOrWait();

        interactionController.reset(keyHandled || sourceManager.isCameraEnabled());
        auto const result = decoderFacade.decodeImageToJson(std::move(source));

        if (!result.empty())
        {
            sourceManager.pauseCamera();
        }

        ui::showImage(interactionController.compose()); });

    return 0;
}
