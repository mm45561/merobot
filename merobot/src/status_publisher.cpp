#include <chrono>
#include <memory>
#include <string>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;
class StatusPublisher : public rclcpp::Node
{
public:
  StatusPublisher() : Node("status_publisher"), count_(0)
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>("robot_status", 10);

    timer_ = this->create_wall_timer(3s, [this]() {
      auto msg = std_msgs::msg::String();
      msg.data = "it is me, tick " + std::to_string(count_++);
      RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", msg.data.c_str());
      publisher_->publish(msg);
    });
  }
  private:
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  size_t count_;
};
int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<StatusPublisher>());
  rclcpp::shutdown();
  return 0;
}