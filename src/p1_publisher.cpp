#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;


class MinimalPublisher : public rclcpp::Node
{
  public:
    MinimalPublisher()
    : Node("minimal_publisher"), count_(0)
    {
      fast_publisher_ = this->create_publisher<std_msgs::msg::String>("fast_topic", 10);
      fast_timer_ = this->create_wall_timer(
      500ms, std::bind(&MinimalPublisher::fast_timer_callback, this));

      slow_publisher_ = this->create_publisher<std_msgs::msg::String>("slow_topic", 10);
      slow_timer_ = this->create_wall_timer(
      1000ms, std::bind(&MinimalPublisher::slow_timer_callback, this));
    }

  private:
    void fast_timer_callback()
    {
      auto message = std_msgs::msg::String();
      message.data = "[Fast] Fast timer" + std::to_string(count_++);
      RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
      fast_publisher_->publish(message);
    }
    rclcpp::TimerBase::SharedPtr fast_timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr fast_publisher_;
  
    
    void slow_timer_callback()
    {
      auto message = std_msgs::msg::String();
      message.data = "[Slow] Slow timer" + std::to_string(count_++);
      RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
      slow_publisher_->publish(message);
    }
    rclcpp::TimerBase::SharedPtr slow_timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr slow_publisher_;
    size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}