# phixel
phixel is a small snippet of c++ opencv code to help you create a pixelized picture
 composed with other pictures by yourself.

 The running result is just like below:

 original picture:

 ![original picture](https://github.com/lawlietxxl/phixel/blob/master/test_pics/test.jpg)

 gray mode:

 ![gray mode](https://github.com/lawlietxxl/phixel/blob/master/test_pics/result_g.jpg)

 color mode:

 ![color mode](https://github.com/lawlietxxl/phixel/blob/master/test_pics/result_c.jpg)

## easy to use
### prepare
Because this file is written with c++ opencv. It is a must to have the opencv c++ 
installation on your computer. You can learn about installing opencv from [here](http://docs.opencv.org/3.1.0/d7/d9f/tutorial_linux_install.html#gsc.tab=0).

And I chose to use opencv with gcc and Cmake. You can learn about it from [here](http://docs.opencv.org/3.1.0/db/df5/tutorial_linux_gcc_cmake.html#gsc.tab=0).

Test environment:

+ ubuntu 16.04
+ c++ opencv 3.10

### let's phixel
To get the code and test running:

NOTE: For the image file folder may be a little bit large, if it is a worry, just download the txt and cpp files. :)


```bash
git clone https://github.com/lawlietxxl/phixel
cd phixel
cmake .
make
./phixel
```

If you can see "The file has been saved to" somewhere after some outputs, congratulations, you are ready to use it.

Get into the phixel.cpp file, you can see 2 variables defined before the real code starts.

```c++
>phixel.cpp

#define PIXEL_LENGTH 20
#define SINGLE_PIC_LENGTH 20
```

**PIXEL_LENGTH** means the length when pixelizing the picture like what the picture shows.

**SINGLE_PIC_LENGTH** means the single picture length when filling the pixelized picture.
Every single picture will take one pixel of the pixelized picture.

![intro](https://github.com/lawlietxxl/phixel/blob/master/test_pics/intro.png)

NOTE: the code will cut the filling pictures into square.

Change the 2 variables as you like. But take care of the memory, cause if you change the **SINGLE_PIC_LENGTH** 
to a very large number, the memory might be out of use.

Then remake the project.

```bash
make
```

### how to use
It's very easy to use it.

```bash
cd /the/project/path
./phixel <imageName> <folderPath> <writePath> <mode: gray/color>
```

+ ImageName: the picture you want to phixelized.
+ folderPath: the candidates waiting to be filled into the new picture.(NOTE: Please don't let the total size be too large.)
+ writePath: the new picture's path/name.
+ mode: just type in gray or color. Nothing more!

Please enjoy!


NOTE: The test file comes from the web and the candidates come from [ImageNet](http://www.image-net.org/).

# LICENSE
```
Copyright 2016 lawlietxxl

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```