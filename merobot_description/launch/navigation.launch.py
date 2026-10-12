import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource


def generate_launch_description():
    pkg_share = get_package_share_directory('merobot_description')
    nav2_share = get_package_share_directory('nav2_bringup')

    simulation = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_share, 'launch', 'gazebo.launch.py')))

    navigation = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(nav2_share, 'launch', 'bringup_launch.py')),
        launch_arguments={
            'use_sim_time': 'true',
            'use_composition': 'False',
            'map': os.path.join(pkg_share, 'maps', 'room.yaml'),
            'params_file': os.path.join(pkg_share, 'config', 'nav2_params.yaml'),
        }.items())

    return LaunchDescription([
        simulation,
        TimerAction(period=10.0, actions=[navigation]),
    ])
