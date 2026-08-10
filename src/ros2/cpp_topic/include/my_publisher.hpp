#ifndef _MY_PUBLISHER_HPP__
#define _MY_PUBLISHER_HPP__

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int16.hpp"
#include "std_msgs/msg/string.hpp"

using int16Msg = std_msgs::msg::Int16;
typedef std_msgs::msg::String stringMsg;

class PubNode: public rclcpp::Node{
public: 
  PubNode();
  ~PubNode();
  void pubCallbackInt();
  void pubCallbackString();
private:
  int count_;
  rclcpp::Publisher<int16Msg>::SharedPtr pub_int;
  rclcpp::Publisher<stringMsg>::SharedPtr pub_string;
  rclcpp::TimerBase::SharedPtr timer_;
};

#endif