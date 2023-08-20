#ifndef OOS_VALUE_COLUMN_SERIALIZER_HPP
#define OOS_VALUE_COLUMN_SERIALIZER_HPP

#include "matador/utils/access.hpp"
#include "matador/utils/identifier.hpp"
#include "matador/utils/serializer.hpp"

#include "matador/sql/column.hpp"

namespace matador {

class sql;

namespace detail {

/// @cond MATADOR_DEV
class value_column_serializer;

class value_column_identifier_serializer : public identifier_serializer
{
public:
  explicit value_column_identifier_serializer(value_column_serializer &serializer)
  : serializer_(serializer) {}

  void serialize(short &value, long size) override { extract_value(value, size); }
  void serialize(int &value, long size) override { extract_value(value, size); }
  void serialize(long &value, long size) override { extract_value(value, size); }
  void serialize(long long &value, long size) override { extract_value(value, size); }
  void serialize(unsigned short &value, long size) override { extract_value(value, size); }
  void serialize(unsigned int &value, long size) override { extract_value(value, size); }
  void serialize(unsigned long &value, long size) override { extract_value(value, size); }
  void serialize(unsigned long long &value, long size) override { extract_value(value, size); }
  void serialize(std::string &value, long size) override { extract_value(value, size); }
  void serialize(null_type_t &, long /*size*/) override {}

private:
  template < typename ValueType >
  void extract_value(ValueType &value, long size);

private:
  value_column_serializer &serializer_;
};

class value_column_serializer
{
public:
  value_column_serializer();

  template<class T>
  void append_to(const std::shared_ptr<columns> &cols, T &x)
  {
    cols_ = cols;
    matador::access::serialize(*this, x);
  }

  template < class T >
  void serialize(T &x)
  {
    matador::access::serialize(*this, x);
  }

  void on_attribute(const char *id, char &x, long /*size*/ = -1);
  void on_attribute(const char *id, short &x, long /*size*/ = -1);
  void on_attribute(const char *id, int &x, long /*size*/ = -1);
  void on_attribute(const char *id, long &x, long /*size*/ = -1);
  void on_attribute(const char *id, long long &x, long /*size*/ = -1);
  void on_attribute(const char *id, unsigned char &x, long /*size*/ = -1);
  void on_attribute(const char *id, unsigned short &x, long /*size*/ = -1);
  void on_attribute(const char *id, unsigned int &x, long /*size*/ = -1);
  void on_attribute(const char *id, unsigned long &x, long /*size*/ = -1);
  void on_attribute(const char *id, unsigned long long &x, long /*size*/ = -1);
  void on_attribute(const char *id, float &x, long /*size*/ = -1);
  void on_attribute(const char *id, double &x, long /*size*/ = -1);
  void on_attribute(const char *id, bool &x, long /*size*/ = -1);
  void on_attribute(const char *id, char *x, long size);
  void on_attribute(const char *id, std::string &x, long size = -1);
  void on_attribute(const char *id, date &x, long /*size*/ = -1);
  void on_attribute(const char *id, time &x, long /*size*/ = -1);
  void on_belongs_to(const char *id, identifiable_holder &x, cascade_type);
  void on_has_one(const char *id, identifiable_holder &x, cascade_type);
  template< typename ValueType >
  void on_primary_key(const char *id, ValueType &x, long size = -1)
  {
    on_attribute(id, x, size);
  }
  void on_has_many(const char *, abstract_has_many &, const char *, const char *, cascade_type) {}
  void on_has_many(const char *, abstract_has_many &, cascade_type) {}

  template < class ValueType >
  void add_column_value(const char *col, ValueType &val, long size)
  {
    cols_->push_back(make_value_column(col, val, size));
  }

  template < class ValueType >
  void add_value(const char *col, ValueType &val, long size)
  {
    cols_->push_back(std::make_shared<value>(col, val, size));
  }

private:
  std::shared_ptr<columns> cols_;
  value_column_identifier_serializer value_column_identifier_serializer_;
};

/// @endcond

template<typename ValueType>
void value_column_identifier_serializer::extract_value(ValueType &value, long size)
{
  // Todo: Add column name
  serializer_.add_column_value("", value, size);
}

}
}
#endif //OOS_VALUE_COLUMN_SERIALIZER_HPP
