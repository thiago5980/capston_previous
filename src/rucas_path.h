struct target_posion{
  float target_position_x_vel;
  float target_position_angular;
  float target_position_time;
};

target_posion route_path[4] =
{
  {1.0, 0, 5.0}, // straight one path
  {0, 1.0, 3.0}, // turn right
  {0, -1.0, 3.0}, // turn left
  {-1.0, 0, 5.0} // back
};

enum direction {straight = 0, right = 1, left = 2, back = 3};

