/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 *  Copyright © 2002 Olivier Martin <omartin@ifrance.com>
 *  Copyright © 2002 Jorn Baayen <jorn@nl.linux.org>
 *
 *  This file is part of Epiphany.
 *
 *  Epiphany is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Epiphany is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Epiphany.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <glib-object.h>

#include "ephy-node.h"

G_BEGIN_DECLS

#define EPHY_TYPE_NODE_FILTER (ephy_node_filter_get_type ())

G_DECLARE_FINAL_TYPE (EphyNodeFilter, ephy_node_filter, EPHY, NODE_FILTER, GObject)

typedef enum
{
  EPHY_NODE_FILTER_EXPRESSION_ALWAYS_TRUE,           /* args: none */
  EPHY_NODE_FILTER_EXPRESSION_NODE_EQUALS,           /* args: EphyNode *a, EphyNode *b */
  EPHY_NODE_FILTER_EXPRESSION_EQUALS,                /* args: EphyNode *node */
  EPHY_NODE_FILTER_EXPRESSION_HAS_PARENT,            /* args: EphyNode *parent */
  EPHY_NODE_FILTER_EXPRESSION_HAS_CHILD,             /* args: EphyNode *child */
  EPHY_NODE_FILTER_EXPRESSION_NODE_PROP_EQUALS,      /* args: int prop_id, EphyNode *node */
  EPHY_NODE_FILTER_EXPRESSION_CHILD_PROP_EQUALS,     /* args: int prop_id, EphyNode *node */
  EPHY_NODE_FILTER_EXPRESSION_STRING_PROP_CONTAINS,  /* args: int prop_id, const char *string */
  EPHY_NODE_FILTER_EXPRESSION_STRING_PROP_EQUALS,    /* args: int prop_id, const char *string */
  EPHY_NODE_FILTER_EXPRESSION_KEY_PROP_CONTAINS,     /* args: int prop_id, const char *string */
  EPHY_NODE_FILTER_EXPRESSION_KEY_PROP_EQUALS,       /* args: int prop_id, const char *string */
  EPHY_NODE_FILTER_EXPRESSION_INT_PROP_EQUALS,       /* args: int prop_id, int int */
  EPHY_NODE_FILTER_EXPRESSION_INT_PROP_BIGGER_THAN,  /* args: int prop_id, int int */
  EPHY_NODE_FILTER_EXPRESSION_INT_PROP_LESS_THAN     /* args: int prop_id, int int */
} EphyNodeFilterExpressionType;

typedef struct _EphyNodeFilterExpression EphyNodeFilterExpression;

/* The filter starts iterating over all expressions at level 0,
 * if one of them is TRUE it continues to level 1, etc.
 * If it still has TRUE when there are no more expressions at the
 * next level, the result is TRUE. Otherwise, it's FALSE.
 */

EphyNodeFilter *ephy_node_filter_new            (void);

void            ephy_node_filter_add_expression (EphyNodeFilter *filter,
                                                 EphyNodeFilterExpression *expression,
                                                 int level);

void            ephy_node_filter_empty          (EphyNodeFilter *filter);

void            ephy_node_filter_done_changing  (EphyNodeFilter *filter);

gboolean        ephy_node_filter_evaluate       (EphyNodeFilter *filter,
                                                 EphyNode *node);

EphyNodeFilterExpression *ephy_node_filter_expression_new  (EphyNodeFilterExpressionType,
                                                            ...);
/* no need to free unless you didn't add the expression to a filter */
void                      ephy_node_filter_expression_free (EphyNodeFilterExpression *expression);

G_END_DECLS
