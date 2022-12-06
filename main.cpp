#include <iostream>

int main() {
  printf("hello");

  chip8 c = chip8();
  c.init();

  return 0;
}

class chip8 {

public:
  void init() {
    // load program into memory
    pc = 0x200;
    ir = 0;
    opcode = 0;

    // clocks
    delay_timer = 60;
    sound_timer = 60;

    sp = 0; // resets the stack pointer

    // fontset needs to be loaded at 0x50 / 80
    for (int i = 0; i < 80; i++) {
      memory[i] = chip8_fontset[i];
    }
  }

  // clock cycle
  void cycle() {
    opcode = memory[pc] << 8 | memory[pc + 1];

    switch (opcode & 0xF000) {
    case 0xA000:
      ir = opcode & 0xF000;
      pc += 2;
      break;
    default:
      printf("No opcodes matched");
    }

    sound_timer--;
    if (sound_timer == 0) {
      printf("beep");
      sound_timer = 60;
    }
  }

private:
  unsigned short opcode; // 65k possible opcodes
  unsigned char V[16];   // Registers V0-V14 and then VE for carry
  unsigned short ir;     // Index Register
  unsigned short pc;     // Program Counter

  // Memory map
  // 0 - 511 / 0x000 - 0x1FF / 512 - reserved for fonts
  // 80 - 160 / 0x050 - 0x0A0 / 80 - default system font
  // 512 - 4095 / 0x200 - 0xFFF /  3583 - Program ROM and work RAM
  // 4096 possible memory locations one byte each
  unsigned char memory[4096];

  // graphics array of 2048 pixels
  unsigned char gfx[2048];

  // counts to 60hz
  unsigned char delay_timer;
  unsigned char sound_timer;

  unsigned short stack[16]; // system stack to remember
  unsigned short sp;        // stack pointer

  // keypad
  unsigned char key[16];

  // the fontset
  unsigned char chip8_fontset[80] = {
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };
};