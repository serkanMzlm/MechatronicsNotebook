#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from example_interfaces.msg import String


class MyNode(Node):
    def __init__(self):
        super().__init__("py_talker_node")
        self.counter_ = 0
        self.publishers_ = self.create_publisher(String, "py_test_counter", 10)
        self.timer_ = self.create_timer(0.5, self.publisherCallback)
        self.get_logger().info("py_talker started...")

    def publisherCallback(self):
        self.counter_ += 1
        msg = String()
        msg.data = str(self.counter_)+") Hello"
        self.publishers_.publish(msg)
        self.get_logger().info(str(self.counter_)+") Hello")


def main(args=None):
    rclpy.init(args=args)
    node = MyNode()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == "__main__":
    main()