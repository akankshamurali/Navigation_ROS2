#include<geometry_msgs/msg/transform_stamped.hpp>
#include<geometry_msgs/msg/twist.hpp>
#include <rclcpp/rclcpp.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include<tf2/exceptions.h>
#include <tf2_ros/transform_broadcaster.h>
#include <turtlesim/msg/pose.hpp>
#include<turtlesim/srv/spawn.hpp>
#include<tf2_ros/buffer.h>

#include <memory>
#include <string>
#include<chrono>

using std::placeholders::_1;
using namespace std::chrono_literals;

class testing : public rclcpp::Node
{
    float pos_x, pos_y, pos_z;
    int flag =0;
    public:
    testing()
    : Node("turtle_tf2_frame_publisher")
    {
        this-> declare_parameter<std::string>("turtlename","turtle1");
        //this->get_parameter("turtlename","turtle1");
        tf_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
        std::ostringstream stream;
        stream<<"/"<<turtlename_.c_str()<<"pose";
        std::string topic_name = stream.str();
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 1);
        subscriber_ = this->create_subscription<turtlesim::msg::Pose>(topic_name, 10, std::bind(&testing::handle_turtle_pose, this, _1));
        timer_ = this->create_wall_timer(500ms, std::bind(&testing::temp_funct, this));
    }
    void handle_turtle_pose(const std::shared_ptr<turtlesim::msg::Pose>)
    {
        geometry_msgs::msg::TransformStamped t;
        turtlesim::msg::Pose p;
        if(flag == 1){
        pos_x = p.x;
        pos_y = p.y;
        pos_z = 0;
        //tf::Quaternion q(0, 0, goal.pose.orientation.z, goal.pose.orientation.w);
        //double roll, pitch, yaw;
        //m.getRCPP(roll, pitch, yaw);
        //t.p.orientation.x = 0;
        //t.p.orientation.y = 0;
        //t.p.orientation.z = yaw;
        //t.p.orientation.w = 0;
        }
    }
    void temp_funct(){
        rclcpp::Time now = this->get_clock()->now();
        geometry_msgs::msg::Twist msg1;
        float error[] = {0,0,0};
        float target[] = {1,1,0};
        float pos_arr[] = {pos_x, pos_y, pos_z};
        int i;
        if(flag == 0){
        for(i=0; i<3; i++)
        {
            error[i] = target[i] - pos_arr[i];
        }
        msg1.linear.x = error[0];
        msg1.linear.y = error[1];
        msg1.linear.z = 0.0;
        publisher_->publish(msg1);
        flag = 1;
        }
    }
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr subscriber_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    std::string turtlename_;
    std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
    std::string target_frame_;
};
int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<testing>());
  rclcpp::shutdown();
  return 0;
}