cmake_minimum_required(VERSION 3.0.2)
project(rucas_robot_main)
find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  rospy
  std_msgs
  rucas_msgs
)
catkin_package(
 INCLUDE_DIRS include
 LIBRARIES rucas_robot_main
 CATKIN_DEPENDS geometry_msgs roscpp rospy std_msgs rucas_msgs
 DEPENDS system_lib
)
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)
add_executable(rucas_robot_main_node src/rucas_robot_main.cpp)
add_dependencies(rucas_robot_main_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(rucas_robot_main_node
  ${catkin_LIBRARIES}
)

add_executable(rucas_product_sort src/ProductSort.cpp)
add_dependencies(rucas_product_sort ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(rucas_product_sort
  ${catkin_LIBRARIES}
)

add_executable(pub_test src/test_pub.cpp)
add_dependencies(pub_test ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(pub_test
  ${catkin_LIBRARIES}
)
add_executable(PathPlan src/PathPlanner.cpp)
add_dependencies(PathPlan ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(PathPlan
  ${catkin_LIBRARIES}
)
