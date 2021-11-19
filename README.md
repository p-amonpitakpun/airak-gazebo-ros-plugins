# airak-gazebo-ros-plugins

prerequisite
  - ros foxy
  - gazebo
  - gazebo_ros_pkgs

$ mkdir -p ros2_ws/src

build the package with colcon
$ colcon build --symlink-install 

first run this code to setup the model path
$ export GAZEBO_MODEL_PATH=$GAZEBO_MODEL_PATH:~/ros2_ws/src/airak_gazebo_ros_plugins/models

start gazebo with
$ cd airak_gazebo_ros_plugins
$ gazebo world/demo_ddbot.world

use tf for camera_link and for lidar_link
$ ros2 run tf2_ros static_transform_publisher 0.03 0 0.25 -1.57 0 -1.57 base_link camera_link
$ ros2 run tf2_ros static_transform_publisher 0 0 0.16 0 0 0 base_link lidar_link

the ddbot can be driven with geometry_msgs/Twist /ddbot/cmd_vel

use Navigation 2 for reference
https://navigation.ros.org/