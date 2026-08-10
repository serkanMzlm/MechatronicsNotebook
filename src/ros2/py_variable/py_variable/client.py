#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from variable.srv import AddThreeInts
from functools import partial

class MyNode(Node):
    def __init__(self):
        super().__init__("py_client_node")
        self.get_logger().info("oop_client:")
        self.call_add_two_ints_server(12, 20, 45)
        self.call_add_two_ints_server(32, 20, 15)


    def call_add_two_ints_server(self, a, b, c):
        client_ = self.create_client(AddThreeInts, "add")
        # Service oluşana kadar bu kısımda bekler
        while not client_.wait_for_service(1.0):
            self.get_logger.warn("Waiting for Server ...")

        request = AddThreeInts.Request()
        request.a = a
        request.b = b
        request.c = c
        # Veriyi gönderir
        future = client_.call_async(request)
        future.add_done_callback(partial(self.addCallback, a=a, b=b, c=c))

    def addCallback(self, future, a, b, c):

        try:
            response = future.result()
            self.get_logger().info(str(a)+"+"+str(b)+str(c)+"="+str(response.sum))

        except Exception as e:
            self.get_logger.error("Server call failed %r" % (e,))


def main(args=None):
    rclpy.init(args=args)
    rclpy.spin(MyNode())
    rclpy.shutdown()


if __name__ == "__main__":
    main()