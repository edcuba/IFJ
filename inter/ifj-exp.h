/* ifj-exp.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Jan Demcak <xdemca01@stud.fit.vutbr.cz>
 */

#ifndef IFJ_EXP_H
#define IFJ_EXP_H

#include "ifj-inter.h"
#include "ifj-util.h"
#include "ifj-syna.h"
#include "ifj-lexa.h"
#include "ifj-token.h"
#include <stdbool.h>

int expresion(ifjInter *self);
int condition(ifjInter *self);

#endif
