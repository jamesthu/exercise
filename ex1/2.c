#include <u.h> 

outb(port)	//output
{
  asm(LL,8);
  asm(BOUT);
}

in()	//input
{
  asm(BIN);
  asm(PSHA);
  asm(POPB);
  outb(1);
}

myInterruption()	//interruption
{
  asm(PSHA);
  asm(PSHB);
  asm(PSHC);
  in();
  asm(POPC);
  asm(POPB);
  asm(POPA);
  asm(RTI);
}

ivec(void *isr)
{
  asm(LL,8);
  asm(IVEC);
}

main()
{
  while(1){
    ivec(myInterruption);
    asm(STI);
  }
}
