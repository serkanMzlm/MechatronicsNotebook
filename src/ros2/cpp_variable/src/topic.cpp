#include "rclcpp/rclcpp.hpp"
#include "variable/msg/info.hpp"

// using namespace std::chrono_literals  1s gibi kavramların sistemde doğrudan kullanılmasını sağlarız
using msg_info = variable::msg::Info;

class MyNode: public rclcpp::Node{
public:
    MyNode():Node("cpp_topic_node"){
        publisher_ = this->create_publisher<msg_info>("info_msg_cpp", 10);
        timer_ = this->create_wall_timer(std::chrono::seconds(1), std::bind(&MyNode::publisherCallback, this));
        // timer_ = this->create_wall_timer(1s, std::bind(&MyNode::publisherCallback, this));  using namespace std::chrono_literals kısmı açılırsa kullanılabilir.
    }
    
private:
    void publisherCallback(){
        msg_info msg;  // auto msg = string_msg;
        msg.name = "ROS2";
        msg.age  =  10;
        publisher_->publish(msg);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<msg_info>::SharedPtr publisher_;
    
};


int main(int argc, char ** args){
    rclcpp::init(argc, args);
    rclcpp::spin(std::make_shared<MyNode>());
    rclcpp::shutdown();
}