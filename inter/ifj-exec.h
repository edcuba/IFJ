/* ifj-exec.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Robert Kolcun <xkolcu00@stud.fit.vutbr.cz>
 */

#ifndef IFJ_EXEC_H
#define IFJ_EXEC_H

#include "ifj-inter.h"

#define I_MUL 5000
#define I_ADD 5001
#define I_SUB 5002
#define I_DIV 5003

#define I_PUSH 5004
#define I_SET 5005

#define I_CALL 5006

#define I_GOTO 5007
#define I_GOTO_CONDITION 5008

#define I_RETURN 5009
#define I_CONDITION 5010
#define I_LABEL 5020

int exec_run ( ifjInter *self );
int checkCondition (	token *a,
						token *b,
						token *rel );
void freeTempTokens ( instruction *inputInstruc );
void printInstruction ( int type );

#endif
