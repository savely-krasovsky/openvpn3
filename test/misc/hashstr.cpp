// TEST : {"cmd": "./go hashstr"}

#include <iostream>

#include <openvpn/log/logsimple.hpp>
#include <openvpn/common/size.hpp>
#include <openvpn/common/exception.hpp>

#define OPENVPN_LOG_SSL(x) OPENVPN_LOG(x)

#include <openvpn/common/file.hpp>
#include <openvpn/ssl/sslchoose.hpp>
#include <openvpn/crypto/cryptoalgs.hpp>
#include <openvpn/crypto/hashstr.hpp>

using namespace openvpn;

int main(int /*argc*/, char* /*argv*/[])
{
  try {
    const std::string content = read_text_utf8("1984.txt");

    DigestFactory::Ptr digest_factory(new CryptoDigestFactory<SSLLib::CryptoAPI>());
    HashString hash(*digest_factory, CryptoAlgs::MD5);
    hash.update(content);
    const std::string actual = hash.final_hex();
    const std::string expected = "2bea7a83bf94971af26372126ebba7e3";
    if (actual != expected)
      throw Exception("actual does not match expected");
  }
  catch (const std::exception& e)
    {
      std::cerr << "Exception: " << e.what() << std::endl;
      return 1;
    }
  return 0;
}
