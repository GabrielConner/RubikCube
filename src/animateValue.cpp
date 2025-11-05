#include <vector>
#include <map>
#include <chrono>

#include "pPack/animateValue.h"
#include <iostream>


namespace pPack {
namespace animate_value {

void setPtrValueTypeToValueType(AnimatedPtrValueType& ptr, AnimatedValueType& to, int valueType);
void setPtrValueTypeToAnimatedPosition(AnimatedPtrValueType& ptr, AnimatedValueType& from, AnimatedValueType& to, double start, double end, double curTime, int valueType);


static std::map<size_t, CreateAnimationInfo> values = std::map<size_t, CreateAnimationInfo>();

static size_t animationIterator = 0;

static double deltaTime = 0;
static double curTime = 0;



std::chrono::steady_clock::time_point prev = std::chrono::steady_clock::now();
void animateValueUpdate() {
  std::vector<size_t> removeValues = std::vector<size_t>();

  for (auto& [i, cAnim] : values) {
    AnimatedValue* val = cAnim.animation;

    if (val->end < curTime) {
      setPtrValueTypeToValueType(val->value, val->to, val->valueType);

      if (val->modifiers & ANIMATE_VALUE_MODIFIER_LOOP) {
        val->started = false;
        double length = val->end - val->start;
        val->start += length;
        val->end += length;
      } else if (val->modifiers & ANIMATE_VALUE_MODIFIER_BOUNCE) {
        val->started = false;
        double length = val->end - val->start;
        val->start += length;
        val->end += length;
        AnimatedValueType temp = val->from;
        val->from = val->to;
        val->to = temp;
      } else {
        removeValues.push_back(i);
        val->completed = true;
        continue;
      }
    }

    if (val->start > curTime)
      continue;

    if (!val->started) {
      setPtrValueTypeToValueType(val->value, val->from, val->valueType);
      val->started = true;
    }

    setPtrValueTypeToAnimatedPosition(val->value, val->from, val->to, val->start, val->end, curTime, val->valueType);
  }


  for (size_t i : removeValues)
    values.erase(i);

  std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
  deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(now - prev).count() / 1000000000.0;
  curTime += deltaTime;
  prev = now;
}

void animateValueUpdate(double deltaTime) {
  std::vector<size_t> removeValues = std::vector<size_t>();

  for (auto& [i, cAnim] : values) {
    AnimatedValue* val = cAnim.animation;

    if (val->end < curTime) {
      setPtrValueTypeToValueType(val->value, val->to, val->valueType);

      if (val->modifiers & ANIMATE_VALUE_MODIFIER_LOOP) {
        val->started = false;
        double length = val->end - val->start;
        val->start += length;
        val->end += length;
      } else if (val->modifiers & ANIMATE_VALUE_MODIFIER_BOUNCE) {
        val->started = false;
        double length = val->end - val->start;
        val->start += length;
        val->end += length;
        AnimatedValueType temp = val->from;
        val->from = val->to;
        val->to = temp;
      } else {
        removeValues.push_back(i);
        val->completed = true;
        continue;
      }
    }

    if (val->start > curTime)
      continue;

    if (!val->started) {
      setPtrValueTypeToValueType(val->value, val->from, val->valueType);
      val->started = true;
    }

    setPtrValueTypeToAnimatedPosition(val->value, val->from, val->to, val->start, val->end, curTime, val->valueType);
  }


  for (size_t i : removeValues)
    values.erase(i);

  curTime += deltaTime;
}


void addAnimation(CreateAnimationInfo animation) {
  if (animation.animation == nullptr || animation.animation->completed)
    return;
  values.insert(std::pair<size_t, CreateAnimationInfo>(animationIterator, std::move(animation)));
  animationIterator++;
}

double getAnimationTime() {
  return curTime;
}

static void setPtrValueTypeToValueType(AnimatedPtrValueType& ptr, AnimatedValueType& to, int valueType) {
  switch (valueType) {
  case ANIMATE_VALUE_TYPE_LONG_DOUBLE:
    *ptr.valueLongDouble = to.valueLongDouble;
    break;
  case ANIMATE_VALUE_TYPE_DOUBLE:
    *ptr.valueDouble = to.valueDouble;
    break;
  case ANIMATE_VALUE_TYPE_FLOAT:
    *ptr.valueFloat = to.valueFloat;
    break;
  case ANIMATE_VALUE_TYPE_INT:
    *ptr.valueInt = to.valueInt;
    break;
  case ANIMATE_VALUE_TYPE_SHORT:
    *ptr.valueShort = to.valueShort;
    break;
  case ANIMATE_VALUE_TYPE_CHAR:
    *ptr.valueChar = to.valueChar;
    break;
  case ANIMATE_VALUE_TYPE_UNSIGNED_INT:
    *ptr.valueUInt = to.valueUInt;
    break;
  case ANIMATE_VALUE_TYPE_UNSIGNED_SHORT:
    *ptr.valueUShort = to.valueUShort;
    break;
  case ANIMATE_VALUE_TYPE_UNSIGNED_CHAR:
    *ptr.valueUChar = to.valueUChar;
    break;
  }
}

static void setPtrValueTypeToAnimatedPosition(AnimatedPtrValueType& ptr, AnimatedValueType& from, AnimatedValueType& to, double start, double end, double curTime, int valueType) {
  switch (valueType) {
  case ANIMATE_VALUE_TYPE_LONG_DOUBLE:
    *ptr.valueLongDouble = from.valueLongDouble + (to.valueLongDouble - from.valueLongDouble) / (end - start) * (curTime - start);
    break;
  case ANIMATE_VALUE_TYPE_DOUBLE:
    *ptr.valueDouble = from.valueDouble + (to.valueDouble - from.valueDouble) / (end - start) * (curTime - start);
    break;
  case ANIMATE_VALUE_TYPE_FLOAT:
    *ptr.valueFloat = from.valueFloat + (to.valueFloat - from.valueFloat) / (end - start) * (curTime - start);
    break;
  case ANIMATE_VALUE_TYPE_INT:
    *ptr.valueInt = from.valueInt + (to.valueInt - from.valueInt) / (end - start) * (curTime - start);
    break;
  case ANIMATE_VALUE_TYPE_SHORT:
    *ptr.valueShort = from.valueShort + (to.valueShort - from.valueShort) / (end - start) * (curTime - start);
    break;
  case ANIMATE_VALUE_TYPE_CHAR:
    *ptr.valueChar = from.valueChar + (to.valueChar - from.valueChar) / (end - start) * (curTime - start);
    break;
  case ANIMATE_VALUE_TYPE_UNSIGNED_INT:
    *ptr.valueUInt = from.valueUInt + (to.valueUInt - from.valueUInt) / (end - start) * (curTime - start);
    break;
  case ANIMATE_VALUE_TYPE_UNSIGNED_SHORT:
    *ptr.valueUShort = from.valueUShort + (to.valueUShort - from.valueUShort) / (end - start) * (curTime - start);
    break;
  case ANIMATE_VALUE_TYPE_UNSIGNED_CHAR:
    *ptr.valueUChar = from.valueUChar + (to.valueUChar - from.valueUChar) / (end - start) * (curTime - start);
    break;
  }
}

} //namespace animate_value
} //namespace pPack