#include "rclcpp/rclcpp.hpp"
#include "variable/srv/add_three_ints.hpp"

typedef variable::srv::AddThreeInts AddThreeInts_msg;
using std::placeholders::_1;
using std::placeholders::_2;


class MyNode: public rclcpp::Node{
public:
    MyNode():Node("cpp_server"){
        RCLCPP_INFO(this->get_logger(),"Server has been started.");
        server_ = this->create_service<AddThreeInts_msg>("add", std::bind(&MyNode::serverCallback, this, _1, _2));
        
    }
    
private:
    void serverCallback(const AddThreeInts_msg::Request::SharedPtr request, const AddThreeInts_msg::Response::SharedPtr response){
        response->sum = request->a + request->b + request->c;
        RCLCPP_INFO(this->get_logger(), "%ld + %ld = %ld", request->a, request->b, response->sum);
    }
    rclcpp::Service<AddThreeInts_msg>::SharedPtr server_;
};


int main(int argc, char ** args){
    rclcpp::init(argc, args);
    rclcpp::spin(std::make_shared<MyNode>());
    rclcpp::shutdown();
}