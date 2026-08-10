from setuptools import find_packages, setup

package_name = 'py_service'

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
            "my_server_node     = py_service.server:main",
            "my_client_node     = py_service.client:main",
            "my_client_oop_node = py_service.oop_client:main"
        ],
    },
)
