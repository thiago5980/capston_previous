#ifndef _SORT_H_
#define _SORT_H_
#include <iostream>
#include <vector>
#include <algorithm>
#include "rucas_msgs/order.h"
#include "rucas_msgs/isempty.h"
#include "rucas_msgs/robot_purpose_node.h"
#include "ros/ros.h"

using namespace std;

class Search{
private:
  ros::NodeHandle nh;

  ros::Subscriber _sort;
  ros::ServiceClient _isEmpty;
  ros::Publisher _path;
  void cb_sort(const rucas_msgs::order::ConstPtr& sorting);

  vector<pair<int, int>> cb_input = {};
  vector<int64_t> insult = {};
  vector<int64_t> result = {};
  vector<int64_t> firstColum = {};
  vector<int64_t> secondColum = {};
  vector<int64_t> thirdColum = {};
  vector<int64_t> product_quantity = {};
  const int row = 4;
  const int colum = 3;
  const int floor = 4;

  int sort_size = 0;
  bool test_empty = false;
  bool send_data = false;
  bool test = false;
public:
  Search();
  ~Search(){};
  void _getinput();
  void _findroute();
  bool r_empty(){return test_empty;}
  bool r_send_data(){return send_data;}
  void _empty();
  void _clean();
  void _pub_path();
  void _print();
};

#endif
