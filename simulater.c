#include <stdio.h>

#define MEM_NUM 100
#define REG_NUM 16

#define NONE  0
#define LOAD  1
#define STORE 2
#define ADD   3
#define SUB   4
#define MUL   5
#define DIV   6

int Mem[MEM_NUM];
int Reg[REG_NUM];

int pc = 0;

void IFstage(int *ifid_reg) {
  ifid_reg[0] = Mem[pc];
  pc++;
}

void IDstage(int *ifid_reg, int *idex_reg) {
  int decode_result[4];
  int op, rd, rs, rt, mem;

  op = (*ifid_reg >> 12) & 0x0f;
  idex_reg[0] = op;
  switch (idex_reg[0]) {
    case ADD:
    case SUB:
    case MUL:
    case DIV:
      rd = (*ifid_reg >> 8) & 0x0f;
      rs = (*ifid_reg >> 4) & 0x0f;
      rt = (*ifid_reg) & 0x0f;
      idex_reg[1] = Reg[rs];
      idex_reg[2] = Reg[rt];
      idex_reg[3] = rd;
      break;
    case LOAD:
      rd = (*ifid_reg) & 0xff;
      mem = (*ifid_reg >> 8)  & 0x0f;
      idex_reg[1] = mem;
      idex_reg[2] = 0;
      idex_reg[3] = rd;
      break;
    case STORE:
      rt = (*ifid_reg >> 8) & 0x0f;
      mem = (*ifid_reg) & 0xff;
      idex_reg[1] = 0;
      idex_reg[2] = Reg[rt];
      idex_reg[3] = mem;
      break;
    case NONE:
      break;
  }
  printf("0:%2d , 1:%2d , 2:%2d , 3:%2d\n", idex_reg[0], idex_reg[1], idex_reg[2], idex_reg[3]);
}

void EXstage(int *idex_reg, int *exmem_reg) {
  int result;
  switch (idex_reg[0]) {
    case ADD:
      result = idex_reg[1] + idex_reg[2];
      break;
    case SUB:
      result = idex_reg[1] - idex_reg[2];
      break;
    case MUL:
      result = idex_reg[1] * idex_reg[2];
      break;
    case DIV:
      result = idex_reg[1] / idex_reg[2];
      break;
    case LOAD:
    case STORE:
      result = idex_reg[1];
      break;
  }
  exmem_reg[0] = idex_reg[0];
  exmem_reg[1] = result;
  exmem_reg[2] = idex_reg[2];
  exmem_reg[3] = idex_reg[3];
}

void MEMstage(int *exmem_reg, int *memwb_reg) {
  switch (exmem_reg[0]) {
    case LOAD:
      memwb_reg[2] = Mem[exmem_reg[1]];
      break;
    case STORE:
      Mem[exmem_reg[1]] = exmem_reg[2];
      break;
  }
  memwb_reg[0] = exmem_reg[0];
  memwb_reg[1] = exmem_reg[1];
  memwb_reg[3] = exmem_reg[3];
}

void WBstage(int *memwb_reg) {
  switch (memwb_reg[0]) {
    case LOAD:
      Mem[memwb_reg[3]] = memwb_reg[2];
      break;
    case STORE:
      Mem[memwb_reg[3]] = memwb_reg[1];
      break;
  }
}

void dispReg() {
  int i;
  for (i = 0; i < 16; i++) {
    printf("Reg[%2d]:%d\n", i, Reg[i]);
  }
  for (i = 0; i < 32; i++) {
    printf("Mem[%2d]:%d\n", i, Mem[i]);
  }
}

int main(void)
{
  int ifid_reg[1];
  int idex_reg[4];
  int exmem_reg[4];
  int memwb_reg[4];
  int i;
  int mem_count = 1;
  //Q1
  Mem[0] = 12801;
  Reg[0] = 1;
  Reg[1] = 2;

  //Q2
  //Mem[0] = 4126;
  //Mem[30] = 212;

  for (i = 0; i < mem_count; i++) {
    IFstage(ifid_reg);
    IDstage(ifid_reg, idex_reg);
    EXstage(idex_reg, exmem_reg);
    MEMstage(exmem_reg, memwb_reg);
    WBstage(memwb_reg);
  }
  dispReg();

  return 0;
}
