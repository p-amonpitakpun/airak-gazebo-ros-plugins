# airak-gazebo-ros-plugins


first run to setup the model path
```
export GAZEBO_MODEL_PATH=$GAZEBO_MODEL_PATH:~/ros2_ws/src/airak_gazebo_ros_plugins/models
```

use tf for camera_link
ros2 run tf2_ros static_transform_publisher 0 0 0.2 -1.57 0 -1.57 base_link camera_link
