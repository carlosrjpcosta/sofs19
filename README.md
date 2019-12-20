# sofs19
Projeto da cadeira de Sistemas de Operação

# Prerequisites
On Ubuntu you need the following packages installed:
build-essential, cmake, doxygen, libfuse-dev, and git.

sudo apt install build-essential cmake doxygen libfuse-dev git

In other Linux distributions you need equivalent packages installed.

# Compiling the code
In a terminal, enter the build directory of your project

cd XXXXX/sofs19/build

Then compile the code


cmake ../src

make


If you prefer ninja, instead of make

cmake -G Ninja ../src

ninja

# Generating documentation
The code is documented in doxygen. So, you can easily generate html documentation pages.

cd XXXXX/sofs19/doc

doxygen

firefox html/index.html &

Of course, you can change firefox by your favourite browser.

# testing the code
The following sequence of commands, where XXXXX is your project's id, allows you to mount a sofs file system

cd XXXXX/sofs19/bin

./createDisk /tmp/dsk 1000      (/tmp/dsk will be a disk with 1000 blocks)

./mksofs /tmp/dsk               (format the disk as a sofs19 file system)

mkdir /tmp/mnt                  (our mount point)

./sofsmount /tmp/dsk /tmp/mnt   (mount the disk in the mount point)


Now, everything created inside the mount point will be stored in disk (the /tmp/dsk file). You can use the showblock tool to check that out.
