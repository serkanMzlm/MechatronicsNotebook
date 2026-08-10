#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from variable.msg import Info


class MyNode(Node):
    def __init__(self):
        super().__init__("py_topic_node")
        self.counter_ = 0
        self.publishers_ = self.create_publisher(Info, "info_msg", 10)
        self.timer_ = self.create_timer(0.5, self.publisherCallback)
        self.get_logger().info("py_my_msg started...")

    def publisherCallback(self):
        self.counter_ += 1
        msg = Info()
        msg.name = "ROS2"
        msg.age  =  10
        self.publishers_.publish(msg)


def main(args=None):
    rclpy.init(args=args)
    node = MyNode()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == "__main__":
    main()