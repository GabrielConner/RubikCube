#ifndef PPM_INCLUDE_PPACK_ANIMATE_VALUE_H
#define PPM_INCLUDE_PPACK_ANIMATE_VALUE_H

constexpr unsigned int ANIMATE_VALUE_TYPE_LONG_DOUBLE = 0x00000001;
constexpr unsigned int ANIMATE_VALUE_TYPE_DOUBLE = 0x00000002;
constexpr unsigned int ANIMATE_VALUE_TYPE_FLOAT = 0x00000004;
constexpr unsigned int ANIMATE_VALUE_TYPE_INT = 0x00000008;
constexpr unsigned int ANIMATE_VALUE_TYPE_SHORT = 0x00000010;
constexpr unsigned int ANIMATE_VALUE_TYPE_CHAR = 0x00000020;
constexpr unsigned int ANIMATE_VALUE_TYPE_UNSIGNED_INT = 0x00000040;
constexpr unsigned int ANIMATE_VALUE_TYPE_UNSIGNED_SHORT = 0x00000080;
constexpr unsigned int ANIMATE_VALUE_TYPE_UNSIGNED_CHAR = 0x00000100;

constexpr unsigned char ANIMATE_VALUE_MODIFIER_LOOP = 0B00000001;
constexpr unsigned char ANIMATE_VALUE_MODIFIER_BOUNCE = 0B00000010;

namespace pPack {
namespace animate_value {


union AnimatedValueType {
  long double valueLongDouble;
  double valueDouble;
  float valueFloat;
  int valueInt;
  short valueShort;
  char valueChar;
  unsigned int valueUInt;
  unsigned short valueUShort;
  unsigned char valueUChar;


  AnimatedValueType(long double v) : valueLongDouble(v) {}
  AnimatedValueType(double v) : valueDouble(v) {}
  AnimatedValueType(float v) : valueFloat(v) {}
  AnimatedValueType(int v) : valueInt(v) {}
  AnimatedValueType(short v) : valueShort(v) {}
  AnimatedValueType(char v) : valueChar(v) {}
  AnimatedValueType(unsigned int v) : valueUInt(v) {}
  AnimatedValueType(unsigned short v) : valueUShort(v) {}
  AnimatedValueType(unsigned char v) : valueUChar(v) {}
};
union AnimatedPtrValueType {
  long double* valueLongDouble;
  double* valueDouble;
  float* valueFloat;
  int* valueInt;
  short* valueShort;
  char* valueChar;
  unsigned int* valueUInt;
  unsigned short* valueUShort;
  unsigned char* valueUChar;


  AnimatedPtrValueType(long double* v) : valueLongDouble(v) {}
  AnimatedPtrValueType(double* v) : valueDouble(v) {}
  AnimatedPtrValueType(float* v) : valueFloat(v) {}
  AnimatedPtrValueType(int* v) : valueInt(v) {}
  AnimatedPtrValueType(short* v) : valueShort(v) {}
  AnimatedPtrValueType(char* v) : valueChar(v) {}
  AnimatedPtrValueType(unsigned int* v) : valueUInt(v) {}
  AnimatedPtrValueType(unsigned short* v) : valueUShort(v) {}
  AnimatedPtrValueType(unsigned char* v) : valueUChar(v) {}
};



struct AnimatedValue {
  AnimatedPtrValueType value;
  AnimatedValueType from;
  AnimatedValueType to;

  double start;
  double end;

  unsigned char modifiers;

  int valueType;

  bool started;
  bool completed;

  AnimatedValue() = delete;
  AnimatedValue(AnimatedPtrValueType Value, AnimatedValueType From, AnimatedValueType To, double Start, double End, unsigned char Modifiers, int ValueType) :
    value(Value), from(From), to(To), start(Start), end(End), modifiers(Modifiers), started(false), completed(false), valueType(ValueType) {
  }
};


struct CreateAnimationInfo {
  AnimatedValue* animation;
  bool handled;

  CreateAnimationInfo(AnimatedValue* Animation, bool Handled = true) : animation(Animation), handled(Handled) {}
  CreateAnimationInfo(const CreateAnimationInfo&& anim) = delete;
  CreateAnimationInfo(CreateAnimationInfo&& anim) noexcept : animation(anim.animation), handled(anim.handled) { anim.handled = false; }

  ~CreateAnimationInfo() {
    if (handled && animation)
      delete(animation);
  }
};


void addAnimation(CreateAnimationInfo animation);
void animateValueUpdate(double deltaTime);
void animateValueUpdate();
double getAnimationTime();

} //namespace animate_value
} //namespace pPack

#endif