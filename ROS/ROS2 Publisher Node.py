import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from rclpy.qos import QoSProfile


class M_pub(Node):
    def __init__(self):
        super().__init__('massage_publisher')
        self.qos_profile = QoSProfile(depth=10)
        self.massage_publisher = self.create_publisher(
            String, 'massage', self.qos_profile)
        self.timer = self.create_timer(1, self.m_publisher)
        self.count = 0

    def m_publisher(self):
        msg = String()
        msg.data = f'hellow {self.count}'
        self.massage_publisher.publish(msg)
        self.get_logger().info(f'Published mesage: {msg.data}')
        self.count += 1


def main(args=None):
    rclpy.init(args=args)
    node = M_pub()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        node.get_logger().info('Keyboard interrupt!!!!')
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main':
    main()
