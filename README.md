# README #

### Dependencies ###

 * PointClouds (PCL)  [Download Pointclouds](http://pointclouds.org/downloads/)
 * Jsoncpp [How to install on unix](http://ask.xmodulo.com/fix-fatal-error-jsoncpp.html)
 * OpenCV [OpenCV:Installation in Linux](http://docs.opencv.org/3.1.0/d7/d9f/tutorial_linux_install.html)
 * Addition modules for OpenCV [OpenCV modules](https://github.com/Itseez/opencv_contrib)
 * We are using [Igloo](http://igloo-testing.org/) for the Tests.
 * And [Doxygen](http://www.stack.nl/~dimitri/doxygen/download.html) for the documentation.

This project use [CMake](http://www.cmake.org) (version 2.6 Minimum) to compile.  
We use g++ compiler. **The version of the compiler is not defined yet!**  
The system used to run this program is Linux (i.e. debian, ubuntu...)
You'll find a complete documentation of [Jsoncpp here](http://open-source-parsers.github.io/jsoncpp-docs/doxygen/index.html)

### What is this repository for? ###

* This repository contain the code used to run different robots that are supposed  
to smartly scan the environment around him.
* Current Version is [0.1]

### How to compile ###

    cd racine_folder_of_the_project
    cmake .
    make

* After the first compilation you just have to type `make` to compile
* You can always delete all cache files and recompile all the project by typing `make re`
* You may have to download and install PointCloud library if not done already
* You may have to download and install JsonCPP (libjsoncpp) library if not done already. Via repository or at https://github.com/open-source-parsers/jsoncpp

### How to run Tests ###

    cd racine_of_the_project/test
    cmake . #for the first time only
    make
    ./test

### How to generate documenation ###

To generate documentation use `doxygen documentation/docConfig.txt`.
You can find doxygen [here](http://www.stack.nl/~dimitri/doxygen/download.html)
The documenation will be generated in the `documentation` folder. You can view the html version of it by opening the file `documentation/html/index.html` in your web browser

### Available Makefile command ###

* `make`: compile project
* `make re`: delete cache and recompile project
* `make clean`: delete project executable
* `make fclean`: delete project executable and all cache files (after this command you'll have to compile as if for the first time)

### Contribution guidelines ###

* Tests are mostly written by Nicolas Thing-leoh
* The code review are currently not processed

### Who do I talk to? ###

* **Robots Functionalities**: Maxime Croizer (*maxime.croizer@epitech.eu*), Maxence Frère (*maxence.frere@epitech.eu*)
* **SLAM**: Nicolas Thing-leoh (*nicolas1.thing-leoh@epitech.eu*), Martin Fouks (*martin.fouks@epitech.eu*)

### More Informations ###

* [Official Documentation for PointClouds](http://pointclouds.org/documentation/tutorials/)
* [Documentation PDF for PointClouds](http://www.cse.buffalo.edu/~jryde/cse673/files/pcl_tutorial.pdf)

### FAQ ###

    CMake Error at CMakeLists.txt:34 (FIND_PACKAGE):
      By not providing "FindPCL.cmake" in CMAKE_MODULE_PATH this project has
      asked CMake to find a package configuration file provided by "PCL", but
      CMake did not find one.
      Could not find a package configuration file provided by "PCL" (requested
      version 1.7) with any of the following names:
        PCLConfig.cmake
        pcl-config.cmake
      Add the installation prefix of "PCL" to CMAKE_PREFIX_PATH or set "PCL_DIR"
      to a directory containing one of the above files.  If "PCL" provides a
      separate development package or SDK, be sure it has been installed.
    -- Configuring incomplete, errors occurred!

* If you got this error message, it means you don't have corretly intalled PointClouds.