/*
    nanogui/imageview.h -- Widget used to display images.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    The false color renderer was implemented by Lukas Michel, based on the visualizer by Thomas Müller in https://github.com/Tom94/practical-path-guiding.git

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include <nanogui/imageview.h>

NAMESPACE_BEGIN(nanogui)

/**
 * \class ImageViewFalsecolor imageview_falsecolor.h nanogui/imageview_falsecolor.h
 *
 * \brief A widget for displaying, panning, and zooming images. Numerical RGBA
 * pixel information is shown at large magnifications.
 */
class NANOGUI_EXPORT ImageViewFalsecolor : public ImageView {
public:
  /// Initialize the widget
  ImageViewFalsecolor(Widget *parent);
  /// set the exposure value of the image. Implementation inspired by what Thomas Müller did for his nanogui version in the PPG implementation
  void set_exposure(float exposure);
  virtual void draw_contents() override;
private:
  /// Added for exposure control
  float m_exposure = 0;
};

NAMESPACE_END(nanogui)
