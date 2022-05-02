#include "MainRobot.cpp"
#include <vector>

using namespace std;


int main(int argc, char *argv[])
{
  ros::init(argc, argv, "ruscas_robot_main");
  MainRobot sequence;
  ros::Rate loop_rate(1);

  
  vector<int> product_want;
  while(ros::ok())
  {
    if(sequence.is_robot_start && !(sequence.return_correction_start()))
      {
        sequence.moving_node();
        ROS_INFO("good %d", sequence.return_correction_start());
      }
    if(sequence.return_correction_start() && (!sequence.return_correction_finish())) 
      {
        sequence._marker_correction();
        ROS_INFO("good condition %d", sequence.return_correction_finish());
      }
    if(sequence.return_correction_finish() && (!sequence.return_linear_finish()))
      sequence._linear();
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
