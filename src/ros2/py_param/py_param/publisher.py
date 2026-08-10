#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from example_interfaces.msg import String


class MyNode(Node):
    def __init__(self):
        super().__init__("py_param_node")
        self.declare_parameter("class_name", "my_class")
        self.declare_parameter("class_number", 5)          #Default olarak ayarlamak zorunda değiliz. 
        #Eğer Default paramatre vermezsek node her başlatığımızda param değerini atama yapmamız lazım 
        self.name = self.get_parameter("class_name")
        self.number = self.get_parameter("class_number").value
        self.publishers_ = self.create_publisher(String, "py_test_param", 10)
        self.timer_ = self.create_timer(0.5, self.publisherCallback)
        self.get_logger().info("started...")

    def publisherCallback(self):
        msg = String()
        msg.data = "Hello"
        self.publishers_.publish(msg)

def main(args=None):
    rclpy.init(args=args)
    node = MyNode()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == "__main__":
    main()