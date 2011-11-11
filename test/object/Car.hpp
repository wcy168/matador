#ifndef CAR_HPP
#define CAR_HPP

#include "object/object.hpp"
#include "object/object_ptr.hpp"

class Engine;

class Car : public oos::object
{
public:
  Car();
  Car(const std::string &brand, const std::string &type);
  virtual ~Car();
  
  virtual void read_from(oos::object_atomizer *reader);
  virtual void write_to(oos::object_atomizer *writer) const;
  
  std::string brand() const;
  void brand(const std::string &b);

  std::string type() const;
  void type(const std::string &t);

  oos::object_ptr<Engine> engine() const;
  void engine(const oos::object_ptr<Engine> &e);
  
  friend std::ostream& operator <<(std::ostream &os, const Car &c);

private:
  std::string brand_;
  std::string type_;
  
  oos::object_ptr<Engine> engine_;
};

#endif /* CAR_HPP */