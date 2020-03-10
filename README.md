# eigen-quadprog

[![License](https://img.shields.io/github/license/jrl-umi3218/eigen-quadprog)](https://opensource.org/licenses/LGPL-3.0)
[![Documentation](https://img.shields.io/badge/doxygen-online-brightgreen?logo=read-the-docs&style=flat)](http://jrl-umi3218.github.io/eigen-quadprog/doxygen/HEAD/index.html)

eigen-quadprog allow to use the QuadProg QP solver with the Eigen3 library.

This package has been forked from [jrl-umi3218](https://github.com/jrl-umi3218/eigen-quadprog). The following changes were made:

- Catkin packaging for ROS
- Removed F2C option (slower) and Python bindings
- Unit tests converted from Boost to Google Test

## Installation

### Install Debian Packages

To install all packages from the this repository as Debian packages use

```
sudo apt install ros-${ROS_DISTRO}-eigen-quadprog
```

### Building from Source

#### Dependencies

- [CMake](cmake.org) >= 3.5
- [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) >= 3.2
- [Robot Operating System (ROS)](http://wiki.ros.org)
- [g++](https://gcc.gnu.org/)
- [gfortran](https://gcc.gnu.org/fortran/)

#### Building

To build from source, clone the latest version from this repository into your catkin workspace and compile the package using

```
cd ~/catkin_ws/src
git clone git@github.com:ANYbotics/eigen-quadprog.git
catkin build eigen-quadprog
```

### Unit Tests

Run the unit tests with

```
catkin run_tests eigen-quadprog
```
