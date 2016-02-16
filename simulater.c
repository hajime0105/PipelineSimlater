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
      idex_reg[1] = Reg[rs];  // P1
      idex_reg[2] = Reg[rt];  // P2
      idex_reg[3] = rd;       // 計算結果
      break;
    case LOAD:
      rd = (*ifid_reg) & 0xff;
      mem = (*ifid_reg >> 8)  & 0x0f;
      idex_reg[1] = mem;      // 読み込むメモリアドレス
      idex_reg[2] = 0;
      idex_reg[3] = rd;       // 読み込んだ値を格納するレジスタ
      break;
    case STORE:
      rt = (*ifid_reg >> 8) & 0x0f;
      mem = (*ifid_reg) & 0xff;
      idex_reg[1] = mem;
      idex_reg[2] = Reg[rt];
      idex_reg[3] = 0;
      break;
    case NONE:
      break;
  }
  printf("idex  0:%2d , 1:%2d , 2:%2d , 3:%2d\n", idex_reg[0], idex_reg[1], idex_reg[2], idex_reg[3]);
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
  printf("exmem 0:%2d , 1:%2d , 2:%2d , 3:%2d\n", exmem_reg[0], exmem_reg[1], exmem_reg[2], exmem_reg[3]);
}

void MEMstage(int *exmem_reg, int *memwb_reg) {
  switch (exmem_reg[0]) {
    case LOAD:
      memwb_reg[2] = Mem[exmem_reg[3]];
      break;
    case STORE:
      Mem[exmem_reg[1]] = exmem_reg[2];
      break;
  }
  memwb_reg[0] = exmem_reg[0];
  memwb_reg[1] = exmem_reg[1];
  memwb_reg[3] = exmem_reg[3];
  printf("0:%2d , 1:%2d , 2:%2d , 3:%2d\n", memwb_reg[0], memwb_reg[1], memwb_reg[2], memwb_reg[3]);
}

void WBstage(int *memwb_reg) {
  switch (memwb_reg[0]) {
    case ADD:
    case SUB:
    case MUL:
    case DIV:
      Reg[memwb_reg[3]] = memwb_reg[1];
      break;
    case LOAD:
      Reg[memwb_reg[3]] = memwb_reg[2];
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
  int mem_count = 5;  //命令数
  int i;

  //Q1
  //Mem[0] = 12801;
  //Reg[0] = 1;
  //Reg[1] = 2;

  //Q2
  //Mem[0] = 4126;
  //Mem[30] = 212;

  //Q3
  Mem[0] = 14593;
  Mem[1] = 23186;
  Mem[2] = 27555;
  Mem[3] = 19636;
  Mem[4] = 0x150a;
  Mem[5] = 0x2008;
  Reg[0] = 17;
  Reg[1] = 1;
  Reg[2] = 3;
  Reg[3] = 2;
  Reg[4] = 23;

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
