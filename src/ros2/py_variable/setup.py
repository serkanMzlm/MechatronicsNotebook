from setuptools import find_packages, setup

package_name = 'py_variable'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='serkan',
    maintainer_email='serkanmazlum306@gmail.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            "info_node = py_variable.topic:main",
            "add_three_int_client = py_variable.client:main",
            "add_three_int_server = py_variable.server:main"
        ],
    },
)
