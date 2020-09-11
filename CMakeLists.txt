cmake_minimum_required(VERSION 3.10)

project(bsound)

add_compile_definitions(__BUILDING_LIBCSOUND)

add_library(
  bsound SHARED

  src/bsound.h
  src/bsound.c

  src/compat/csoundcompat.h
  src/compat/csoundcompat.c

  src/opcodes/compress.h
  src/opcodes/compress.c

  src/opcodes/reverbsc.h
  src/opcodes/reverbsc.c
)
