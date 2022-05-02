#ifndef _PATHPLAN_H_
#define _PATHPLAN_H_
#include <iostream>
#include <vector>
#include <chrono>
#include <geometry_msgs/Twist.h>
#include "ros/ros.h"
#include "rucas_msgs/node_moving.h"
using namespace std;
class output_path
{
private:
  int prev_path_row = 0;
  int prev_path_colum= 0;
  
  int next_path_row;
  int next_path_colum;

  const float node_one_vel_x = 1.0;
  const float node_one_time_x = 1.5;

  const float node_one_vel_z = 1.0;
  const float node_one_time_z = 1.3; 

  const float ANGULAR = 1;
  const float LINEAR = 2;
  std::chrono::steady_clock::time_point now;
  std::chrono::steady_clock::time_point start;
  std::chrono::duration<double> time_duration = now - start;

  vector<vector<float>> output_matrix;

  ros::NodeHandle nh;
  ros::Publisher _move;
  ros::ServiceServer _nodesrv;

  bool _cbNodesrv(rucas_msgs::node_moving::Request &req, rucas_msgs::node_moving::Response &res);

public:
  output_path(int row, int colum);
  ~output_path(){};
  int path_cal_test(int for_row, int for_colum);
  int path_cal();
  void push_node(int push_row, int push_colum);
  void print_path();
  void clear_node();
  void _movement(float m_time, geometry_msgs::Twist m_route);
  void _pushmove();
};

#endif
