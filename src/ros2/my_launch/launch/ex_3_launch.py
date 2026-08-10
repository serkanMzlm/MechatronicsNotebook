import os
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess, IncludeLaunchDescription, SetEnvironmentVariable
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
from os.path import join as Path

"""
Eğer param hiç build edilmeden direkt çalışmasını  istenirse bunu yapmanın iki yolu var
ilk yol `os.path.join` direkt param dosyasının konumunu girmek. İkinci yol ise --symlink-install
ile param dosyasını 1 kez build etmek.
"""
config_file = os.path.join(
        get_package_share_directory('cpp_param'), # Build klasöründe bulunan yml dosyasını aldığı için değişiklik yapılıdğı zaman yml paketide build edilmelidir.
        'config',
        'params.yml'
    )

talker = Node(
            package="cpp_param",                                               # ros_ign_bridge eski versiyonda kullanılır.
            executable="talker_node",
            # ros_arguments=[
                # "--log-level", "talker_node:=debug",
                # "--remap", "talker_node:=my_command_node"
            # ],
            parameters=[config_file],
            output="screen"
          )

def generate_launch_description():
    return LaunchDescription([
      talker
    ]) 