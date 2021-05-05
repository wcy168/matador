#ifndef MATADOR_JWT_TOKEN_HPP
#define MATADOR_JWT_TOKEN_HPP

#include "matador/utils/time.hpp"

namespace matador {
namespace http {
namespace auth {

class jwt_header
{
public:
  std::string algorithm {};
  std::string type {};

  template<class Serializer>
  void serialize(Serializer &serializer)
  {
    serializer.serialize("alg", algorithm);
    serializer.serialize("typ", type);
  }
};

class jwt_payload
{
public:
  std::string issuer {};
  std::string subject {};
  std::string audience {};
  matador::time expiration_time {0};
  matador::time not_before {0};
  matador::time issued_at {0};
  std::string jwt_id {};
  std::string name {};

  template<class Serializer>
  void serialize(Serializer &serializer)
  {
    serializer.serialize("iss", issuer);
    serializer.serialize("sub", subject);
    serializer.serialize("aud", audience);
    serializer.serialize("exp", expiration_time);
    serializer.serialize("nbf", not_before);
    serializer.serialize("iat", issued_at);
    serializer.serialize("jti", jwt_id);
    serializer.serialize("name", name);
  }
};

class jwt_token
{
public:
  jwt_header header {};
  jwt_payload payload {};
};

}
}
}
#endif //MATADOR_JWT_TOKEN_HPP
