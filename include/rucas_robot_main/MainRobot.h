#ifndef _MAINROBOT_H_
#define _MAINROBOT_H_

#include "ros/ros.h"
#include "rucas_msgs/robot_purpose.h"
#include "rucas_msgs/door_open.h"
#include "rucas_msgs/isempty.h"
#include "rucas_msgs/linear_on.h"
#include "rucas_msgs/marker_msgs.h"
#include "rucas_msgs/robot_current.h"
#include "geometry_msgs/Twist.h"
#include "rucas_msgs/robot_purpose_node.h"
#include "rucas_msgs/node_moving.h"
#include "rucas_msgs/correction_node.h"
#include <chrono>
using namespace std;

#include <vector>
class MainRobot{
private:
  ros::NodeHandle nh;

  ros::Publisher robot_movement;
  ros::ServiceClient linear_movement;
  ros::ServiceClient door_movement;
  ros::ServiceServer marker_input;
  ros::ServiceClient position_feedback;
  ros::Subscriber robot_path;
  ros::ServiceServer is_empty;
  ros::ServiceClient robot_node_moving;
  ros::ServiceClient marker_correction;

  bool cb_Marker_input(rucas_msgs::marker_msgs::Request &req, rucas_msgs::marker_msgs::Response &res);
  void cb_Robot_path(const rucas_msgs::robot_purpose_node::ConstPtr& purpose_node_msg);
  bool cb_Is_Empty(rucas_msgs::isempty::Request &req, rucas_msgs::isempty::Response &res);

  vector<int> product_path_x;
  vector<int> product_path_y;
  vector<int> product_path_floor;
  vector<int> product_quantity;
  int path_product_size = 0;
  int loop_instruction = 0;
  bool correction_start = false;
  bool correction_finish = false;
  bool linear_start = false;
  bool linear_finish = false;
  bool cbPathDone = false;
  float correction_done;
  float correction_position_y;

public:
  MainRobot();
  ~MainRobot(){};
  void pub_robot_move(float x_vel, float angular, float time);
  bool is_robot_start;
  int return_path_product_size();
  void moving_node();
  bool return_correction_start(){return correction_start;}
  bool return_correction_finish(){return correction_finish;}
  bool return_linear_finish(){return linear_finish;}
  void _marker_correction();
  void _linear();
  void _clear();
};
#endif
