#ifndef KEY_EVENTS_H
#define KEY_EVENTS_H

#include "Real-Engine/Core/Events/Event.h"

namespace Real
{
  class REAL_API KeyEvent : public Event
  {
  public:
    inline int getKeyCode() const {return m_key_code;}
    virtual int getCategoryNum() const override {return EventCategory::CATKeyboard | EventCategory::CATInput;}


  protected:
    KeyEvent(int key_code) : m_key_code(key_code) {}
    int m_key_code;
  };

  class REAL_API KeyPressedEvent : public KeyEvent
  {
  public:
    KeyPressedEvent(int key_code, int repeate_count) : KeyEvent(key_code), m_repeate_count(repeate_count) {}
    inline int getRepeateCount() {return m_repeate_count;}
    std::string toString() const override
    {
      std::stringstream ss;
      ss << "KeyPressedEvent: "<<m_key_code<<" , "<<"repeate_count: "<<m_repeate_count;
      return ss.str();
    }
  
  static  EventType  getStaticType()               {return EventType::KeyPressed;}
  virtual EventType  getEventType() const override {return getStaticType();}
  virtual std::string getName()     const override {return "KeyPressedEvent";}
  private:
    int m_repeate_count;
  };
  

  class REAL_API KeyReleasedEvent : public KeyEvent
  {
  public:
    KeyReleasedEvent(int key_code) : KeyEvent(key_code) {}
    std::string toString() const override
    {
      std::stringstream ss;
      ss << "KeyReleasedEvent: "<<m_key_code;
      return ss.str();
    }
    
  static  EventType  getStaticType()               {return EventType::KeyReleased;}
  virtual EventType  getEventType() const override {return getStaticType();}
  virtual std::string getName()     const override {return "KeyReleasedEvent";}
  };
}

#endif //KEY_EVENTS_H
