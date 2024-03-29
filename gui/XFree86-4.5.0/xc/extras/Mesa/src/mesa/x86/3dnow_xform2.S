/* $Id: 3dnow_xform2.S,v 1.2 2004/04/26 10:10:25 alanh Exp $ */
/* $XFree86: xc/extras/Mesa/src/mesa/x86/3dnow_xform2.S,v 1.3 2004/12/10 15:30:13 alanh Exp $ */

/*
 * Mesa 3-D graphics library
 * Version:  3.5
 *
 * Copyright (C) 1999-2001  Brian Paul   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef USE_3DNOW_ASM
#include "matypes.h"
#include "xform_args.h"

    SEG_TEXT

#define FRAME_OFFSET	4


ALIGNTEXT16
GLOBL GLNAME( _mesa_3dnow_transform_points2_general )
GLNAME( _mesa_3dnow_transform_points2_general ):

    PUSH_L    ( ESI )

    MOV_L     ( ARG_DEST, ECX )
    MOV_L     ( ARG_MATRIX, ESI )
    MOV_L     ( ARG_SOURCE, EAX )
    MOV_L     ( CONST(4), REGOFF(V4F_SIZE, ECX) )
    OR_B      ( CONST(VEC_SIZE_4), REGOFF(V4F_FLAGS, ECX) )
    MOV_L     ( REGOFF(V4F_COUNT, EAX), EDX )
    MOV_L     ( EDX, REGOFF(V4F_COUNT, ECX) )

    PUSH_L    ( EDI )

    MOV_L     ( REGOFF(V4F_START, ECX), EDX )
    MOV_L     ( ESI, ECX )
    MOV_L     ( REGOFF(V4F_COUNT, EAX), ESI )
    MOV_L     ( REGOFF(V4F_STRIDE, EAX), EDI )
    MOV_L     ( REGOFF(V4F_START, EAX), EAX )

    TEST_L    ( ESI, ESI )
    JZ        ( LLBL( G3TPGR_3 ) )

    MOVD      ( REGIND(ECX), MM0 )	/*                 | m00             */
    PUNPCKLDQ ( REGOFF(16, ECX), MM0 )	/* m10             | m00             */

    MOVD      ( REGOFF(4, ECX), MM1 )	/*                 | m01             */
    PUNPCKLDQ ( REGOFF(20, ECX), MM1 )	/* m11             | m01             */

    MOVD      ( REGOFF(8, ECX), MM2 )	/*                 | m02             */
    PUNPCKLDQ ( REGOFF(24, ECX), MM2 )	/* m12             | m02             */

    MOVD      ( REGOFF(12, ECX), MM3 )	/*                 | m03             */
    PUNPCKLDQ ( REGOFF(28, ECX), MM3 )	/* m13             | m03             */

    MOVQ      ( REGOFF(48, ECX), MM4 )	/* m31             | m30             */
    MOVQ      ( REGOFF(56, ECX), MM5 )	/* m33             | m32             */

ALIGNTEXT16
LLBL( G3TPGR_2 ):

    MOVQ      ( REGIND(EAX), MM6 )	/* x1              | x0              */
    MOVQ      ( MM6, MM7 )		/* x1              | x0              */

    PFMUL     ( MM0, MM6 )		/* x1*m10          | x0*m00          */
    PFMUL     ( MM1, MM7 )		/* x1*m11          | x0*m01          */

    PFACC     ( MM7, MM6 )		/* x0*m01+x1*m11   | x0*x00+x1*m10   */
    PFADD     ( MM4, MM6 )		/* x0*...*m11+m31  | x0*...*m10+m30  */

    MOVQ      ( MM6, REGIND(EDX) )	/* write r1, r0                      */
    MOVQ      ( REGIND(EAX), MM6 )	/* x1              | x0              */

    MOVQ      ( MM6, MM7 )		/* x1              | x0              */
    PFMUL     ( MM2, MM6 )		/* x1*m12          | x0*m02          */

    PFMUL     ( MM3, MM7 )		/* x1*m13          | x0*m03          */
    ADD_L     ( EDI, EAX )		/* next vertex                       */

    PFACC     ( MM7, MM6 )		/* x0*m03+x1*m13   | x0*x02+x1*m12   */
    PFADD     ( MM5, MM6 )		/* x0*...*m13+m33  | x0*...*m12+m32  */

    MOVQ      ( MM6, REGOFF(8, EDX) )	/* write r3, r2                      */
    ADD_L     ( CONST(16), EDX )	/* next r                            */

    DEC_L     ( ESI )			/* decrement vertex counter          */
    JNZ       ( LLBL( G3TPGR_2 ) )	/* cnt > 0 ? -> process next vertex  */

LLBL( G3TPGR_3 ):

    FEMMS
    POP_L     ( EDI )
    POP_L     ( ESI )
    RET




ALIGNTEXT16
GLOBL GLNAME( _mesa_3dnow_transform_points2_perspective )
GLNAME( _mesa_3dnow_transform_points2_perspective ):

    PUSH_L    ( ESI )

    MOV_L     ( ARG_DEST, ECX )
    MOV_L     ( ARG_MATRIX, ESI )
    MOV_L     ( ARG_SOURCE, EAX )
    MOV_L     ( CONST(4), REGOFF(V4F_SIZE, ECX) )
    OR_B      ( CONST(VEC_SIZE_4), REGOFF(V4F_FLAGS, ECX) )
    MOV_L     ( REGOFF(V4F_COUNT, EAX), EDX )
    MOV_L     ( EDX, REGOFF(V4F_COUNT, ECX) )

    PUSH_L    ( EDI )

    MOV_L     ( REGOFF(V4F_START, ECX), EDX )
    MOV_L     ( ESI, ECX )
    MOV_L     ( REGOFF(V4F_COUNT, EAX), ESI )
    MOV_L     ( REGOFF(V4F_STRIDE, EAX), EDI )
    MOV_L     ( REGOFF(V4F_START, EAX), EAX )

    TEST_L    ( ESI, ESI )
    JZ        ( LLBL( G3TPPR_3 ) )

    MOVD      ( REGIND(ECX), MM0 )	/*                 | m00             */
    PUNPCKLDQ ( REGOFF(20, ECX), MM0 )	/* m11             | m00             */

    MOVD      ( REGOFF(56, ECX), MM3 )	/*                 | m32             */

ALIGNTEXT16
LLBL( G3TPPR_2 ):

    MOVQ      ( REGIND(EAX), MM4 )	/* x1              | x0              */
    PFMUL     ( MM0, MM4 )		/* x1*m11          | x0*m00          */

    MOVQ      ( MM4, REGIND(EDX) )	/* write r1, r0                      */
    MOVQ      ( MM3, REGOFF(8, EDX) )	/* write r2  (=m32), r3 (=0)         */

    ADD_L     ( EDI, EAX )		/* next vertex                       */
    ADD_L     ( CONST(16), EDX )	/* next r                            */

    DEC_L     ( ESI )			/* decrement vertex counter          */
    JNZ       ( LLBL( G3TPPR_2 ) )	/* cnt > 0 ? -> process next vertex  */

LLBL( G3TPPR_3 ):

    FEMMS
    POP_L     ( EDI )
    POP_L     ( ESI )
    RET




ALIGNTEXT16
GLOBL GLNAME( _mesa_3dnow_transform_points2_3d )
GLNAME( _mesa_3dnow_transform_points2_3d ):

    PUSH_L    ( ESI )

    MOV_L     ( ARG_DEST, ECX )
    MOV_L     ( ARG_MATRIX, ESI )
    MOV_L     ( ARG_SOURCE, EAX )
    MOV_L     ( CONST(3), REGOFF(V4F_SIZE, ECX) )
    OR_B      ( CONST(VEC_SIZE_3 ), REGOFF(V4F_FLAGS, ECX) )
    MOV_L     ( REGOFF(V4F_COUNT, EAX), EDX )
    MOV_L     ( EDX, REGOFF(V4F_COUNT, ECX) )

    PUSH_L    ( EDI )

    MOV_L     ( REGOFF(V4F_START, ECX), EDX )
    MOV_L     ( ESI, ECX )
    MOV_L     ( REGOFF(V4F_COUNT, EAX), ESI )
    MOV_L     ( REGOFF(V4F_STRIDE, EAX), EDI )
    MOV_L     ( REGOFF(V4F_START, EAX), EAX )

    TEST_L    ( ESI, ESI )
    JZ        ( LLBL( G3TP3R_3 ) )

    MOVD      ( REGIND(ECX), MM0 )	/*                 | m00             */
    PUNPCKLDQ ( REGOFF(16, ECX), MM0 )	/* m10             | m00             */

    MOVD      ( REGOFF(4, ECX), MM1 )	/*                 | m01             */
    PUNPCKLDQ ( REGOFF(20, ECX), MM1 )	/* m11             | m01             */

    MOVD      ( REGOFF(8, ECX), MM2 )	/*                 | m02             */
    PUNPCKLDQ ( REGOFF(24, ECX), MM2 )	/* m12             | m02             */

    MOVQ      ( REGOFF(48, ECX), MM4 )	/* m31             | m30             */
    MOVD      ( REGOFF(56, ECX), MM5 )	/*                 | m32             */

ALIGNTEXT16
LLBL( G3TP3R_2 ):

    MOVQ      ( REGIND(EAX), MM6 )	/* x1              | x0              */
    MOVQ      ( MM6, MM7 )		/* x1              | x0              */

    PFMUL     ( MM0, MM6 )		/* x1*m10          | x0*m00          */
    PFMUL     ( MM1, MM7 )		/* x1*m11          | x0*m01          */

    PFACC     ( MM7, MM6 )		/* x0*m01+x1*m11   | x0*x00+x1*m10   */
    PFADD     ( MM4, MM6 )		/* x0*...*m11+m31  | x0*...*m10+m30  */

    MOVQ      ( MM6, REGIND(EDX) )	/* write r1, r0                      */
    MOVQ      ( REGIND(EAX), MM6 )	/* x1              | x0              */

    MOVQ      ( MM6, MM7 )		/* x1              | x0              */
    PFMUL     ( MM2, MM6 )		/* x1*m12          | x0*m02          */

    PFACC     ( MM7, MM6 )		/* ***trash***     | x0*x02+x1*m12   */
    PFADD     ( MM5, MM6 )		/* ***trash***     | x0*...*m12+m32  */

    MOVD      ( MM6, REGOFF(8, EDX) )	/* write r2                          */
    ADD_L     ( EDI, EAX )		/* next vertex                       */

    ADD_L     ( CONST(16), EDX )	/* next r                            */
    DEC_L     ( ESI )			/* decrement vertex counter          */

    JNZ       ( LLBL( G3TP3R_2 ) )	/* cnt > 0 ? -> process next vertex  */

LLBL( G3TP3R_3 ):

    FEMMS
    POP_L     ( EDI )
    POP_L     ( ESI )
    RET




ALIGNTEXT16
GLOBL GLNAME( _mesa_3dnow_transform_points2_3d_no_rot )
GLNAME( _mesa_3dnow_transform_points2_3d_no_rot ):

    PUSH_L    ( ESI )

    MOV_L     ( ARG_DEST, ECX )
    MOV_L     ( ARG_MATRIX, ESI )
    MOV_L     ( ARG_SOURCE, EAX )
    MOV_L     ( CONST(3), REGOFF(V4F_SIZE, ECX) )
    OR_B      ( CONST(VEC_SIZE_3 ), REGOFF(V4F_FLAGS, ECX) )
    MOV_L     ( REGOFF(V4F_COUNT, EAX), EDX )
    MOV_L     ( EDX, REGOFF(V4F_COUNT, ECX) )

    PUSH_L    ( EDI )

    MOV_L     ( REGOFF(V4F_START, ECX), EDX )
    MOV_L     ( ESI, ECX )
    MOV_L     ( REGOFF(V4F_COUNT, EAX), ESI )
    MOV_L     ( REGOFF(V4F_STRIDE, EAX), EDI )
    MOV_L     ( REGOFF(V4F_START, EAX), EAX )

    TEST_L    ( ESI, ESI )
    JZ        ( LLBL( G3TP3NRR_3 ) )

    MOVD      ( REGIND(ECX), MM0 )	/*                 | m00             */
    PUNPCKLDQ ( REGOFF(20, ECX), MM0 )	/* m11             | m00             */

    MOVQ      ( REGOFF(48, ECX), MM2 )	/* m31             | m30             */
    MOVD      ( REGOFF(56, ECX), MM3 )	/*                 | m32             */

ALIGNTEXT16
LLBL( G3TP3NRR_2 ):

    MOVQ      ( REGIND(EAX), MM4 )	/* x1              | x0              */
    PFMUL     ( MM0, MM4 )		/* x1*m11          | x0*m00          */

    PFADD     ( MM2, MM4 )		/* x1*m11+m31      | x0*m00+m30      */
    MOVQ      ( MM4, REGIND(EDX) )	/* write r1, r0                      */

    MOVD      ( MM3, REGOFF(8, EDX) )	/* write r2                          */
    ADD_L     ( EDI, EAX )		/* next vertex                       */

    ADD_L     ( CONST(16), EDX )	/* next r                            */
    DEC_L     ( ESI )			/* decrement vertex counter          */

    JNZ       ( LLBL( G3TP3NRR_2 ) )	/* cnt > 0 ? -> process next vertex  */

LLBL( G3TP3NRR_3 ):

    FEMMS
    POP_L     ( EDI )
    POP_L     ( ESI )
    RET




ALIGNTEXT16
GLOBL GLNAME( _mesa_3dnow_transform_points2_2d )
GLNAME( _mesa_3dnow_transform_points2_2d ):

    PUSH_L    ( ESI )

    MOV_L     ( ARG_DEST, ECX )
    MOV_L     ( ARG_MATRIX, ESI )
    MOV_L     ( ARG_SOURCE, EAX )
    MOV_L     ( CONST(2), REGOFF(V4F_SIZE, ECX) )
    OR_B      ( CONST(VEC_SIZE_2), REGOFF(V4F_FLAGS, ECX) )
    MOV_L     ( REGOFF(V4F_COUNT, EAX), EDX )
    MOV_L     ( EDX, REGOFF(V4F_COUNT, ECX) )

    PUSH_L    ( EDI )

    MOV_L     ( REGOFF(V4F_START, ECX), EDX )
    MOV_L     ( ESI, ECX )
    MOV_L     ( REGOFF(V4F_COUNT, EAX), ESI )
    MOV_L     ( REGOFF(V4F_STRIDE, EAX), EDI )
    MOV_L     ( REGOFF(V4F_START, EAX), EAX )

    TEST_L    ( ESI, ESI )
    JZ        ( LLBL( G3TP2R_3 ) )

    MOVQ      ( REGIND(ECX), MM0 )	/* m01             | m00             */
    MOVQ      ( REGOFF(16, ECX), MM1 )	/* m11             | m10             */

    MOVQ      ( REGOFF(48, ECX), MM2 )	/* m31             | m30             */

ALIGNTEXT16
LLBL( G3TP2R_2 ):

    MOVD      ( REGIND(EAX), MM4 )	/*                 | x0              */
    MOVD      ( REGOFF(4, EAX), MM5 )	/*                 | x1              */

    PUNPCKLDQ ( MM4, MM4 )		/* x0              | x0              */
    ADD_L     ( EDI, EAX )		/* next vertex                       */

    PFMUL     ( MM0, MM4 )		/* x0*m01          | x0*m00          */
    PUNPCKLDQ ( MM5, MM5 )		/* x1              | x1              */

    PFMUL     ( MM1, MM5 )		/* x1*m11          | x1*m10          */
    PFADD     ( MM2, MM4 )		/* x...x1*m11+31   | x0*..*m10+m30   */

    PFADD     ( MM5, MM4 )		/* x0*m01+x1*m11   | x0*m00+x1*m10   */
    MOVQ      ( MM4, REGIND(EDX) )	/* write r1, r0                      */

    ADD_L     ( CONST(16), EDX )	/* next r                            */
    DEC_L     ( ESI )			/* decrement vertex counter          */

    JNZ       ( LLBL( G3TP2R_2 ) )	/* cnt > 0 ? -> process next vertex  */

LLBL( G3TP2R_3 ):

    FEMMS
    POP_L     ( EDI )
    POP_L     ( ESI )
    RET




ALIGNTEXT16
GLOBL GLNAME( _mesa_3dnow_transform_points2_2d_no_rot )
GLNAME( _mesa_3dnow_transform_points2_2d_no_rot ):

    PUSH_L    ( ESI )

    MOV_L     ( ARG_DEST, ECX )
    MOV_L     ( ARG_MATRIX, ESI )
    MOV_L     ( ARG_SOURCE, EAX )
    MOV_L     ( CONST(2), REGOFF(V4F_SIZE, ECX) )
    OR_B      ( CONST(VEC_SIZE_2), REGOFF(V4F_FLAGS, ECX) )
    MOV_L     ( REGOFF(V4F_COUNT, EAX), EDX )
    MOV_L     ( EDX, REGOFF(V4F_COUNT, ECX) )

    PUSH_L    ( EDI )

    MOV_L     ( REGOFF(V4F_START, ECX), EDX )
    MOV_L     ( ESI, ECX )
    MOV_L     ( REGOFF(V4F_COUNT, EAX), ESI )
    MOV_L     ( REGOFF(V4F_STRIDE, EAX), EDI )
    MOV_L     ( REGOFF(V4F_START, EAX), EAX )

    TEST_L    ( ESI, ESI )
    JZ        ( LLBL( G3TP2NRR_3 ) )

    MOVD      ( REGIND(ECX), MM0 )	/*                 | m00             */
    PUNPCKLDQ ( REGOFF(20, ECX), MM0 )	/* m11             | m00             */

    MOVQ      ( REGOFF(48, ECX), MM2 )	/* m31             | m30             */

ALIGNTEXT16
LLBL( G3TP2NRR_2 ):

    MOVQ      ( REGIND(EAX), MM4 )	/* x1              | x0              */
    ADD_L     ( EDI, EAX )		/* next vertex                       */

    PFMUL     ( MM0, MM4 )		/* x1*m11          | x0*m00          */
    PFADD     ( MM2, MM4 )		/* m31             | x0*m00+m30      */

    MOVQ      ( MM4, REGIND(EDX) )	/* write r1, r0                      */
    ADD_L     ( CONST(16), EDX )	/* next r                            */

    DEC_L     ( ESI )			/* decrement vertex counter          */
    JNZ       ( LLBL( G3TP2NRR_2 ) )	/* cnt > 0 ? -> process next vertex  */

LLBL( G3TP2NRR_3 ):

    FEMMS
    POP_L     ( EDI )
    POP_L     ( ESI )
    RET




ALIGNTEXT16
GLOBL GLNAME( _mesa_3dnow_transform_points2_identity )
GLNAME( _mesa_3dnow_transform_points2_identity ):

    PUSH_L    ( ESI )

    MOV_L     ( ARG_DEST, ECX )
    MOV_L     ( ARG_MATRIX, ESI )
    MOV_L     ( ARG_SOURCE, EAX )
    MOV_L     ( CONST(2), REGOFF(V4F_SIZE, ECX) )
    OR_B      ( CONST(VEC_SIZE_2), REGOFF(V4F_FLAGS, ECX) )
    MOV_L     ( REGOFF(V4F_COUNT, EAX), EDX )
    MOV_L     ( EDX, REGOFF(V4F_COUNT, ECX) )

    PUSH_L    ( EDI )

    MOV_L     ( REGOFF(V4F_START, ECX), EDX )
    MOV_L     ( ESI, ECX )
    MOV_L     ( REGOFF(V4F_COUNT, EAX), ESI )
    MOV_L     ( REGOFF(V4F_STRIDE, EAX), EDI )
    MOV_L     ( REGOFF(V4F_START, EAX), EAX )

    TEST_L    ( ESI, ESI )
    JZ        ( LLBL( G3TPIR_3 ) )

ALIGNTEXT16
LLBL( G3TPIR_3 ):

    MOVQ      ( REGIND(EAX), MM0 )	/* x1              | x0              */
    ADD_L     ( EDI, EAX )		/* next vertex                       */

    MOVQ      ( MM0, REGIND(EDX) )	/* r1              | r0              */
    ADD_L     ( CONST(16), EDX )	/* next r                            */

    DEC_L     ( ESI )			/* decrement vertex counter          */
    JNZ       ( LLBL( G3TPIR_3 ) )	/* cnt > 0 ? -> process next vertex  */

LLBL( G3TPIR_4 ):

    FEMMS
    POP_L     ( EDI )
    POP_L     ( ESI )
    RET
#endif
