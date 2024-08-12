# Ray-Marching
## Project Description
This project is an implementation of the ray-marching technique using Open-GL and GLSL.

## Setup
### Dependencies
On a Debian system you can install the needed dependencies using:
```shell
sudo apt install g++ cmake make libglfw3-dev libxxf86vm-dev libxi-dev -y
```

### Clone Repository
```shell
git clone https://github.com/llikian/Ray-Marching.git
cd Ray-Marching
```

### Build
To build you can simply run the `release.sh` script at the root of the project using:
```shell
bash release.sh
```

You can also manually build it using:
```shell
cmake -B build && \
cmake --build build -j
```

Then you can run it using:
```shell
bin/Ray-Marching
```

## Credits
Graphics are handled with [OpenGL](https://www.opengl.org/), using the [GLAD](https://github.com/Dav1dde/glad) implementation.

Window management and event handling are done with [GLFW](https://www.glfw.org/).