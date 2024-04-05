#ifndef APP_EVENTS_H
#define APP_EVENTS_H

#include "Karen/Core/Events/Event.h"

namespace Karen
{
  class KAREN_API WindowResizeEvent : public Event
  {
  public:
    WindowResizeEvent(unsigned int width, unsigned int height) : m_width(width), m_height(height) {}
    
    inline unsigned int getWidth()  const {return m_width;}
    inline unsigned int getHeight() const {return m_height;}

    std::string toString() const override 
    {
      std::stringstream ss;
      ss <<"WindowResizedEvent: "<<m_width<<", "<<m_height;
      return ss.str();
    }

  static  EventType   getStaticType()                 {return EventType::WindowResized;}
  virtual EventType   getEventType()   const override {return getStaticType();}
  virtual std::string getName()        const override {return "WindowResized";}
  virtual int         getCategoryNum() const override {return EventCategory::CATAPP;}

  private:
    unsigned int m_width, m_height;
  };


  class KAREN_API WindowClosedEvent : public Event
  {
  public:
    WindowClosedEvent() {} 
    static  EventType   getStaticType()                 {return EventType::WindowClosed;}
    virtual EventType   getEventType()   const override {return getStaticType();}
    virtual std::string getName()        const override {return "WindowClosed";}
    virtual int         getCategoryNum() const override {return EventCategory::CATAPP;}


  };
}


#endif //APP_EVENTS_H
