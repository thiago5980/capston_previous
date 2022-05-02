#include "rucas_robot_main/PathPlan.h"

using namespace std;

output_path::output_path(int row, int colum)
{
  _move = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
  _nodesrv = nh.advertiseService("/node_move", &output_path::_cbNodesrv, this);
  next_path_row = row;
  next_path_colum = colum;
  this->path_cal();
}
int output_path::path_cal()
{
  if(prev_path_row < 0 || prev_path_colum < 0) {std::cout << "wrong_path" << endl; return 0;}
  if(prev_path_colum != 0 && prev_path_row != 0)
  {
    if(prev_path_row != 0)
    {
      if(prev_path_colum == next_path_colum) 
      {
        int value_r = next_path_row - prev_path_row;
        int direction = 0;
        if(value_r <= 0) direction = -1;
        else direction = 1;
        output_matrix.push_back({direction*node_one_vel_x, abs(value_r*node_one_time_x), LINEAR});
      }
      else
      {
        int value_r = (-prev_path_row);
        output_matrix.push_back({-node_one_vel_x, abs(value_r*node_one_time_x), LINEAR});      
      }
    }
    if(prev_path_colum != 0)
    {
      output_matrix.push_back({-node_one_vel_z, node_one_time_z, ANGULAR});
    }
  }
  int f_value_row = next_path_colum - prev_path_colum;
  int direction = 0;
  if(f_value_row <= 0) direction = -1;
  else direction = 1;

  output_matrix.push_back({direction*node_one_vel_x, abs(f_value_row*node_one_time_x), LINEAR});
  if(next_path_colum != 0 && next_path_row != 0)
  {
    output_matrix.push_back({node_one_vel_z, node_one_time_z, ANGULAR});
    
    int d_value_row = next_path_row - prev_path_row;

    if(prev_path_colum != 0 && prev_path_row != 0) d_value_row = next_path_row;
    if(d_value_row <= 0) direction = -1;
    else direction = 1;
    output_matrix.push_back({direction*node_one_vel_x, abs(d_value_row*node_one_time_x), LINEAR});
  }

  prev_path_colum = next_path_colum;
  prev_path_row = next_path_row;
  return 0;
}
void output_path::push_node(int push_row, int push_colum)
{
  next_path_colum = push_colum;
  next_path_row = push_row;
  this->path_cal();
}
void output_path::print_path()
{
  for(auto test_node: output_matrix)
  {
    cout << "test_node_vel : " << test_node[0] << endl;
    cout << "test_node_time : " << test_node[1] << endl;
  }
}
void output_path::clear_node()
{
  this->output_matrix = {};
}
void output_path::_pushmove()
{
  ROS_INFO("In_pushmove");
  geometry_msgs::Twist input_msg;
  for(auto test_node : output_matrix)
  {
    if(test_node[2] == ANGULAR)
    {
      input_msg.angular.z = test_node[0];
      input_msg.linear.x = 0;
      this->_movement(test_node[1], input_msg);
    }
    if(test_node[2] == LINEAR)
    {
      input_msg.angular.z = 0;
      input_msg.linear.x = test_node[0];
      this->_movement(test_node[1], input_msg);
    }
  }
}
void output_path::_movement(float m_time, geometry_msgs::Twist m_route)
{
  auto start = std::chrono::steady_clock::now();
  auto now = std::chrono::steady_clock::now();
  
  geometry_msgs::Twist stop;
  stop.linear.x = 0;
  
  while(time_duration.count() < m_time)
  {
    _move.publish(m_route);
    now = std::chrono::steady_clock::now();
    time_duration = now - start;
  }
  _move.publish(stop);
  time_duration = std::chrono::milliseconds::zero();
}
bool output_path::_cbNodesrv(rucas_msgs::node_moving::Request &res, rucas_msgs::node_moving::Response &req)
{
  next_path_row = res.node_position[1];
  next_path_colum = res.node_position[0];
  ROS_INFO("next_path_row = %d", res.node_position[1]);
  ROS_INFO("next_path_colum = %d", res.node_position[0]);
  this->push_node(next_path_row, next_path_colum);
  this->_pushmove();
  this->print_path();  
  this->clear_node();
  req.done_moving = true;
  return true;
}