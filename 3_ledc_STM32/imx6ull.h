
#define CCM_BASE    (0x020C4000)       /* CCM外设基地址 */

typedef struct
{
    volatile unsigned int CCR;
    volatile unsigned int CCDR;
    volatile unsigned int CSR;
    volatile unsigned int CCSR;
    volatile unsigned int CACRR;
    volatile unsigned int CBCDR;
    volatile unsigned int CSCMR1;
    volatile unsigned int CSCMR2;
    volatile unsigned int CSCDR1;
    volatile unsigned int CSCDR2;
    volatile unsigned int CDCDR;
    volatile unsigned int CHSCCDR;
    volatile unsigned int CSCDR2;
    volatile unsigned int CSCDR3;
    volatile unsigned int RESERVED_1[2];
    volatile unsigned int CDHIPR;
    volatile unsigned int RESERVED_2[2];
    volatile unsigned int CLPCR;
    volatile unsigned int CISR;
    volatile unsigned int CIMR;
    volatile unsigned int CCOSR;
    volatile unsigned int CGPR;
    volatile unsigned int CCGR0;
    volatile unsigned int CCGR1;
    volatile unsigned int CCGR2;
    volatile unsigned int CCGR3;
    volatile unsigned int CCGR4;
    volatile unsigned int CCGR5;
    volatile unsigned int CCGR6;
    volatile unsigned int RESERVED_3[1];
    volatile unsigned int CMEOR;

}CCCM_Type;

#define CCM         ((CCM_Type *) CCM_BASE)