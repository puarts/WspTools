
#include "cl-camera.h"
#include <wsp/common/fn-debug.h>

wsp::graphic::Camera::Camera(int viewport_width, int viewport_height)
    : field_of_view_vertical_(50.0f * (float)M_PI / 180.0f)
    , viewport_width_(1)
    , viewport_height_(1)
    , near_clip_(0.1f)
    , far_clip_(1000.0f)
    , position_(wsp::Point  <float>(0.0f, 0.0f, 0.0f))
    , rotation_(wsp::Vector3<float>(0.0f, 0.0f, 0.0f))
    , axis_x_(wsp::Vector3<float>(1.0f, 0.0f, 0.0f))
    , axis_y_(wsp::Vector3<float>(0.0f, 1.0f, 0.0f))
    , axis_z_(wsp::Vector3<float>(0.0f, 0.0f, 1.0f))
    , aim_position_x_(0.0f)
    , aim_position_y_(0.0f)
    , aim_position_z_(1.0f)
    , up_vector_x_(0.0f)
    , up_vector_y_(1.0f)
    , up_vector_z_(0.0f)
    , camera_type_(CAMERA_TYPE_PERSPECTIVE)
{
    WSP_ASSERT(viewport_width > 0, "viewport_width must be larger then zero: %d", viewport_width);
    WSP_ASSERT(viewport_height > 0, "viewport_height must be larger then zero: %d", viewport_height);
    viewport_width_ = viewport_width;
    viewport_height_ = viewport_height;
}

void wsp::graphic::Camera::set_viewport_width(int viewport_width)
{
    WSP_ASSERT(viewport_width > 0, "viewport_width must be larger then zero: %d", viewport_width);
    viewport_width_ = viewport_width;
}

void wsp::graphic::Camera::set_viewport_height(int viewport_height)
{
    WSP_ASSERT(viewport_height > 0, "viewport_height must be larger then zero: %d", viewport_height);
    viewport_height_ = viewport_height;
}