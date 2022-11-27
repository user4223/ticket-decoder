#include <gtest/gtest.h>

#include "lib/uic918/detail/include/UicSignatureChecker.h"
#include "lib/utility/include/Logging.h"
#include "lib/utility/include/Base64.h"

#include "test/support/include/Loader.h"

#include <filesystem>

namespace uic918::detail
{
  TEST(SignatureChecker, certificateNotFound)
  {
    EXPECT_EQ(api::SignatureChecker::Result::KeyNotFound, support::getSignatureChecker()->check("4242", "32323", {}, {}));
  }

  TEST(SignatureChecker, valid_1080_00002)
  {
    // from UIC918_9_FV_SuperSparpreis
    auto const signature = ::utility::base64::decode("eC4v4YSh2F6J6TOLKY7GGuuiSM5yIFbKlAqWfIodORJuLGKMT86pG6NSFqCjUPiU3l69e4kJkg/elH/u3g4gxA==");
    EXPECT_EQ(64, signature.size());
    auto const compressedMessage = ::utility::base64::decode("eJwBvABD/1VfRkxFWDEzMDE4OGKyAIbhDcEl6igVEQiBBRyERGTZhWaOI6AKgAAOlsLk5ujK3Aiu0tjZAQRE174BACIc5hDqVZtkNkw4qCNh0cteHl0yo9CXm9CZyEJrC3NzQytLaFKTK6ujY0tzOytxWrNLCdEB4YmBwYHxQkIhUhKRUhKSoXo6kgoRUloJUoIxSVKyCklSmVKCYngIMAGkw4rluzA6znADgAcAFLACQEAPU3VwZXIgU3BhcnByZWlzxB5KAw==");
    EXPECT_EQ(199, compressedMessage.size());
    auto const certificate = UicCertificate{"1080", "00002", "DB Vertrieb GmbH", "SHA256withDSA(2048,256)", "MIIFAzCCBKmgAwIBAgIJAL4b6YtdfC1HMAsGCWCGSAFlAwQDAjBkMQswCQYDVQQGEwJERTELMAkGA1UECAwCSEUxFzAVBgNVBAoMDkRCVmVydHJpZWJHbUJIMRswGQYDVQQLDBJNb2JpbGVUaWNrZXRpbmdPcHMxEjAQBgNVBAMMCVRob3JnZUxvaDAeFw0xOTA1MTMwODM3MzBaFw0yNDA1MTEwODM3MzBaMGQxCzAJBgNVBAYTAkRFMQswCQYDVQQIDAJIRTEXMBUGA1UECgwOREJWZXJ0cmllYkdtQkgxGzAZBgNVBAsMEk1vYmlsZVRpY2tldGluZ09wczESMBAGA1UEAwwJVGhvcmdlTG9oMIIDRzCCAjkGByqGSM44BAEwggIsAoIBAQDvBHnyGImsnwD+u7a+4y8Kds6pJvmicDx//g/SXkj366T81luFYw3qWU6fV2F/p81j2PGfKHGIhhS89CPtBtXdt1cntHhs2B6+08Hmtd5RGGvqQiUuun5WrSloxJVWPfZRIp5BVNYnkybi+J10TsAL4xf1Wy5uWIOa8pQsBAl1ARMSz0vtQ9vUARLzzJtkS1QpAy6XiNVF9LodFUgC17m76NxK7htHcyoPhEnwdkHXP0YCYAMoXZEdhBVHL4kuyAj/+S+d/Fr+k/jRRLUdevrpsTbVttOkhO/uDtiOs1Z2Ou8PHqZDUvV7p7QMM45KDMBEhjqqEaVfkqTxr4DU71jDAiEAxl+jg6oBinGD274AOiOgdpVEG+dPuEVc6Ckiyxgx3ycCggEANO//Pafo6cAEtMmR7EsLc1dq+H2Hf4cX0o5pU1wiA5bY5kibcnmwSZynphoxUPZXAsZdoXw0ugx9Zkj68A/RwVZAyg+tfApfaIZVKp9BIQVAGwyOOHaEJBsdUahshpkM2SvuMNCxmZScnq5rherQvebbvkf5bmLvK4ftrve03lhnu92LbF8F4XTV/vHLtDAvJGo/380EA6yQVwe1lIUNET5vU3GYSoOZNsDFIu7ijl/mt0m8sjduFPVK5ueE4XO+Hal9lc5hYpiQq4AwUqtRsA+A1HAR7h3tu+QsqMo8AhbuxGdY/bipGSWyRcSg1mvLDEctev0rpvN5fX8ZymiCIwOCAQYAAoIBAQCkDpFu1+QttJUDSMJPScErldgepOoTaVSWIEkc8UYAmVgxXr+hF4t5/MAHeh4+kO5VXUA2xYbTiV4aA6fUkDm+6LW1aIG0Z4hE+SX6C8Lt8u0hp1UzQhERCobl1kRgMktipKes5h3aLaQ2Spy7+t8wzb0jScWNirrgtVZGUajcyQCuaZb5QIaQdLCPm0q5qD3PTDKaLxI/eFuIHSvNoh5WYTw9bfXxN//UZ1I+KQn7JKRdnkTHBvPm7Ww40Yo5Kcc45cxyUU6WDtmUcahaFOdpmfVBhCkK3H0oFOkTEXUAEd3irW8d38yq1znv/I+W0sBNjbtRpc59g+aBZO4oX1kDo1MwUTAdBgNVHQ4EFgQUp/Ih719wqFM0rDWnrLE5rfXqGxEwHwYDVR0jBBgwFoAUp/Ih719wqFM0rDWnrLE5rfXqGxEwDwYDVR0TAQH/BAUwAwEB/zALBglghkgBZQMEAwIDRwAwRAIgWY1GPRhkC9r8QC7AD0/Meki49G7MTA8Z7PrSsLCUYLoCIA/Lsca8Bal5cWs7siFlTJKWefb77CNRjNLvWqKbVW28"};
    EXPECT_TRUE(certificate.verify(compressedMessage, signature));
  }

  TEST(SignatureChecker, valid_UIC918_9_FV_SuperSparpreis)
  {
    auto const signature = ::utility::base64::decode("eC4v4YSh2F6J6TOLKY7GGuuiSM5yIFbKlAqWfIodORJuLGKMT86pG6NSFqCjUPiU3l69e4kJkg/elH/u3g4gxA==");
    EXPECT_EQ(64, signature.size());
    auto const compressedMessage = ::utility::base64::decode("eJwBvABD/1VfRkxFWDEzMDE4OGKyAIbhDcEl6igVEQiBBRyERGTZhWaOI6AKgAAOlsLk5ujK3Aiu0tjZAQRE174BACIc5hDqVZtkNkw4qCNh0cteHl0yo9CXm9CZyEJrC3NzQytLaFKTK6ujY0tzOytxWrNLCdEB4YmBwYHxQkIhUhKRUhKSoXo6kgoRUloJUoIxSVKyCklSmVKCYngIMAGkw4rluzA6znADgAcAFLACQEAPU3VwZXIgU3BhcnByZWlzxB5KAw==");
    EXPECT_EQ(199, compressedMessage.size());
    EXPECT_EQ(api::SignatureChecker::Result::Successful, support::getSignatureChecker()->check("1080", "00002", compressedMessage, signature));
  }

  TEST(SignatureChecker, invalid_UIC918_9_FV_SuperSparpreis)
  {
    auto const signature = ::utility::base64::decode("eC4v4YSh2F6J6TOLKY7GGuuiSM5yIFbKlAqWfIodORJuLGKMT86pG6NSFqCjUPiU3l69e4kJkg/elH/u3g4gxA==");
    EXPECT_EQ(64, signature.size());
    auto compressedMessage = ::utility::base64::decode("eJwBvABD/1VfRkxFWDEzMDE4OGKyAIbhDcEl6igVEQiBBRyERGTZhWaOI6AKgAAOlsLk5ujK3Aiu0tjZAQRE174BACIc5hDqVZtkNkw4qCNh0cteHl0yo9CXm9CZyEJrC3NzQytLaFKTK6ujY0tzOytxWrNLCdEB4YmBwYHxQkIhUhKRUhKSoXo6kgoRUloJUoIxSVKyCklSmVKCYngIMAGkw4rluzA6znADgAcAFLACQEAPU3VwZXIgU3BhcnByZWlzxB5KAw==");
    compressedMessage[23] = 42;
    EXPECT_EQ(199, compressedMessage.size());
    EXPECT_EQ(api::SignatureChecker::Result::Failed, support::getSignatureChecker()->check("1080", "00002", compressedMessage, signature));
  }

  TEST(SignatureChecker, valid_0080_00007)
  {
    // from UIC918_3_Quer_durchs_Land_Ticket
    auto const signature = ::utility::base64::decode("MCwCFAg/3Fhpe1JJTU2zykgZlmU8rvroAhQWzi/E35MPW079nE+5JctGrcos4wAAAAA=");
    EXPECT_EQ(50, signature.size());
    auto const compressedMessage = ::utility::base64::decode("eJxdUcFKw0AUTEERPAleehQ8eNr43kvSbnootE1spUG0SQS9lNBGE1o8JKn+gn8j3r35G36M76WxSLOHnZ2ZnZ3sxvOJP/AAARwLQIP/OBx3Q0XGv480AvFAtF3wfM8X4zAAC4g0CA3I+m52bK1d10Uy0ApBssm626SF8jbFIitVkLwsVZQvVmnFMoFYGFgCRgxcBg4qUMzin8zh7A0p5DBmKFxkRf5UnQVJySnUERVFlUYk7HljCcFCyUDbBDSlIDO0z0g8/woDrEE8j4LBg5hcezaKpIbeVhQI0J2uk7JMe/VG2nKd27QozV6NUUN96lWSFaukqNJjwN1upPH317rKn1dpXvX4NEeEvZZc221omWxeUbPC7a3p5m5jf6a8eDaahCoY3Hgquh5N/UgK38f105Kx5HdstS4+XuN2dfDZfuubE6N/2TaMk0zIo5/Dk9N3o/ULB996SQ==");
    EXPECT_EQ(346, compressedMessage.size());
    auto const certificate = UicCertificate{"0080", "00007", "DB Netz AG", "SHA1withDSA(1024,160)", "MIID0zCCA5GgAwIBAgIJAMbpeJTPSaoCMAsGCWCGSAFlAwQDAjCBoDELMAkGA1UEBhMCREUxDzANBgNVBAgMBkhlc3NlbjESMBAGA1UEBwwJRnJhbmtmdXJ0MRkwFwYDVQQKDBBEQiBWZXJ0cmllYiBHbWJIMRAwDgYDVQQLDAdQLkRWTyAzMRMwEQYDVQQDDApUaG9yZ2UgTG9oMSowKAYJKoZIhvcNAQkBFht0aG9yZ2UubG9oQGRldXRzY2hlYmFobi5jb20wHhcNMTUwOTI5MDgzMjE3WhcNMjUwOTI2MDgzMjE3WjCBoDELMAkGA1UEBhMCREUxDzANBgNVBAgMBkhlc3NlbjESMBAGA1UEBwwJRnJhbmtmdXJ0MRkwFwYDVQQKDBBEQiBWZXJ0cmllYiBHbWJIMRAwDgYDVQQLDAdQLkRWTyAzMRMwEQYDVQQDDApUaG9yZ2UgTG9oMSowKAYJKoZIhvcNAQkBFht0aG9yZ2UubG9oQGRldXRzY2hlYmFobi5jb20wggG4MIIBLAYHKoZIzjgEATCCAR8CgYEA2M7klKVQf657OEwFAia/psltoSA9OQw+3zjDF7jIHsQkgbuTUWnXRUUQnJZWLp2cqBfRtuWr45tMFaEqXVo21iuhlZMgx2xcr1Q6+ohBHh9WmdLI9mLda8Rj+KRsT8gJdn3GgihuAZ2QeN4smmEuIb42ynbdORdu/F6emwNNOW0CFQCLu+UpbUKOhrQJUFtpw2AZD8Er/QKBgQCAj8YnWSgd54jKKXJFxiHaN2GqQQ+wnbDeXrQ4JBgAr4zkPUu1Ld4I3OXlWJXdBijvfi2ANTbcD+sAm9NTaG3nXs9o2W0NdK/K0QDg5Xr9eNqWw41hwaf8/pMW+oYjfThUYqb4kIEAcOZF1X1DntNEn1ixiCSUhm6dx061E9jBrgOBhQACgYEAqhL3eCBtEs3P8P0U0r+wmzwnJNY2dtk/AI26omnVmv8iExnYx9sh+6YprNsh34L+lFtfCByskUhalHY28pfqrDc7N7SJBdjeIHiFI6GcZjVivBjaXeEiCkgvzz4yP1bGpx3BYRTdzRAn0XyZvieeSWn+BPLjBDQEOqhv9pwtqxajUDBOMB0GA1UdDgQWBBT6cbqPsxaDeUEnT7SC6+AG6ECq6zAfBgNVHSMEGDAWgBT6cbqPsxaDeUEnT7SC6+AG6ECq6zAMBgNVHRMEBTADAQH/MAsGCWCGSAFlAwQDAgMvADAsAhRQ81F8ZrKZHs6zMxHuQrTAN/UVIAIUIMn7sxtFJFEDadU3gOLxK+irIcY="};
    EXPECT_TRUE(certificate.verify(compressedMessage, signature));
  }
}
