#ifndef _MINOR_H_
#define _MINOR_H_

#define tVOID	0
#define tINT	1
#define tSTR	2
#define tVEC	3
#define tTYPE(t) (t&3)
#define tCNST	4
#define tPUB	8
#define tFWD	16
#define tFUNC	32
#define TID(x) strNode(ID,x)
#define TINT(x) intNode(INT,x)

#endif /* _MINOR_H_ */
