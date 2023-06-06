#include "Camera.h"

void Camera::set_rotation(const glm::quat &_q)
{
  glm::mat4 rotation_matrix = glm::mat4_cast(_q);
  right_dir_ = glm::normalize(glm::vec3(rotation_matrix[0]));
  up_dir_ = glm::normalize(glm::vec3(rotation_matrix[1]));
  front_dir_ = -glm::normalize(glm::vec3(rotation_matrix[2]));
}

const glm::quat Camera::get_rotation() const
{
  return glm::quat_cast(glm::mat4(glm::vec4(right_dir_, 0.0f), glm::vec4(up_dir_, 0.0f), glm::vec4(-front_dir_, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
}

void Camera::set_pose(const glm::quat &_q, const glm::vec3 &_t)
{
  set_rotation(_q);
  position_ = _t;
}

void Camera::get_pose(glm::quat &_q, glm::vec3 &_t) const
{
  _q = get_rotation();
  _t = position_;
}

const glm::mat4 Camera::get_pose() const
{
  return glm::translate(glm::mat4_cast(get_rotation()), position_);
}

void Camera::set_pose(const glm::mat4 &_frame)
{
  position_ = glm::vec3(_frame[3]);
  set_rotation(glm::quat_cast(_frame));
}

void Camera::set_pose(const glm::vec3 &_pos, const glm::vec3 &_at, const glm::vec3 &_up_dir)
{
  position_ = _pos;
  front_dir_ = glm::normalize(_pos - _at);
  right_dir_ = glm::normalize(glm::cross(_up_dir, front_dir_));
  up_dir_ = glm::normalize(glm::cross(front_dir_, right_dir_));
  set_rotation(glm::quat_cast(glm::mat4(glm::vec4(right_dir_, 0.0f), glm::vec4(up_dir_, 0.0f), glm::vec4(-front_dir_, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))));
}

const glm::mat4 Camera::get_view_matrix() const
{
  return glm::inverse(get_pose());
}
const glm::mat4 Camera::get_projection_matrix() const
{
  if (mode_ == kOrtho)
  {
    float half_width = ortho_scale_ * aspect_;
    float half_height = ortho_scale_;
    return glm::ortho(-half_width, half_width, -half_height, half_height, near_, far_);
  }
  else
  {
    return glm::perspective(glm::radians(fovy_), aspect_, near_, far_);
  }
}

void Camera::move_forward(float delta)
{
  position_ += front_dir_ * delta;
}

void Camera::move_backward(float delta)
{
  position_ -= front_dir_ * delta;
}

void Camera::move_left(float delta)
{
  position_ -= right_dir_ * delta;
}

void Camera::move_right(float delta)
{
  position_ += right_dir_ * delta;
}

void Camera::move_up(float delta)
{
  position_ += up_dir_ * delta;
}

void Camera::move_down(float delta)
{
  position_ -= up_dir_ * delta;
}
