# For Players
(Devs, fill this out when the time comes)

# For Devs
## What is this
This is a clone of the famous Chrome Dino Game written in C using SDL2.
It's built using the GNU Autotools for \*nix platforms and the Windows build system is currently TBD (but will probably just be either NMake or a VS Solution)
This project also aims to be as portable as possible.
Windows, Mac, Linux, and whatever other platforms we want to try and challenge ourselves with porting to.

## Questions About This Project I Expect To Get
### Why aren't we building with CMake?
Because CMake is *CRINGE*.

- It's slow as balls.
- Generated Makefiles require CMake to be installed to run, whereas autotools configure scripts and Makefiles do not require autotools to be installed, it is just required to create the scripts.
- The scripting language sucks
### Do we really have to make it portable? Can't we just target windows?
Yes, Windows, Mac, and Linux.
It will be a good learning experience.
### How do I build this?
#### \*nix Systems
`./configure`
Pay attention to make sure your compiler isn't being silly and putting in extra CFLAGS even though you literally never asked it to anywhere. (This happens on my desktop).
If it does, do this.
``export CFLAGS="" ./configure``
You can also put any extra CFLAGS in the quotes if you want any.

If you want to enable debug builds, add ``--enable-debug=yes`` to the end of the command.

``make``

Add ``-j<num>`` to build with multiple threads

If you want to install, simply ``sudo make install``
#### Windows
TBD
