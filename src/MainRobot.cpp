#include "rucas_robot_main/MainRobot.h"

MainRobot::MainRobot()
{
  robot_movement = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
  linear_movement = nh.serviceClient<rucas_msgs::linear_on>("/linear_start");
  door_movement = nh.serviceClient<rucas_msgs::door_open>("/rucas_door_open");
  marker_input = nh.advertiseService("/marker", &MainRobot::cb_Marker_input, this);
  position_feedback = nh.serviceClient<rucas_msgs::robot_current>("/robot_test_position");
  robot_path = nh.subscribe("/robot_Main", 1, &MainRobot::cb_Robot_path, this);
  is_empty = nh.advertiseService("/isempty_robot_test", &MainRobot::cb_Is_Empty, this);
  robot_node_moving = nh.serviceClient<rucas_msgs::node_moving>("/node_move");
  marker_correction = nh.serviceClient<rucas_msgs::correction_node>("/maker_correction_node");
  is_robot_start = false;
}

bool MainRobot::cb_Marker_input(rucas_msgs::marker_msgs::Request &req, rucas_msgs::marker_msgs::Response &res)
{
  correction_done = req.position_correction[0];
  correction_position_y = req.position_correction[1];
  res.marker_done = 1;
  return true;
}

void MainRobot::cb_Robot_path(const rucas_msgs::robot_purpose_node::ConstPtr& purpose_node_msg)
{
  if(is_robot_start && (!cbPathDone))
  {
    for(int product_x:purpose_node_msg->purpose_node_x){
      product_path_x.push_back(product_x);
      ROS_INFO("good");
    }
    for(int product_y:purpose_node_msg->purpose_node_y){
      product_path_y.push_back(product_y);
      ROS_INFO("y");
    }
    for(int product_z:purpose_node_msg->node_floor){
      product_path_floor.push_back(product_z);
      ROS_INFO("Z");
    }
    for(int quantity:purpose_node_msg->quantity){
      product_quantity.push_back(quantity);
    }
    path_product_size = purpose_node_msg->product_size;
    ROS_INFO("size");
    cbPathDone = true;
  }
}

bool MainRobot::cb_Is_Empty(rucas_msgs::isempty::Request &req, rucas_msgs::isempty::Response &res)
{
  if(!is_robot_start && req.robot_empty)
  {
    res.robot_working = false;
  }
  else
  {
    res.robot_working = true;
  }
  if(req.robot_start_now)
  {
    is_robot_start = true;
    res.robot_working = true;
  }
  return true;
}

void MainRobot::pub_robot_move(float x_vel, float angular, float time)
{
  if(x_vel>0 && angular==0)
  {
    geometry_msgs::Twist push_x_vel;
    push_x_vel.linear.x = x_vel;
  }
}

int MainRobot::return_path_product_size()
{return this->path_product_size;}


void MainRobot::moving_node()
{
  rucas_msgs::node_moving position;
  ROS_INFO("moving_node");
  if(path_product_size != 0)
  {
    position.request.node_position = 
      {product_path_x[loop_instruction], product_path_y[loop_instruction]};
    ROS_INFO("in_moving_node");
    if(robot_node_moving.call(position)) correction_start = true;
  }
}

void MainRobot::_marker_correction()
{
  rucas_msgs::correction_node correct;
  
  if(correction_start)
  {
    correct.request.maker_correction_start = true;
    if(marker_correction.call(correct)) correction_finish = true;
  }
}

void MainRobot::_linear()
{
  rucas_msgs::linear_on start_linear;

  if(correction_finish)
  {
    start_linear.request.floor = product_path_floor[loop_instruction];
    start_linear.request.quantity = product_quantity[loop_instruction];
    if(linear_movement.call(start_linear)) {
      linear_finish = true;
      loop_instruction++;
    }
  }
}

void MainRobot::_clear(){
  correction_start = false;
  correction_finish = false;
  linear_start = false;
  linear_finish = false;
  cbPathDone = false;
}
