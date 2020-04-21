#include "matador/logger/rotating_file_sink.hpp"

#include "matador/utils/string.hpp"
#include "matador/utils/os.hpp"

#include <iostream>

namespace matador {

rotating_file_sink::rotating_file_sink(const std::string& path, size_t max_size, size_t file_count)
  : logfile_(path, "a")
  , max_size_(max_size)
  , file_count_(file_count)
{
  split(logfile_.path());

  current_size_ = logfile_.size();
}

void rotating_file_sink::write(const char *message, size_t size)
{
  current_size_ += size;
  if (current_size_ > max_size_) {
    current_size_ = size;
    rotate();
  }
  fwrite(message, sizeof(char), size, logfile_.stream());
  fflush(logfile_.stream());
}

void rotating_file_sink::close()
{
  logfile_.close();
}

std::string rotating_file_sink::calculate_filename(size_t fileno)
{
  char buffer[1024];
  sprintf(buffer, "%s.%zu.%s", base_path_.c_str(), fileno, extension_.c_str());
  return buffer;
}

/*
 * Rotate log files:
 * log_path.log  -> log_path.01.log
 * log_path.01.log  -> log_path.02.log
 * log_path.02.log  -> log_path.03.log
 * ...
 * log_path.[n-1].log  -> log_path.[n].log
 * delete log_path.[n].log
 * create new log_path.log
 */
void rotating_file_sink::rotate()
{
  std::string filename;
  for (size_t i = file_count_; i != 0; --i) {
    filename = calculate_filename(i);
    if (!matador::os::exists(filename.c_str())) {
      continue;
    }
    if (i == file_count_) {
      matador::os::remove(filename.c_str());
    } else {
      matador::os::rename(filename.c_str(), calculate_filename(i+1).c_str());
    }
  }
  std::string path = logfile_.path();
  logfile_.close();
  matador::os::rename(path.c_str(), filename.c_str());
  logfile_.open(path, "a");
}

void rotating_file_sink::split(const std::string &path)
{
  std::vector<std::string> result;
  if (matador::split(path, '.', result) != 2) {
    throw std::logic_error("splitted path must consists of two elements");
  }
  base_path_ = result.at(0);
  extension_ = result.at(1);
}
}
