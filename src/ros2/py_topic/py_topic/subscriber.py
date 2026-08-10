#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from example_interfaces.msg import String


class MyNode(Node):
    def __init__(self):
        super().__init__("py_listener_node")
        self.subscriber_ = self.create_subscription(
            String, "py_test_counter", self.subscriberCallback, 10)
        self.get_logger().info("py_listener started...")

    def subscriberCallback(self, msg):
        self.get_logger().info(msg.data)


def main(args=None):
    rclpy.init(args=args)
    rclpy.spin(MyNode())
    rclpy.shutdown()


if __name__ == "__main__":
    main()