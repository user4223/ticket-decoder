#include <gtest/gtest.h>

#include "lib/utility/include/Base64.h"

namespace utility::base64
{
  TEST(Base64, decodeEmpty)
  {
    EXPECT_EQ((std::vector<std::uint8_t>{}), decode(""));
  }

  TEST(Base64, decodeString)
  {
    EXPECT_EQ((std::vector<std::uint8_t>{'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'}), decode("aGVsbG8gd29ybGQ="));
  }

  TEST(Base64, decodeBytes)
  {
    auto const decoded = decode("MIID0zCCA5GgAwIBAgIJAMbpeJTPSaoCMAsGCWCGSAFlAwQDAjCBoDELMAkGA1UEBhMCREUxDzANBgNVBAgMBkhlc3NlbjESMBAGA1UEBwwJRnJhbmtmdXJ0MRkwFwYDVQQKDBBEQiBWZXJ0cmllYiBHbWJIMRAwDgYDVQQLDAdQLkRWTyAzMRMwEQYDVQQDDApUaG9yZ2UgTG9oMSowKAYJKoZIhvcNAQkBFht0aG9yZ2UubG9oQGRldXRzY2hlYmFobi5jb20wHhcNMTUwOTI5MDgzMjE3WhcNMjUwOTI2MDgzMjE3WjCBoDELMAkGA1UEBhMCREUxDzANBgNVBAgMBkhlc3NlbjESMBAGA1UEBwwJRnJhbmtmdXJ0MRkwFwYDVQQKDBBEQiBWZXJ0cmllYiBHbWJIMRAwDgYDVQQLDAdQLkRWTyAzMRMwEQYDVQQDDApUaG9yZ2UgTG9oMSowKAYJKoZIhvcNAQkBFht0aG9yZ2UubG9oQGRldXRzY2hlYmFobi5jb20wggG4MIIBLAYHKoZIzjgEATCCAR8CgYEA2M7klKVQf657OEwFAia/psltoSA9OQw+3zjDF7jIHsQkgbuTUWnXRUUQnJZWLp2cqBfRtuWr45tMFaEqXVo21iuhlZMgx2xcr1Q6+ohBHh9WmdLI9mLda8Rj+KRsT8gJdn3GgihuAZ2QeN4smmEuIb42ynbdORdu/F6emwNNOW0CFQCLu+UpbUKOhrQJUFtpw2AZD8Er/QKBgQCAj8YnWSgd54jKKXJFxiHaN2GqQQ+wnbDeXrQ4JBgAr4zkPUu1Ld4I3OXlWJXdBijvfi2ANTbcD+sAm9NTaG3nXs9o2W0NdK/K0QDg5Xr9eNqWw41hwaf8/pMW+oYjfThUYqb4kIEAcOZF1X1DntNEn1ixiCSUhm6dx061E9jBrgOBhQACgYEAqhL3eCBtEs3P8P0U0r+wmzwnJNY2dtk/AI26omnVmv8iExnYx9sh+6YprNsh34L+lFtfCByskUhalHY28pfqrDc7N7SJBdjeIHiFI6GcZjVivBjaXeEiCkgvzz4yP1bGpx3BYRTdzRAn0XyZvieeSWn+BPLjBDQEOqhv9pwtqxajUDBOMB0GA1UdDgQWBBT6cbqPsxaDeUEnT7SC6+AG6ECq6zAfBgNVHSMEGDAWgBT6cbqPsxaDeUEnT7SC6+AG6ECq6zAMBgNVHRMEBTADAQH/MAsGCWCGSAFlAwQDAgMvADAsAhRQ81F8ZrKZHs6zMxHuQrTAN/UVIAIUIMn7sxtFJFEDadU3gOLxK+irIcY=");
    EXPECT_EQ((std::vector<std::uint8_t>{0x30, 0x82, 0x03, 0xD3, 0x30, 0x82, 0x03, 0x91}),
              (std::vector<std::uint8_t>{decoded.begin(), decoded.begin() + 8}));
    EXPECT_EQ((std::vector<std::uint8_t>{'H', 'e', 's', 's', 'e', 'n'}),
              (std::vector<std::uint8_t>{decoded.begin() + 64, decoded.begin() + 70}));
    EXPECT_EQ((std::vector<std::uint8_t>{'F', 'r', 'a', 'n', 'k', 'f', 'u', 'r', 't'}),
              (std::vector<std::uint8_t>{decoded.begin() + 81, decoded.begin() + 90}));
    EXPECT_EQ((std::vector<std::uint8_t>{'D', 'B', ' ', 'V', 'e', 'r', 't', 'r', 'i', 'e', 'b', ' ', 'G', 'm', 'b', 'H'}),
              (std::vector<std::uint8_t>{decoded.begin() + 101, decoded.begin() + 117}));
    EXPECT_EQ((std::vector<std::uint8_t>{0x80, 0xE2, 0xF1, 0x2B, 0xE8, 0xAB, 0x21, 0xC6}),
              (std::vector<std::uint8_t>{decoded.end() - 8, decoded.end()}));
  }
}
