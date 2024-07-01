from setuptools import setup
package_name = 'move_turtle_pkg'
setup(
    version='0.0.0',
    data_files=[
            ['resource/' + package_name]),
    ],
    zip_safe=True,
    maintainer_email='tkrwjd119@gmail.com',
    license='TODO: License declaration',
    entry_points={
            'hp = move_turtle_pkg.move_turtle:main',
        ],
)
