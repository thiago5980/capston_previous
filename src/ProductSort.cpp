#include "Sort.cpp"
int main(int argc, char* argv[])
{
  ros::init(argc, argv, "ruscas_robot_path");
  Search first;
  ros::Rate loop_rate(1);
  while(ros::ok())
  {
    if(first.r_empty()){
      first._getinput();
      first._findroute();
      first._empty();
      first._pub_path();
      first._print();
      first._clean();
    }
    ros::spinOnce();

    loop_rate.sleep();
  }
}