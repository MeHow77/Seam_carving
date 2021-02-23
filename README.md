# Project

This repository contains a working CMake based project you may use for writing your solution.

## Prerequisities

All packages necessary to build the project are defined inside the [Dockerfile](Dockerfile "Dockerfile") (Ubuntu 20.04 based).

## Remote debugging in CLion with docker compose
All below listed commands can be used for remote GBD debugging. How to do it is presented at this blog: https://austinmorlan.com/posts/docker_clion_development/

### Docker Container

Building the docker image:
```
cd seam-carving
docker build . -t seamcarving

```

Running the docker container:
```
docker run -v $(pwd):/workdir -it seamcarving bash
```

The above command maps the current working directory from the host OS to the _/workdir_ directory inside the docker container.

## Building

Execute the following steps inside the docker container:
```
cd /workdir
cmake -S src -B build -G Ninja
cmake --build build
```

## Running

The following command runs the built _seam-carving_ binary (executed from the docker container):
```
/workdir/build/seam-carving examples/castle.jpg
```

The process requires path to the input image passed as a mandatory command line argument. The generated output image created by your seam-carving algorithm is saved down to the filesystem in the current runtime directory (directory the docker container has been run from) under _output.png_ name. You may open that file from your host OS file to see the result. 

## Implementation

The `seamCarving` function defined inside the [seam_carving.cpp](src/seam_carving.cpp "seam_carving.cpp") file is an entry point to your implementation.

## Examples

Example images are located inside the [examples/](examples/ "Example input images") directory.

# Results
Original image
![jungle original](https://github.com/MeHow77/Seam_carving/examples/master/jungle.jpg)

Carved 200 pixels in width
![jungle carved](https://github.com/MeHow77/Seam_carving/carved_images/master/carved_jungle.jpg)

Original image
![original castle](https://github.com/MeHow77/Seam_carving/examples/castle/jungle.jpg?raw=true)

Carved 600 pixels in width
![carved castle](https://github.com/MeHow77/Seam_carving/carved_images/master/carved_castle.jpg?raw=true)

Original image
![original museum](https://github.com/MeHow77/Seam_carving/examples/master/museum.jpg?raw=true)

Carved 350 pixels in hight
![carved museum](https://github.com/MeHow77/Seam_carving/carved_images/master/carved_museum.jpg?raw=true)
