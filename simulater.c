#include <stdio.h>

#define MEM_NUM 100
#define REG_NUM 16

#define NONE 0
#define LOAD 1
#define STORE 2
#define ADD 3
#define SUB 4
#define MUL 5
#define DIV 6

int Mem[MEM_NUM];
int Reg[REG_NUM];

void decode(int ifid_reg, int *decode_result) {
  int data[10];
  int index_num = 0;
  int i;

  if (ifid_reg != 0) {
    for(i = 0; i< 4; i++){
      data[i] = ifid_reg % 16 ;
      if(data[i] != 0){
        index_num = i;
      }
      ifid_reg /= 16;
    }
    int j = 0;
    for(i = index_num; i >= 0 ; i--, j++){
      decode_result[j] = data[i];
    }
  }
  else {
    for (i = 0; i < 4; i++) {
      decode_result[i] = ifid_reg;
    }
  }
}

void replace_idex(int *idex_reg) {
  int tmp;

  tmp = idex_reg[1];
  idex_reg[1] = idex_reg[3];
  idex_reg[3] = tmp;

  tmp = idex_reg[1];
  idex_reg[1] = idex_reg[2];
  idex_reg[2] = tmp;
}
/*
void alu_load(int *idex_reg, int *exmem_reg) {
  exmem_reg[1] = Mem[idex_reg[1]];
  Reg[idex_reg[2]] = exmem_reg[1];
}

void alu_store(int *idex_reg, int *exmem_reg) {
  exmem_reg[1] = Reg[idex_reg[1]];
  Mem[idex_reg[2]] = exmem_reg[1];
}
*/
void alu_add(int *idex_reg, int *exmem_reg) {
  exmem_reg[1] = Reg[idex_reg[1]] + Reg[idex_reg[2]];
  Reg[idex_reg[3]] = exmem_reg[1];
}

void alu_sub(int *idex_reg, int *exmem_reg) {
  exmem_reg[1] = Reg[idex_reg[1]] - Reg[idex_reg[2]];
  Reg[idex_reg[3]] = exmem_reg[1];
}

void alu_mul(int *idex_reg, int *exmem_reg) {
  exmem_reg[1] = Reg[idex_reg[1]] * Reg[idex_reg[2]];
  Reg[idex_reg[3]] = exmem_reg[1];
}

void alu_div(int *idex_reg, int *exmem_reg) {
  exmem_reg[1] = Reg[idex_reg[1]] / Reg[idex_reg[2]];
  Reg[idex_reg[3]] = exmem_reg[1];
}

void calc_exmem(int *idex_reg, int *exmem_reg) {
  exmem_reg[0] = idex_reg[0];
  exmem_reg[2] = idex_reg[3];
  switch (idex_reg[0]) {
    case NONE:
      break;
    case LOAD:
      //alu_load(idex_reg, exmem_reg, Mem, Reg);
      break;
    case STORE:
      //alu_store(idex_reg, exmem_reg, Mem, Reg);
      break;
    case ADD:
      alu_add(idex_reg, exmem_reg);
      break;
    case SUB:
      alu_sub(idex_reg, exmem_reg);
      break;
    case MUL:
      alu_mul(idex_reg, exmem_reg);
      break;
    case DIV:
      alu_div(idex_reg, exmem_reg);
      break;
  }
}

void result_memwb(int *exmem_reg, int *memwb_reg) {
  int i;
  for (i = 0; i < 3; i++) {
      exmem_reg[i] = memwb_reg[i];
  }
}

void dispReg() {
  int i;
  for (i = 0; i < 16; i++) {
    printf("Reg[%2d]:%d ", i, Reg[i]);
    if (i % 4 == 3) {
      printf("\n");
    }
  }
}

void IFstage(int *ifid_reg, int *pc) {
  ifid_reg[0] = Mem[pc];
  pc++;
  //命令終了なら実効ループをぬける
}

void IDstage(int ifid_reg, int *idex_reg) {
  int decode_result[4];
  decode(ifid_reg, decode_result);
  decode_result[0];
}

void EXstage() {
}

void MEMstage() {
}

void WBstage() {
}


int main(void)
{
  int ifid_reg[1];
  int idex_reg[4];
  int exmem_reg[4];
  int memwb_reg[4];
  int i;
  int pc;
  itn flag;


  //Q1
  Mem[0] = 12801;
  Reg[0] = 1;
  Reg[1] = 2;

  /*
  //Q2
  Mem[0] = 4126;
  Mem[30] = 212;
*/
  flag = 1;
  pc = 0;
  while (flag != 0) {
    IFstage(ifid_reg, &pc);
    IDstage();
    EXstage();
    MEMstage();
    WBstage();
  }
  dispReg();

  return 0;
}
