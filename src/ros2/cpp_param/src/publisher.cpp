#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/msg/string.hpp"
#define FREQ 1

// using namespace std::chrono_literals  // 1s gibi kavramların sistemde doğrudan kullanılmasını sağlarız
using string_msg = example_interfaces::msg::String;

class MyNode: public rclcpp::Node{
public:
    MyNode():Node("cpp_talker_node"){
        this->declare_parameter("cpp_class_name",2);
        this->declare_parameter("freq_pub", FREQ);
        frequency = this->get_parameter("freq_pub").as_int();
        rclcpp::Parameter number_ = this->get_parameter("cpp_class_name");
        RCLCPP_INFO(this->get_logger(), "%ld) Talker started... ", number_.as_int());
        publisher_ = this->create_publisher<string_msg>("cpp_test_counter", 10);
        timer_ = this->create_wall_timer(std::chrono::milliseconds((int)(1000.0 /frequency)), std::bind(&MyNode::publisherCallback, this));
    }
    
private:
    void publisherCallback(){
        // RCLCPP_INFO(this->get_logger()," Hello");
        rclcpp::Parameter deneme;
        if(this->get_parameter("number", deneme)){
            RCLCPP_INFO(this->get_logger()," True %d", deneme.as_int());
        }else{
            RCLCPP_INFO(this->get_logger()," False %d", deneme);
        }
        string_msg msg;  // auto msg = string_msg;
        msg.data = std::string(" Hello ");
        publisher_->publish(msg);
    }
    // int number_;
    int frequency;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<string_msg>::SharedPtr publisher_;
    
};


int main(int argc, char ** args){
    rclcpp::init(argc, args);
    rclcpp::spin(std::make_shared<MyNode>());
    rclcpp::shutdown();
}