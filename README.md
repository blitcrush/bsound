# bsound

bsound is a collection of Csound opcodes, separated from the rest of the Csound codebase and organized in a format that can be easily compiled into a dynamic library. We're currently lacking in-depth documentation for bsound, because it's very similar to Csound, but if you're interested in using it and need more detail, please file an issue on this repo and we'd be happy to help you out! 

## Building

bsound has one configuration format and that is a dynamic library - just build it like any other CMake project and you should be good to go!

## Usage

Use the `bsound_*_init` functions to initialize an opcode, `bsound_*_perf` to execute its functionality, and `bsound_*_destroy` to destroy the opcode. After initializing and opcode's struct, you can directly modify its properties as appropriate. Nevertheless, it's advisable to only modify the same properties as would be exposed to a Csound application using the opcode, and not any of the opcode's internal state. Some of the variable names can be difficult to understand, but thankfully [the Csound reference manual](https://csound.com/docs/manual/PartReference.html) is very helpful in this regard.

## Licensing

Just like Csound, bsound is licensed under the LGPL 2.1 - full details are in the LICENSE file. Much of the code in this repository keeps copyright notices from the original files from which they were derived. Nevertheless, they are modified version.
