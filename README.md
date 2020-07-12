# VEngine
A very simple 3D Engine for PS Vita made for my personal hobby/learning purposes.  
Currently is able to load simple .obj models (vertex only).

# Compile
Download the repo with the following and make a build folder
```
git clone https://github.com/Jie-He/VEngine/
cd VEngine/
mkdir build
cd build
```
For OpenCV build:
```
cmake -DOPENCV=1 .. && make
```
For PSVita build
```
cmake -DPSVITA=1 .. && make
```
The default build should display a rotating cube.  
Exectute the build with (may differ on different platforms): 
```
./VEngine
```
# Progress so far
![Image of render](https://people.bath.ac.uk/jh2699/GitImages/VE_Render.png)
