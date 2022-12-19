cmake_minimum_required(VERSION 3.0.2)
project(cam_image_sub)

find_package(
  
  catkin REQUIRED COMPONENTS
  image_transport
  roscpp
  sensor_msgs
  std_msgs
  cv_bridge
  
)

find_package (OpenCV REQUIRED)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES opencv
  CATKIN_DEPENDS cv_bridge image_transport roscpp sensor_msgs std_msgs
  
)
include_directories(
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)
add_executable(img_sub_node src/img_sub.cpp)
target_link_libraries(img_sub_node ${catkin_LIBRARIES})

