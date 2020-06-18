#ifndef MATADOR_BASIC_FILE_SINK_HPP
#define MATADOR_BASIC_FILE_SINK_HPP

#ifdef _MSC_VER
#ifdef matador_logger_EXPORTS
    #define OOS_LOGGER_API __declspec(dllexport)
    #define EXPIMP_LOGGER_TEMPLATE
  #else
    #define OOS_LOGGER_API __declspec(dllimport)
    #define EXPIMP_LOGGER_TEMPLATE extern
  #endif
  #pragma warning(disable: 4251)
#else
#define OOS_LOGGER_API
#endif

#include "matador/logger/log_sink.hpp"

#include <cstdio>

namespace matador {

/**
 * @brief basic file log sink
 *
 * Basic file log sink which writes the
 * message to an open file stream.
 */
class OOS_LOGGER_API basic_file_sink : public log_sink
{
protected:
  basic_file_sink() = default;

  /**
   * Creates a basic_file_sink with a given file stream
   *
   * @param f File stream to write on
   */
  explicit basic_file_sink(FILE *f);

public:
  /**
   * Writes the message to a internal file stream
   *
   * @param message The message to log
   * @param size The size of the message
   */
  void write(const char *message, size_t size) override;

  /**
   * Closes the internal file stream
   */
  void close() override;

protected:
  /// @cond MATADOR_DEV
  FILE *stream = nullptr;
  /// @endcond
};

}
#endif //MATADOR_BASIC_FILE_SINK_HPP
