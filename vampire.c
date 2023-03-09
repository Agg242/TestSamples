#include <exec/types.h>
#include <exec/execbase.h>
#include <hardware/custom.h>

extern struct ExecBase *SysBase;
extern volatile struct Custom custom;

struct VampireCustom *vampire_regs;


BOOL is_vampire_present(void)
{
    return ((SysBase->AttnFlags & AFB_FPGA) != 0) ? TRUE : FALSE;
}


void init_vampire_regs(void)
{
    vampire_regs = (struct VampireCustom *)&custom;    
}
