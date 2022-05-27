//
// Created by lukas on 20.05.22.
//


#pragma once

#include <nanogui/screen.h>
#include <nanogui/texture.h>
#include <nanogui/widget.h>

class Texture;

NAMESPACE_BEGIN(nanogui)
class ParentedScreen : public Screen {
public:
  ParentedScreen(const Vector2i &size, const std::string &caption = "Unnamed",
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
                                                     gl_minor) {
    this->m_resize_callback = [&, this](Vector2i size) {
      //Vector2i saveSize{size};
      bool repeat;
      //this->perform_layout();
      int oldHeight = 0;
      for (auto c: m_children)
        oldHeight += c->height();
      do {
        repeat = false;
        //for (auto child: m_children) {
        float heightRescaleFactor = float(size[1] - this->m_theme->m_window_header_height) / float(oldHeight);

        for (auto it = m_children.rbegin(); it != m_children.rend(); it++) {
          auto child = *it;
          child->set_width(size[0]);
          int newChildHeight = std::floor(float(child->height()) * heightRescaleFactor);
          child->set_height(newChildHeight);
          child->perform_layout(m_nvg_context);
          /*if (child->width() > size[0]) {
           size[0] = child->width();
           repeat = true;
          }
          if (child->height() > newChildHeight) {
           size[1] += child->height() - newChildHeight;
           repeat = true;
          }*/
        }
      } while (repeat);
      this->perform_layout();
      /*Vector2i newSize{0, 0};
      for (auto c: m_children)
       newSize += c->size();
      if (newSize != saveSize)
       resize_callback_event(size[0], size[1]);*/
    };
  }



  bool keyboard_event(int key, int scancode, int action, int modifiers) override {
    if (m_keyboard_event_callback)
      m_keyboard_event_callback(key, scancode, action, modifiers);
    return Screen::keyboard_event(key, scancode, action, modifiers);
  }

  bool mouse_button_event(const Vector2i &p, int button, bool down, int modifiers) override {
    if (m_mouse_button_event_callback)
      m_mouse_button_event_callback(p, button, down, modifiers);
    return Screen::mouse_button_event(p, button, down, modifiers);
  }

  bool mouse_motion_event(const Vector2i &p, const Vector2i &rel, int button, int modifiers) override {
    return Screen::mouse_motion_event(p, rel, button, modifiers);
  }

  bool mouse_drag_event(const Vector2i &p, const Vector2i &rel, int button, int modifiers) override {
    return Screen::mouse_drag_event(p, rel, button, modifiers);
  }

  bool scroll_event(const Vector2i &p, const Vector2f &rel) override {
    return Screen::scroll_event(p, rel);
  }

  const std::function<void(int, int, int, int)> &getKeyboardEventCallback() const {
    return m_keyboard_event_callback;
  }

  void setKeyboardEventCallback(const std::function<void(int, int, int, int)> &KeyboardEventCallback) {
    m_keyboard_event_callback = KeyboardEventCallback;
  }

  const std::function<void(const Vector2i &, int, bool, int)> &getMMouseButtonEventCallback() const {
    return m_mouse_button_event_callback;
  }

  void
  setMMouseButtonEventCallback(const std::function<void(const Vector2i &, int, bool, int)> &mMouseButtonEventCallback) {
    m_mouse_button_event_callback = mMouseButtonEventCallback;
  }

  const std::function<void(const Vector2i &, const Vector2i &, int, int)> &getMMouseMotionEventCallback() const {
    return m_mouse_motion_event_callback;
  }

  void setMMouseMotionEventCallback(
      const std::function<void(const Vector2i &, const Vector2i &, int, int)> &mMouseMotionEventCallback) {
    m_mouse_motion_event_callback = mMouseMotionEventCallback;
  }

  const std::function<void(const Vector2i &, const Vector2i &, int, int)> &getMMouseDragEventCallback() const {
    return m_mouse_drag_event_callback;
  }

  void setMMouseDragEventCallback(
      const std::function<void(const Vector2i &, const Vector2i &, int, int)> &mMouseDragEventCallback) {
    m_mouse_drag_event_callback = mMouseDragEventCallback;
  }

  const std::function<void(const Vector2i &, const Vector2i &)> &getMScrollEventCallback() const {
    return m_scroll_event_callback;
  }

  void setMScrollEventCallback(const std::function<void(const Vector2i &, const Vector2i &)> &mScrollEventCallback) {
    m_scroll_event_callback = mScrollEventCallback;
  }

private:
  ref<Widget> m_parent;
  std::function<void(int, int, int, int)> m_keyboard_event_callback;
  std::function<void(const Vector2i &, int, bool, int)> m_mouse_button_event_callback;
  std::function<void(const Vector2i &, const Vector2i &, int, int)> m_mouse_motion_event_callback;
  std::function<void(const Vector2i &, const Vector2i &, int, int)> m_mouse_drag_event_callback;
  std::function<void(const Vector2i &, const Vector2i &)> m_scroll_event_callback;
};
NAMESPACE_END(nanogui)
