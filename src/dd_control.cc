#include <gazebo/common/Plugin.hh>
#include <gazebo/physics/Model.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo_ros/node.hpp>
#include <rclcpp/rclcpp.hpp>

#include <std_msgs/msg/float64_multi_array.hpp>

#include <memory>

#include <iostream>
#include <vector>
#include <algorithm>

// using std::placeholders::_1;

namespace gazebo_plugins
{
    class DDControl : public gazebo::ModelPlugin
    {
    public:
        struct Vel
        {
            double l;
            double r;
        };
        struct Vel vel = {0.0, 0.0};

        std::string left_wheel_name;
        std::string right_wheel_name;

        gazebo::event::ConnectionPtr update_connection_;
        gazebo_ros::Node::SharedPtr ros_node_;

        void Load(gazebo::physics::ModelPtr model, sdf::ElementPtr sdf)
        {
            this->model = model;
            this->ros_node_ = gazebo_ros::Node::Get(sdf);
            this->vel_sub = this->ros_node_->create_subscription<std_msgs::msg::Float64MultiArray>(
                "ddbot/vel",
                10,
                std::bind(&DDControl::topic_callback, this, std::placeholders::_1));

            RCLCPP_INFO(this->ros_node_->get_logger(), model->GetName().c_str());


            this->update_connection_ = gazebo::event::Events::ConnectWorldUpdateBegin(std::bind(&DDControl::OnUpdate, this));

            jointController = this->model->GetJointController();
            jointController->Reset();

            jointController->AddJoint(this->model->GetJoint("left_wheel_hinge"));
            jointController->AddJoint(this->model->GetJoint("right_wheel_hinge"));

            this->left_wheel_name = this->model->GetJoint("left_wheel_hinge")->GetScopedName();
            this->right_wheel_name = this->model->GetJoint("right_wheel_hinge")->GetScopedName();

            jointController->SetVelocityTarget(this->right_wheel_name, 0.0);
            jointController->SetVelocityTarget(this->left_wheel_name, 0.0);

            printf("model loaded...\n");
        }

        void OnUpdate()
        {
            // printf("vel\t %f\t %f\n", this->vel.l, this->vel.r);
            this->jointController = this->model->GetJointController();
            this->jointController->SetVelocityTarget(this->left_wheel_name, this->vel.l);
            this->jointController->SetVelocityTarget(this->right_wheel_name, this->vel.r);
        }

    private:
        gazebo::physics::ModelPtr model;
        gazebo::physics::JointControllerPtr jointController;
        
        rclcpp::Subscription<std_msgs::msg::Float64MultiArray>::SharedPtr vel_sub;

        void topic_callback(const std_msgs::msg::Float64MultiArray::SharedPtr msg)
        {
            this->vel.l = msg->data[0];
            this->vel.r = msg->data[1];
        }
    };

    // Register this plugin with the simulator
    GZ_REGISTER_MODEL_PLUGIN(DDControl)
} // namespace gazebo_plugins