#ifndef MATADOR_HTTP_HPP
#define MATADOR_HTTP_HPP

#ifdef _MSC_VER
#ifdef matador_utils_EXPORTS
    #define OOS_HTTP_API __declspec(dllexport)
    #define EXPIMP_HTTP_TEMPLATE
  #else
    #define OOS_HTTP_API __declspec(dllimport)
    #define EXPIMP_HTTP_TEMPLATE extern
  #endif
  #pragma warning(disable: 4251)
#else
#define OOS_HTTP_API
#endif

#include <unordered_map>
#include <string>

namespace matador {
namespace http {

typedef std::unordered_map<std::string, std::string> t_string_param_map;

class OOS_HTTP_API http
{
public:
  enum method_t {
    UNKNOWN,
    GET,
    POST,
    PUT,
    DELETE,
    OPTIONS,
    HEAD
  };

  static std::string to_string(method_t m);
  static method_t to_method(const std::string &str);

  enum status_t {
    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NO_CONTENT = 204,
    MULTIPLE_CHOICES = 300,
    MOVED_PERMANENTLY = 301,
    MOVED_TEMPORARILY = 302,
    NOT_MODIFIED = 304,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    BAD_GATEWAY = 502,
    SERVICE_UNAVAILABLE = 503
  };

  static std::string to_string(status_t status);

private:
  static std::unordered_map<method_t, std::string> method_string_map_;
  static std::unordered_map<std::string, method_t> string_method_map_;
  static std::unordered_map<status_t, std::string> status_string_map_;
};

}
}

#endif //MATADOR_HTTP_HPP
