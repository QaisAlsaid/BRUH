#ifndef MOUSE_EVENTS_H
#define MOUSE_EVENTS_H

#include "Karen/Core/Events/Event.h"

namespace Karen
{
  class KAREN_API MouseMovedEvent : public Event
  {
  public:
    MouseMovedEvent(float x, float y) : m_mouse_x(x), m_mouse_y(y) {}
    inline float getX() const {return m_mouse_x;}
    inline float getY() const {return m_mouse_y;}

    std::string toString() const override 
    {
      std::stringstream ss;
      ss << "MouseMovedEvent: "<<m_mouse_x<<", "<<m_mouse_y;
      return ss.str();
    }

    static  EventType   getStaticType()                 {return EventType::MouseMoved;}
    virtual EventType   getEventType()   const override {return getStaticType();}
    virtual std::string getName()        const override {return "MouseMoved";}
    virtual int         getCategoryNum() const override {return EventCategory::CATMouse | EventCategory::CATInput;}


  private:
    float m_mouse_x, m_mouse_y;
  };


  class KAREN_API MouseScrolledEvent : public Event
  {
  public:
    MouseScrolledEvent(float x_offset, float y_offset) : m_x_offset(x_offset), m_y_offset(y_offset) {}
    inline float getXoffset() const {return m_x_offset;}
    inline float getYoffset() const {return m_y_offset;}

    std::string toString() const override
    {
      std::stringstream ss;
      ss << "MouseScrolledEvent: "<<m_x_offset<<", "<<m_y_offset;
      return ss.str();
    } 
    static  EventType   getStaticType()                 {return EventType::MouseScrolled;}
    virtual EventType   getEventType()   const override {return getStaticType();}
    virtual std::string getName()        const override {return "MouseScrolledEvent";}
    virtual int         getCategoryNum() const override {return EventCategory::CATInput | EventCategory::CATMouse;}

  private:
    float m_x_offset, m_y_offset;
  };


  class KAREN_API MouseButtonEvent : public Event
  {
  public:
    inline int getMouseButton() const {return m_button;}
    virtual int getCategoryNum() const override {return EventCategory::CATMouse | EventCategory::CATInput;}


  protected:
    MouseButtonEvent(int button) : m_button(button) {}
    int m_button;
  };


  class KAREN_API MouseButtonPressed : public MouseButtonEvent
  {
  public:
    MouseButtonPressed(int button) : MouseButtonEvent(button) {}
    std::string toString() const override 
    {
      std::stringstream ss;
      ss <<"MouseButtonPressed: "<<m_button;
      return ss.str();
    }
    
    static  EventType   getStaticType()               {return EventType::MouseButtonPressed;}
    virtual EventType   getEventType() const override {return getStaticType();}
    virtual std::string getName()      const override {return "MouseButtonPressed";}

  };


 class KAREN_API MouseButtonReleased : public MouseButtonEvent
  {
  public:
    MouseButtonReleased(int button) : MouseButtonEvent(button) {}
    std::string toString() const override 
    {
      std::stringstream ss;
      ss <<"MouseButtonReleased: "<<m_button;
      return ss.str();
    }
    
    static  EventType   getStaticType()               {return EventType::MouseButtonReleased;}
    virtual EventType   getEventType() const override {return getStaticType();}
    virtual std::string getName()      const override {return "MouseButtonReleased";}

  };

}

#endif //MOUSE_EVENTS_H
