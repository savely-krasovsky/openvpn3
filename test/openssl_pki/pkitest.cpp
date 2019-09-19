// TEST : {"cmd": "./go"}

#include <iostream>

#include <openvpn/log/logsimple.hpp>
#include <openvpn/common/file.hpp>
#include <openvpn/pki/cclist.hpp>
#include <openvpn/openssl/pki/x509.hpp>
#include <openvpn/openssl/pki/x509store.hpp>
#include <openvpn/openssl/pki/pkey.hpp>
#include <openvpn/openssl/pki/dh.hpp>
#include <openvpn/openssl/pki/crl.hpp>
#include <openvpn/init/initprocess.hpp>

using namespace openvpn;

typedef CertCRLListTemplate<OpenSSLPKI::X509List, OpenSSLPKI::CRLList> CertCRLList;

template <typename VEC>
VEC move_contents(VEC& src)
{
  VEC v;
  for (auto &e : src)
    v.push_back(std::move(e));
  return v;
}

void move_contents(CertCRLList& dest, CertCRLList& src)
{
  dest.certs = move_contents(src.certs);
  dest.crls = move_contents(src.crls);
}

void test_cert_crl()
{
  OPENVPN_LOG("======= TEST CERT CRL =======");
  std::string cert_crl_txt = read_text("certcrl.pem");

  CertCRLList ccl, ccl2;
  ccl.parse_pem(cert_crl_txt, "TEST1");
  CertCRLList ccl1(ccl);
  ccl2 = ccl1;
  ccl2 = ccl1;
  std::string rend2 = ccl2.render_pem();

  CertCRLList ccl3;
  ccl3.parse_pem_file("certcrl.pem");
  std::string rend3 = ccl3.render_pem();

  if (rend2 != rend3)
    throw Exception("BAD COMPARE #1");

  CertCRLList ccl4(rend3, "TEST2");
  CertCRLList ccl5(std::move(ccl4));
  ccl2 = ccl5;
  rend2 = ccl2.render_pem();
  if (rend2 != rend3)
    throw Exception("BAD COMPARE #2");

  CertCRLList ccl6(rend3, "TEST3");
  move_contents(ccl2, ccl6);
  rend2 = ccl2.render_pem();
  if (rend2 != rend3)
    throw Exception("BAD COMPARE #3");

  OpenSSLPKI::X509Store xs(ccl2);

  //std::cout << rend2;
}

void test_pkey()
{
  OPENVPN_LOG("======= TEST PKEY =======");

  std::string pkey_txt = read_text("key.pem");

  OpenSSLPKI::PKey pkey, pkey2;
  pkey.parse_pem(pkey_txt, "TEST0");
  OpenSSLPKI::PKey pkey1(pkey);
  pkey2 = pkey1;
  pkey2 = pkey1;
  std::string rend2 = pkey2.render_pem();

  OpenSSLPKI::PKey pkey3(pkey_txt, "TEST2");
  std::string rend3 = pkey3.render_pem();

  if (rend2 != rend3)
    throw Exception("BAD COMPARE #1");

  OpenSSLPKI::PKey pkey4(rend3, "TEST3");
  OpenSSLPKI::PKey pkey5(std::move(pkey4));
  pkey2 = pkey5;
  rend2 = pkey2.render_pem();
  if (rend2 != rend3)
    throw Exception("BAD COMPARE #2");

  //std::cout << rend2;
}

void test_dh()
{
  OPENVPN_LOG("======= TEST DH =======");
  std::string dh_txt = read_text("dh2048.pem");

  OpenSSLPKI::DH dh, dh2;
  dh.parse_pem(dh_txt);
  OpenSSLPKI::DH dh1(dh);
  dh2 = dh1;
  dh2 = dh1;
  std::string rend2 = dh2.render_pem();

  OpenSSLPKI::DH dh3(dh_txt);
  std::string rend3 = dh3.render_pem();

  if (rend2 != rend3)
    throw Exception("BAD COMPARE #1");

  OpenSSLPKI::DH dh4(rend3);
  OpenSSLPKI::DH dh5(std::move(dh4));
  dh2 = dh5;
  rend2 = dh2.render_pem();
  if (rend2 != rend3)
    throw Exception("BAD COMPARE #2");

  //std::cout << rend2;
}

int main(int /*argc*/, char* /*argv*/[])
{
  // process-wide initialization
  InitProcess::init();

  try {
    test_cert_crl();
    test_pkey();
    test_dh();
  }
  catch (const std::exception& e)
    {
      std::cerr << "Exception: " << e.what() << std::endl;
      return 1;
    }
  InitProcess::uninit();
  return 0;
}
