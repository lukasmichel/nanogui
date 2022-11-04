/*
    nanogui/imageview.cpp -- Widget used to display images.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    The false color renderer was implemented by Lukas Michel, based on the visualizer by Thomas Müller in https://github.com/Tom94/practical-path-guiding.git

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/imageview_falsecolor.h>
#include <nanogui/renderpass.h>
#include <nanogui/textbox.h>
#include <nanogui/shader.h>
#include <nanogui/texture.h>
#include <nanogui/screen.h>
#include <nanogui/opengl.h>
#include <nanogui_resources.h>

NAMESPACE_BEGIN(nanogui)

ImageViewFalsecolor::ImageViewFalsecolor(Widget *parent) : ImageView(parent) {
  // the shader is not referenced yet, has a reference count of 0 and thus will throw an error on deletion
  /*if (m_image_shader->ref_count() == 0)
	m_image_shader->inc_ref();
  while (m_image_shader->ref_count() > 1)
	m_image_shader->dec_ref(true);*/
  m_image_shader.reset();
  //m_image_shader->dec_ref(true);
  m_image_shader = new Shader(
	  render_pass(),
	  /* An identifying name */
	  "a_simple_falsecolor_shader",
	  NANOGUI_SHADER(imageview_vertex),
	  NANOGUI_SHADER(imageview_falsecolor_fragment),
	  Shader::BlendMode::AlphaBlend
  );
  const float positions[] = {
	  0.f, 0.f, 1.f, 0.f, 0.f, 1.f,
	  1.f, 0.f, 1.f, 1.f, 0.f, 1.f
  };
  m_image_shader->set_buffer("position", VariableType::Float32, {6, 2},
							 positions);
}

// the ImageView::draw_contents() function, extended only with setting the exposure value
void ImageViewFalsecolor::draw_contents() {
  if (!m_image)
	return;

  /* Ensure that 'offset' is a multiple of the pixel ratio */
  float pixel_ratio = screen()->pixel_ratio();
  m_offset = (Vector2f(Vector2i(m_offset / pixel_ratio)) * pixel_ratio);

  Vector2f bound1 = Vector2f(m_size) * pixel_ratio,
	  bound2 = -Vector2f(m_image->size()) * scale();

  if ((m_offset.x() >= bound1.x()) != (m_offset.x() < bound2.x()))
	m_offset.x() = std::max(std::min(m_offset.x(), bound1.x()), bound2.x());
  if ((m_offset.y() >= bound1.y()) != (m_offset.y() < bound2.y()))
	m_offset.y() = std::max(std::min(m_offset.y(), bound1.y()), bound2.y());

  Vector2i viewport_size = render_pass()->viewport().second;

  float scale = std::pow(2.f, m_scale / 5.f);

  /* Matrix4f matrix_background =
	   Matrix4f::scale(Vector3f(m_image->size().x() * scale / 20.f,
								m_image->size().y() * scale / 20.f, 1.f));
 */
  Matrix4f matrix_image =
	  Matrix4f::ortho(0.f, viewport_size.x(), viewport_size.y(), 0.f, -1.f, 1.f) *
		  Matrix4f::translate(Vector3f(m_offset.x(), (int) m_offset.y(), 0.f)) *
		  Matrix4f::scale(Vector3f(m_image->size().x() * scale,
								   m_image->size().y() * scale, 1.f));

  m_image_shader->set_uniform("matrix_image", Matrix4f(matrix_image));
  //m_image_shader->set_uniform("matrix_background", Matrix4f(matrix_background));
  //m_image_shader->set_uniform("background_color", m_image_background_color);

  // set the value for the exposure
  m_image_shader->set_uniform("exposure", m_exposure);

  m_image_shader->begin();
  m_image_shader->draw_array(Shader::PrimitiveType::Triangle, 0, 6, false);
  m_image_shader->end();
}
/// set the exposure value of the image. Implementation inspired by what Thomas Müller did for his nanogui version in the PPG implementation - by Lukas Michel
void ImageViewFalsecolor::set_exposure(float exposure) {
  m_exposure = exposure;
}
bool ImageViewFalsecolor::mouse_motion_event(const Vector2i &p, const Vector2i &rel, int button, int modifiers) {
  if (Widget::mouse_motion_event(p, rel, button, modifiers)) {
	return true;
  }
  if (m_mouseover_pixel_callback) {
	// subtract m_pos from the position of the mouse. m_pos should point to the upper-left corner (0,0) of the widget
	// and can thus be used to correctly handle stacked widgets (i.e. there are widgets above this or left of this widget
	// that influence the actual top-left coordinate of this)
	m_mouseover_pixel_callback(pos_to_pixel(Vector2f{p.x() + .5f, p.y() + .5f}-Vector2f(m_pos)));
  }
  return true;
}

bool ImageViewFalsecolor::mouse_button_event(const Vector2i &p, int button, bool down, int modifiers) {
  if(Widget::mouse_button_event(p, button, down, modifiers))
    return true;
  if (m_mousebutton_pixel_callback) {
    // subtract m_pos from the position of the mouse. m_pos should point to the upper-left corner (0,0) of the widget
    // and can thus be used to correctly handle stacked widgets (i.e. there are widgets above this or left of this widget
    // that influence the actual top-left coordinate of this)
    m_mousebutton_pixel_callback(pos_to_pixel(Vector2f{p.x() + .5f, p.y() + .5f}-Vector2f(m_pos)));
  }
  return true;
}

NAMESPACE_END(nanogui)
