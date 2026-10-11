#include <chrono>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;
/*2*/
class StatusMonitor : public rclcpp::Node
{
public:
  StatusMonitor() : Node("status_monitor")
  {
    last_msg_time_ = this->now();

    subscription_ = this->create_subscription<std_msgs::msg::String>(
      "robot_status", 10,
      [this](const std_msgs::msg::String & msg) {
        last_msg_time_ = this->now();
        RCLCPP_INFO(this->get_logger(), "Heard: '%s'", msg.data.c_str());
      });

    watchdog_ = this->create_wall_timer(1s, [this]() {
      double elapsed = (this->now() - last_msg_time_).seconds();
      if (elapsed > 5.0) {
        RCLCPP_WARN(this->get_logger(),
          "No status for %.1f s! Robot may be down.", elapsed);
      }
    });
  }
  /*3*/
  private:
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
  rclcpp::TimerBase::SharedPtr watchdog_;
  rclcpp::Time last_msg_time_;
};
/*4*/
int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<StatusMonitor>());
  rclcpp::shutdown();
  return 0;
}