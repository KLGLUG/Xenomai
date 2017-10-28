

apt-get install build-essential gcc libncurses5-dev libssl-dev #1.First you need the build-essential package 2.libncurses5-dev package will make it easier to configure
 
sudo make localmodconfig #  This command detects currently running kernel components and marks them for compilation.

sudo make nconfig # This command is used to choose what to compile. Here we make changes and generate a .config file

sudo make -j$(nproc --all) # This command is used to compile the kernel."$" represents number of processors

sudo make modules_install # This command is used to finish compiling the rest of the kernel

sudo make install # This will automatically copy the kernel to your /boot folder and generate the appropriate files to make it work.It updates the grub menu with our compiled kernel.










