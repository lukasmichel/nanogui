//
// Created by lukas on 20.05.22.
//


#pragma once

#include <nanogui/widget.h>
#include <nanogui/texture.h>
#include <nanogui/screen.h>
#include <nanogui/widget.h>

class Texture;

NAMESPACE_BEGIN(nanogui)
class ParentedScreen : public Screen {
public:
  ParentedScreen(Screen *parent, const Vector2i &size, const std::string &caption = "Unnamed",
                 bool resizable = true,
                 bool fullscreen = false,
                 bool depth_buffer = true,
                 bool stencil_buffer = true,
                 bool float_buffer = false,
                 unsigned int gl_major = 3,
                 unsigned int gl_minor = 2) : Screen(size,
                                                     caption,
                                                     resizable,
                                                     fullscreen,
                                                     depth_buffer,
                                                     stencil_buffer,
                                                     float_buffer,
                                                     gl_major,
                                                     gl_minor),
                                              m_parent(parent) {
    parent->add_child(this);
    this->m_resize_callback = [&, this](Vector2i size) {
      bool repeat;
      do {
        repeat = false;
        for (auto child: m_children) {
          {
            child->set_size(size);
            auto c_size = child->size();
            if (c_size != size) {
              repeat = true;
              break;
            }
          }
        }
      } while (repeat);
    };
  }

  ParentedScreen(Screen *parent) : Screen(parent->size(),
                                          parent->caption(),
                                          true,
                                          false,
                                          parent->has_depth_buffer(),
                                          parent->has_stencil_buffer(),
                                          parent->has_float_buffer(),
                                          3,
                                          2) {
    parent->add_child(this);
    this->m_resize_callback = [&, this](Vector2i size) {
      bool repeat;
      do {
        repeat = false;
        for (auto child: m_children) {
          {
            child->set_size(size);
            auto c_size = child->size();
            if (c_size != size) {
              repeat = true;
              break;
            }
          }
        }
      } while (repeat);
    };
  }

  bool keyboard_event(int key, int scancode, int action, int modifiers) override {
    bool r = m_parent->keyboard_event(key, scancode, action, modifiers);
    //r |= Screen::keyboard_event(key, scancode, action, modifiers);
    return r;
  }

  bool keyboard_character_event(unsigned int codepoint) override {
    bool r = m_parent->keyboard_character_event(codepoint);
    //r |= Screen::keyboard_character_event(codepoint);
    return r;
  }

  bool mouse_button_event(const Vector2i &p, int button, bool down, int modifiers) override {
    bool r = m_parent->mouse_button_event(p, button, down, modifiers);
    //r |= Widget::mouse_button_event(p, button, down, modifiers);
    return r;
  }

  bool mouse_motion_event(const Vector2i &p, const Vector2i &rel, int button, int modifiers) override {
    bool r = m_parent->mouse_motion_event(p, rel, button, modifiers);
    //r |= Widget::mouse_motion_event(p, rel, button, modifiers);
    return r;
  }

  bool mouse_drag_event(const Vector2i &p, const Vector2i &rel, int button, int modifiers) override {
    bool r = m_parent->mouse_drag_event(p, rel, button, modifiers);
    //for (auto c: m_children)
    //r |= c->mouse_drag_event(p, rel, button, modifiers);
    return r;
  }

  bool mouse_enter_event(const Vector2i &p, bool enter) override {
    bool r = m_parent->mouse_enter_event(p, enter);
    //r |= Widget::mouse_enter_event(p, enter);
    return r;
  }

  bool scroll_event(const Vector2i &p, const Vector2f &rel) override {
    // bool r = m_parent->scroll_event(p, rel);
    bool r = Widget::scroll_event(p, rel);
    return r;
  }


  Vector2i preferred_size(NVGcontext *ctx) const override {
    return Widget::preferred_size(m_nvg_context);
  }

  Vector2i preferred_size() const {
    return Widget::preferred_size(m_nvg_context);
  }

  void draw(NVGcontext *ctx) override {
    Widget::draw(m_nvg_context);
  }

  void draw() {
    Widget::draw(m_nvg_context);
  }

  void perform_layout(NVGcontext *ctx) override {
    Widget::perform_layout(m_nvg_context);
  }

  void perform_layout() {
    Widget::perform_layout(m_nvg_context);
  }

public:
  ref<Widget> m_parent;
};
NAMESPACE_END(nanogui)
