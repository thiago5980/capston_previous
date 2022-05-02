#include "PathPlan.cpp"


int main(int argc, char* argv[])
{
  ros::init(argc, argv, "path_plan");
  ros::Rate loop_rate(1);

  output_path test(0, 0);
  while(ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}