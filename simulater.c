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

void change_idex(int ifid_reg, int *idex_reg) {
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
      idex_reg[j] = data[i];
    }
  }
  else {
    for (i = 0; i < 4; i++) {
      idex_reg[i] = ifid_reg;
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
void alu_load(int *idex_reg, int *exmem_reg, int *Mem, int *Reg) {
  exmem_reg[1] = Mem[idex_reg[1]];
  Reg[idex_reg[2]] = exmem_reg[1];
}

void alu_store(int *idex_reg, int *exmem_reg, int *Mem, int *Reg) {
  exmem_reg[1] = Reg[idex_reg[1]];
  Mem[idex_reg[2]] = exmem_reg[1];
}
*/
void alu_add(int *idex_reg, int *exmem_reg, int *Reg) {
  exmem_reg[1] = Reg[idex_reg[1]] + Reg[idex_reg[2]];
  Reg[idex_reg[3]] = exmem_reg[1];
}

void alu_sub(int *idex_reg, int *exmem_reg, int *Reg) {
  exmem_reg[1] = Reg[idex_reg[1]] - Reg[idex_reg[2]];
  Reg[idex_reg[3]] = exmem_reg[1];
}

void alu_mul(int *idex_reg, int *exmem_reg, int *Reg) {
  exmem_reg[1] = Reg[idex_reg[1]] * Reg[idex_reg[2]];
  Reg[idex_reg[3]] = exmem_reg[1];
}

void alu_div(int *idex_reg, int *exmem_reg, int *Reg) {
  exmem_reg[1] = Reg[idex_reg[1]] / Reg[idex_reg[2]];
  Reg[idex_reg[3]] = exmem_reg[1];
}

void calc_exmem(int *idex_reg, int *exmem_reg, int *Mem, int *Reg) {
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
      alu_add(idex_reg, exmem_reg, Reg);
      break;
    case SUB:
      alu_sub(idex_reg, exmem_reg, Reg);
      break;
    case MUL:
      alu_mul(idex_reg, exmem_reg, Reg);
      break;
    case DIV:
      alu_div(idex_reg, exmem_reg, Reg);
      break;
  }
}

void result_memwb(int *exmem_reg, int *memwb_reg) {
  int i;
  for (i = 0; i < 3; i++) {
      exmem_reg[i] = memwb_reg[i];
  }
}

void dispReg(int Reg[]) {
  int i;
  for (i = 0; i < 16; i++) {
    printf("Reg[%2d]:%d ", i, Reg[i]);
    if (i % 4 == 3) {
      printf("\n");
    }
  }
}

int main(void)
{
  int Mem[MEM_NUM];
  int Reg[REG_NUM];
  int ifid_reg[1];
  int idex_reg[4];
  int exmem_reg[3];
  int memwb_reg[3];
  int i;


  //Q1
  Mem[0] = 12801;
  Reg[0] = 1;
  Reg[1] = 2;

  /*
  //Q2
  Mem[0] = 4126;
  Mem[30] = 212;
*/
  for (i = 0; i < MEM_NUM; i++) {
    ifid_reg[0] = Mem[i];
    change_idex(ifid_reg[0], idex_reg);
    replace_idex(idex_reg);
    calc_exmem(idex_reg, exmem_reg, Mem, Reg);
    result_memwb(exmem_reg, memwb_reg);
  }

  dispReg(Reg);

  return 0;
}
