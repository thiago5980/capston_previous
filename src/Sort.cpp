#include "rucas_robot_main/Sort.h"

Search::Search()
{
  _sort = nh.subscribe("/path_argorithm_publisher", 1, &Search::cb_sort, this);
  _isEmpty = nh.serviceClient<rucas_msgs::isempty>("/isempty_robot_test");
  _path = nh.advertise<rucas_msgs::robot_purpose_node>("/robot_Main", 1);
}
void Search::_getinput()
{
  sort(cb_input.begin(), cb_input.end());
  for(int i = 0; i < sort_size; i++)
  {
    insult.push_back(cb_input[i].first);
    product_quantity.push_back(cb_input[i].second);
  }
  ROS_INFO("get_input");
}
void Search::_findroute()
{
  for(int i=0; i<insult.size(); i++)
  {
    int dif=0, rest=0, colum_dif=0, colum_rest=0;
    dif = insult[i] / (row*floor);
    firstColum.push_back(++dif); // 선반 줄

    rest = insult[i] % (row*floor);
    colum_dif = rest / row;
    colum_rest = rest % row;
    secondColum.push_back(++colum_dif); // 각 선반 마다 위치
    thirdColum.push_back(colum_rest); // 각 선반 마다의 층
  }
  ROS_INFO("find_route");
}
void Search::cb_sort(const rucas_msgs::order::ConstPtr& sorting)
{
  for(int i=0; i<sorting->product_id.size(); i++)
  {
    cb_input.push_back(pair<int, int>(sorting->product_id[i], sorting->quantity[i]));
  }
  test_empty = true;
  sort_size = sorting->product_id.size();
  ROS_INFO("cb_sort");
}
void Search::_empty()
{
  rucas_msgs::isempty f_em;
  rucas_msgs::isempty b_em;
  bool next_step = false;
  f_em.request.robot_empty = true;
  if(_isEmpty.call(f_em)){
    if(!f_em.response.robot_working)
    {
      b_em.request.robot_start_now = true;
      next_step = true;
    }
  }
  if(next_step && _isEmpty.call(b_em)){
    if(b_em.response.robot_working) send_data = true;
  }
  ROS_INFO("empty");
}
void Search::_pub_path()
{
  rucas_msgs::robot_purpose_node send;
  send.purpose_node_x = firstColum;
  send.purpose_node_y = secondColum;
  send.node_floor = thirdColum;
  send.quantity = product_quantity;
  send.product_size = sort_size;
  _path.publish(send);
  ROS_INFO("pub_path");
}
void Search::_clean()
{
  cb_input = {};
  insult = {};
  result = {};
  firstColum = {};
  secondColum = {};
  thirdColum = {};
  test_empty = false;
  send_data = false;
}
void Search::_print()
{
  for(int i=0; i<sort_size; i++)
  {
    ROS_INFO("x : %d, y : %d, z : %d, quantity : %d", firstColum[i], secondColum[i], thirdColum[i], product_quantity[i]);
  }
  ROS_INFO("Sort_size : %d", sort_size);
}