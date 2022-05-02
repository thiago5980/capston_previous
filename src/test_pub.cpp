#include "ros/ros.h"
#include "rucas_msgs/order.h"

using namespace std;
int main(int argc, char* argv[])
{
  ros::init(argc, argv, "test_pub");
  ros::NodeHandle nh;  

  ros::Publisher _pub = nh.advertise<rucas_msgs::order>("/path_argorithm_publisher", 1);
  rucas_msgs::order pub_test;
    int a = 0;
  while(ros::ok())
  {
    pub_test.product_id = {2,3,1,5,10};
    pub_test.quantity = {2,1,4,1,2};
    ros::Rate loop_rate(30);
    _pub.publish(pub_test);

    ros::spinOnce();

    loop_rate.sleep();
  }

  return 0;
}
