#ifndef __CODE_H__
#define __CODE_H__

class Pet {
 private:
  const char * name;
  const char * species;
  unsigned  hungerLevel;
 public:
  Pet(const char * n, const char * s);
  void feed(unsigned food);  
  void getHungrier(unsigned howMuch);
  void describe(void);
};

class Person {
 private:
  const char * name;
  Pet * myPet;
 public:
  Person(const char * n);
  void describe(void);
  void feedPet(unsigned food);
  bool havePet(void);
  void setPet(Pet * p);
};


#endif
