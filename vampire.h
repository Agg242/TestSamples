#ifndef VAMPIRE_H__
#define VAMPIRE_H__

#include <exec/types.h>

extern BOOL is_vampire_present(void);

extern void init_vampire_regs(void);


#define AUDF_16BITS  (1 << 0)
#define AUDF_ONESHOT (1 << 1)
#define AUDF_STEREO  (1 << 2)
 

struct VampireCustom {
    UWORD   bltddat;
    UWORD   dmaconr;
    UWORD   vposr;
    UWORD   vhposr;
    UWORD   dskdatr;
    UWORD   joy0dat;
    UWORD   joy1dat;
    UWORD   clxdat;
    UWORD   adkconr;
    UWORD   pot0dat;
    UWORD   pot1dat;
    UWORD   potinp;
    UWORD   serdatr;
    UWORD   dskbytr;
    UWORD   intenar;
    UWORD   intreqr;
    APTR    dskpt;
    UWORD   dsklen;
    UWORD   dskdat;
    UWORD   refptr;
    UWORD   vposw;
    UWORD   vhposw;
    UWORD   copcon;
    UWORD   serdat;
    UWORD   serper;
    UWORD   potgo;
    UWORD   joytest;
    UWORD   strequ;
    UWORD   strvbl;
    UWORD   strhor;
    UWORD   strlong;
    UWORD   bltcon0;
    UWORD   bltcon1;
    UWORD   bltafwm;
    UWORD   bltalwm;
    APTR    bltcpt;
    APTR    bltbpt;
    APTR    bltapt;
    APTR    bltdpt;
    UWORD   bltsize;
    UBYTE   pad2d;
    UBYTE   bltcon0l;	/* low 8 bits of bltcon0, write only */
    UWORD   bltsizv;
    UWORD   bltsizh;	
    UWORD   bltcmod;
    UWORD   bltbmod;
    UWORD   bltamod;
    UWORD   bltdmod;
    UWORD   pad34[4];
    UWORD   bltcdat;
    UWORD   bltbdat;
    UWORD   bltadat;
    UWORD   sprhdat;
    UWORD   bplhdat;
    UWORD   pad3b;
    UWORD   deniseid;	
    UWORD   dsksync;
    ULONG   cop1lc;
    ULONG   cop2lc;
    UWORD   copjmp1;
    UWORD   copjmp2;
    UWORD   copins;
    UWORD   diwstrt;
    UWORD   diwstop;
    UWORD   ddfstrt;
    UWORD   ddfstop;
    UWORD   dmacon;
    UWORD   clxcon;
    UWORD   intena;
    UWORD   intreq;
    UWORD   adkcon;
    struct  AudChannel8 {
      UWORD *ac8_ptr; /* ptr to start of waveform data */
      UWORD ac8_len;	/* length of waveform in words */
      UWORD ac8_per;	/* sample period */
      UWORD ac8_vol;	/* volume */
      UWORD ac8_dat;	/* sample pair */
      UWORD ac8_pad[2];	/* unused */
    } aud8[4];
    APTR    bplpt[8];
    UWORD   bplcon0;
    UWORD   bplcon1;
    UWORD   bplcon2;
    UWORD   bplcon3;
    UWORD   bpl1mod;
    UWORD   bpl2mod;
    UWORD   bplcon4;
    UWORD   clxcon2;
    UWORD   bpldat[8];
    APTR    sprpt[8];
    struct  vSpriteDef {
      UWORD pos;
      UWORD ctl;
      UWORD dataa;
      UWORD datab;
    } spr[8];
    UWORD color[32];
    UWORD htotal;
    UWORD hsstop;
    UWORD hbstrt;
    UWORD hbstop;
    UWORD vtotal;
    UWORD vsstop;
    UWORD vbstrt;
    UWORD vbstop;
    UWORD sprhstrt;
    UWORD sprhstop;
    UWORD bplhstrt;
    UWORD bplhstop;
    UWORD hhposw;
    UWORD hhposr;
    UWORD beamcon0;
    UWORD hsstrt;
    UWORD vsstrt;
    UWORD hcenter;
    UWORD diwhigh;	
    UWORD bplhmod;
    UWORD sprhpth;
    UWORD sprhptl;
    UWORD bplhpth;
    UWORD bplhptl;
    UWORD padf8[2];
    UWORD gfxmode;
    UWORD padfb[3];
    UWORD fmode;
    UWORD padff[2];
    UWORD dmacon2r;
    UWORD pad102[12];
    UWORD intena2r;
    UWORD intreq2r;
    UWORD joybuttons[4];
    UWORD pad114[4];
    UWORD clxdat8[8];
    UWORD pad120[43];
    UWORD dmacon2;
    UWORD pad14c;
    UWORD intena2;
    UWORD intreq2;
    UWORD pad14f[113];
    UWORD planarcolh;
    UWORD planarcoll;
    UWORD spritecolh;
    UWORD spritecoll;
    UWORD chunkycolh;
    UWORD chunkycoll;
    UWORD pipcolh;
    UWORD pipcoll;
    UWORD pad1c8[32];
    UWORD pipxstrt;
    UWORD pipystrt;
    UWORD pipxstop;
    UWORD pipystop;
    APTR  pipptr;
    UWORD pipformat;
    UWORD pipmodulo;
    UWORD pipcolorkey;
    UWORD pipdmarowlen;
    UWORD pad1f2[12];
    UWORD vampireversion;
    UWORD pad1ff;
    struct  vAudChannel16 {
      UWORD *ac16_ptr; /* ptr to start of waveform data */
      ULONG ac16_len;	/* length of waveform in words */
      UWORD ac16_vol;	/* volume */
      UWORD ac16_ctrl;	/* 16bits|one_shot|stereo */
      UWORD ac16_per;	/* sample period */
      UWORD ac16_pad;	/* unused */
    } aud16[8];
};

extern struct VampireCustom *vampire_regs;


#endif
