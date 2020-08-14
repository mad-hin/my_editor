# my_editor
This is an editor that I follow the tutorial on [https://viewsourcecode.org/snaptoken/kilo/](https://viewsourcecode.org/snaptoken/kilo/)\
Maybe in the future I will add some my own function or change it.
## Compiling
This project is written in C, therefore you need a C compiler to compile it
### Windows
1. This project requires **Linux environment**, in order to compile it you may install :
  - Windows Subsystem for Linux (WSL)
    - Install WSL
      1. Enable the "Windows Subsystem for Linux" optional feature by open **PowerShell** as **Administrator** and run:\
      ```dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart```
      2. Install your Linux distribution of choice
        - Go to [Microsoft Store](https://aka.ms/wslstore)
        - type **"Linux"**
        - Choose one to install (i.e. Ubuntu 20.04 LTS)
      3. Set up your distribution
      4. Install ```gcc``` and ```make```
        - type: ```sudo apt-get install gcc make```
      5. Clone this repository via Git: ```git clone https://github.com/mad-hin/my_editor.git```
      6. cd to the directory that you installed (i.e. ```cd editor```)
      7. type ```make``` to compile it
  - Cygwin
    - Install Cygwin
      1. Download the installer from [cygwin.com/install.html](cygwin.com/install.html)
      2. During install **Remember** select the ```gcc-core``` and ```make``` packages which are in ```devel``` category
      4. Set Environment Variables 
      5. Open Cygwin terminal program\
      (If it set up is correct, when you type ```gcc --version``` it will show you the version of gcc that you installed)
      6. Clone this repository via Git: ```git clone https://github.com/mad-hin/my_editor.git```
      7. cd to the directory that you installed (i.e. ```cd editor```)
      8. type ```make``` to compile it
 - Note that ```MinGW```, ```MinGW-w64``` or``` Visual C++``` **cannot** compile this project
### macOS
 - The simplest way: install ```Xcode```
 - Clone this repository via Git: ```git clone https://github.com/mad-hin/my_editor.git```
  - cd to the directory that you installed (i.e. ```cd editor```)
  - type ```make``` to compile it
### Linux 
  - install ```gcc``` and ```make``` by type the command: ```sudo apt-get install gcc make```\
   (note that this command may not work for some Linux distro ```i.e. Manjaro Linux```)
   - Clone this repository via Git: ```git clone https://github.com/mad-hin/my_editor.git```
  - cd to the directory that you installed (i.e. ```cd editor```)
  - type ```make``` to compile it
## Run editor
  1. type ```make run``` to compile and run the program
## Remove editor
  1. type ```make clean``` to remove
## Copyright of the orginal work
As I said in beginning, this editor is following the tutorial on [https://viewsourcecode.org/snaptoken/kilo/](https://viewsourcecode.org/snaptoken/kilo/) which was [originally written](https://github.com/antirez/kilo) by [Salvatore Sanfilippo](https://github.com/antirez)  aka antirez and is released under the BSD 2 clause license (see file ```Source LICENSE```) and being [modified](https://github.com/snaptoken/kilo-tutorial) by [snaptoken](https://github.com/snaptoken) under [CC BY 4.0](https://creativecommons.org/licenses/by/4.0/)
## License
For the part that I modified it is under BSD-2-Clause License [tl;dr](https://tldrlegal.com/license/bsd-2-clause-license-(freebsd))
```
BSD 2-Clause License

Copyright (c) 2020, Marco Tam
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```