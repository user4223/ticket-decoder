#include <gtest/gtest.h>

#include "lib/utility/include/SignatureChecker.h"
#include "lib/utility/include/Logging.h"
#include "lib/utility/include/Base64.h"

#include "test/support/include/Loader.h"

#include <filesystem>

namespace utility
{
  TEST(SignatureChecker, certificateNotFound)
  {
    EXPECT_EQ(SignatureChecker::Result::KeyNotFound, support::getSignatureChecker()->check("4242", "32323", {}, {}));
  }

  TEST(SignatureChecker, valid_UIC918_9_FV_SuperSparpreis)
  {
    auto const signature = base64::decode("eC4v4YSh2F6J6TOLKY7GGuuiSM5yIFbKlAqWfIodORJuLGKMT86pG6NSFqCjUPiU3l69e4kJkg/elH/u3g4gxA==");
    EXPECT_EQ(64, signature.size());
    auto const message = base64::decode("eJwBvABD/1VfRkxFWDEzMDE4OGKyAIbhDcEl6igVEQiBBRyERGTZhWaOI6AKgAAOlsLk5ujK3Aiu0tjZAQRE174BACIc5hDqVZtkNkw4qCNh0cteHl0yo9CXm9CZyEJrC3NzQytLaFKTK6ujY0tzOytxWrNLCdEB4YmBwYHxQkIhUhKRUhKSoXo6kgoRUloJUoIxSVKyCklSmVKCYngIMAGkw4rluzA6znADgAcAFLACQEAPU3VwZXIgU3BhcnByZWlzxB5KAw==");
    EXPECT_EQ(199, message.size());
    EXPECT_EQ(SignatureChecker::Result::Successful, support::getSignatureChecker()->check("1080", "00002", message, signature));
  }

  TEST(SignatureChecker, invalid_UIC918_9_FV_SuperSparpreis)
  {
    auto const signature = base64::decode("eC4v4YSh2F6J6TOLKY7GGuuiSM5yIFbKlAqWfIodORJuLGKMT86pG6NSFqCjUPiU3l69e4kJkg/elH/u3g4gxA==");
    EXPECT_EQ(64, signature.size());
    auto message = base64::decode("eJwBvABD/1VfRkxFWDEzMDE4OGKyAIbhDcEl6igVEQiBBRyERGTZhWaOI6AKgAAOlsLk5ujK3Aiu0tjZAQRE174BACIc5hDqVZtkNkw4qCNh0cteHl0yo9CXm9CZyEJrC3NzQytLaFKTK6ujY0tzOytxWrNLCdEB4YmBwYHxQkIhUhKRUhKSoXo6kgoRUloJUoIxSVKyCklSmVKCYngIMAGkw4rluzA6znADgAcAFLACQEAPU3VwZXIgU3BhcnByZWlzxB5KAw==");
    message[23] = 42;
    EXPECT_EQ(199, message.size());
    EXPECT_EQ(SignatureChecker::Result::Failed, support::getSignatureChecker()->check("1080", "00002", message, signature));
  }

  TEST(SignatureChecker, valid_UIC918_3_Quer_durchs_Land_Ticket)
  {
    auto const signature = base64::decode("MCwCFAg/3Fhpe1JJTU2zykgZlmU8rvroAhQWzi/E35MPW079nE+5JctGrcos4wAAAAA=");
    EXPECT_EQ(50, signature.size());
    auto const message = base64::decode("eJxdUcFKw0AUTEERPAleehQ8eNr43kvSbnootE1spUG0SQS9lNBGE1o8JKn+gn8j3r35G36M76WxSLOHnZ2ZnZ3sxvOJP/AAARwLQIP/OBx3Q0XGv480AvFAtF3wfM8X4zAAC4g0CA3I+m52bK1d10Uy0ApBssm626SF8jbFIitVkLwsVZQvVmnFMoFYGFgCRgxcBg4qUMzin8zh7A0p5DBmKFxkRf5UnQVJySnUERVFlUYk7HljCcFCyUDbBDSlIDO0z0g8/woDrEE8j4LBg5hcezaKpIbeVhQI0J2uk7JMe/VG2nKd27QozV6NUUN96lWSFaukqNJjwN1upPH317rKn1dpXvX4NEeEvZZc221omWxeUbPC7a3p5m5jf6a8eDaahCoY3Hgquh5N/UgK38f105Kx5HdstS4+XuN2dfDZfuubE6N/2TaMk0zIo5/Dk9N3o/ULB996SQ==");
    EXPECT_EQ(346, message.size());
    EXPECT_EQ(SignatureChecker::Result::Successful, support::getSignatureChecker()->check("0080", "00007", message, signature));
  }
}
