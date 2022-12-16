#include <stdio.h>

int main() { return 0; }

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

    // ---------- FETCH ----------

    opcode = memory[pc] << 8 | memory[pc + 1];
    unsigned char opcode_byte1 = memory[pc];
    unsigned char opcode_byte2 = memory[pc + 1];

    // "Joins" the 2 bytes
    opcode = opcode_byte1 << 8 | opcode_byte2;

    // ---------- DECODE ----------

    // 0x_NNN where _000 is what I get out
    switch (opcode & 0xF000) {
      // OPCODES

    // Calls machine code routine (RCA 1802 for COSMAC VIP) at address NNN. Not
    // necessary for most ROMs
    case 0x0000:
      // 0x00E0 and 0x00EE
      switch (opcode & 0x00FF) {
      case 0x00E0:
        // TODO clears the screen
        break;

      case 0x00EE: // usually ran after 0x2NNN
        sp--;
        pc = stack[sp];
        break;
      }

      break;

    // 0x1NNN - GOTO NNN
    case 0x1000:
      unsigned short nnn = opcode & 0x0FFF;
      pc = nnn;
      break;

    // 0x2NNN - Execute instruction at NNN
    case 0x2000:
      unsigned short nnn = opcode & 0x0FFF;
      stack[sp] = pc; // push current address to the stack
      sp++;
      pc = nnn;
      break;

    // 0x3XNN - Skips the next instruction if VX equals NN (usually the next
    // instruction is a jump to skip a code block)
    case 0x3000:
      unsigned short x = opcode & 0x0F00;
      unsigned short nn = opcode & 0x00FF;
      if (V[x << 8] == nn) {
        pc += 4;
      } else {
        pc += 2;
      }
      break;

    // 4XNN - opposite if 0x3XNN
    case 0x4000:
      unsigned short x = opcode & 0x0F00;
      unsigned short nn = opcode & 0x00FF;
      if (V[x << 8] != nn) {
        pc += 4;
      } else {
        pc += 2;
      }
      break;

    // 5XY0 - Skips the next instruction if VX equals VY (usually the next
    // instruction is a jump to skip a code block)
    case 0x5000:
      unsigned int x = (opcode & 0x0F00) << 8;
      unsigned int y = (opcode & 0x00F0) << 8;
      if (V[x] == V[y]) {
        pc += 4;
      } else {
        pc += 2;
      }
      break;

    // TODO - FINISH THIS
    

    // 0xANNN - Set ir to NNN
    case 0xA000:
      unsigned short nnn = opcode & 0x0FFF;
      ir = nnn;
      pc += 2;
      break;

    // 0xBNNN - Jumps to the address NNN plus V0.
    case 0xB000:
      unsigned short nnn = opcode & 0x0FFF;
      pc = V[0] + nnn;
      break;
    }

    // timers
    if (delay_timer > 0) {
      delay_timer--;
    } else {
      delay_timer = 60;
    }
    if (sound_timer > 0) {
      sound_timer--;
      if (sound_timer == 1) {
        printf("beep");
      }
    } else {
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