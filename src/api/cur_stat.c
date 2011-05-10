/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 2008-2011 WiredTiger, Inc.
 *	All rights reserved.
 */

#include "wt_internal.h"

/*
 * __curstat_first --
 *	WT_CURSOR->first method for the btree cursor type.
 */
static int
__curstat_first(WT_CURSOR *cursor)
{
	SESSION *session;
	int ret;

	CURSOR_API_CALL(cursor, session, first, NULL);
	ret = WT_NOTFOUND;
	API_END();

	return (ret);
}

/*
 * __curstat_last --
 *	WT_CURSOR->last method for the btree cursor type.
 */
static int
__curstat_last(WT_CURSOR *cursor)
{
	SESSION *session;
	int ret;

	CURSOR_API_CALL(cursor, session, last, NULL);
	ret = ENOTSUP;
	API_END();

	return (ret);
}

/*
 * __curstat_next --
 *	WT_CURSOR->next method for the btree cursor type.
 */
static int
__curstat_next(WT_CURSOR *cursor)
{
	SESSION *session;
	int ret;

	CURSOR_API_CALL(cursor, session, next, NULL);
	ret = WT_NOTFOUND;
	API_END();

	return (ret);
}

/*
 * __curstat_prev --
 *	WT_CURSOR->prev method for the btree cursor type.
 */
static int
__curstat_prev(WT_CURSOR *cursor)
{
	SESSION *session;
	int ret;

	CURSOR_API_CALL(cursor, session, prev, NULL);
	ret = ENOTSUP;
	API_END();

	return (ret);
}

/*
 * __curstat_search_near --
 *	WT_CURSOR->search_near method for the btree cursor type.
 */
static int
__curstat_search_near(WT_CURSOR *cursor, int *lastcmp)
{
	SESSION *session;
	int ret;

	WT_UNUSED(lastcmp);

	CURSOR_API_CALL(cursor, session, search_near, NULL);
	ret = ENOTSUP;
	API_END();

	return (ret);
}

/*
 * __curstat_insert --
 *	WT_CURSOR->insert method for the btree cursor type.
 */
static int
__curstat_insert(WT_CURSOR *cursor)
{
	SESSION *session;
	int ret;

	CURSOR_API_CALL(cursor, session, insert, NULL);
	ret = ENOTSUP;
	API_END();

	return (ret);
}

/*
 * __curstat_update --
 *	WT_CURSOR->update method for the btree cursor type.
 */
static int
__curstat_update(WT_CURSOR *cursor)
{
	SESSION *session;
	int ret;

	CURSOR_API_CALL(cursor, session, update, NULL);
	ret = ENOTSUP;
	API_END();

	return (ret);
}

/*
 * __curstat_remove --
 *	WT_CURSOR->remove method for the btree cursor type.
 */
static int
__curstat_remove(WT_CURSOR *cursor)
{
	SESSION *session;
	int ret;

	CURSOR_API_CALL(cursor, session, insert, NULL);
	ret = ENOTSUP;
	API_END();

	return (ret);
}

/*
 * __curstat_close --
 *	WT_CURSOR->close method for the btree cursor type.
 */
static int
__curstat_close(WT_CURSOR *cursor, const char *config)
{
	SESSION *session;
	int ret;

	CURSOR_API_CALL_CONF(cursor, session, close, NULL, config);
	ret = 0;
	WT_TRET(__wt_cursor_close(cursor, config));
	API_END();

	return (ret);
}

/*
 * __wt_cursor_open --
 *	WT_SESSION->open_cursor method for the btree cursor type.
 */
int
__wt_curstat_open(SESSION *session,
    const char *uri, const char *config, WT_CURSOR **cursorp)
{
	static WT_CURSOR iface = {
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		__curstat_first,
		__curstat_last,
		__curstat_next,
		__curstat_prev,
		NULL,
		__curstat_search_near,
		__curstat_insert,
		__curstat_update,
		__curstat_remove,
		__curstat_close,
		{ NULL, NULL },		/* TAILQ_ENTRY q */
		0,			/* recno key */
		{ NULL, 0, 0, NULL, 0 },/* WT_BUF key */
		{ NULL, 0, 0, NULL, 0 },/* WT_BUF value */
		0,			/* int saved_err */
		0			/* uint32_t flags */
	};
	CURSOR_STAT *cst;
	WT_CONFIG_ITEM cval;
	WT_CURSOR *cursor;
	int dump, printable, raw, ret;

	API_CONF_INIT(session, open_cursor, config);

	/* Skip "stat:". */
	uri += 5;

	WT_ERR(__wt_config_gets(__cfg, "dump", &cval));
	dump = (cval.val != 0);
	WT_ERR(__wt_config_gets(__cfg, "printable", &cval));
	printable = (cval.val != 0);
	WT_ERR(__wt_config_gets(__cfg, "raw", &cval));
	raw = (cval.val != 0);

	WT_RET(__wt_calloc_def(session, 1, &cst));

	cursor = &cst->iface;
	*cursor = iface;
	cursor->session = &session->iface;
	cursor->key_format = "S";
	cursor->value_format = printable ? "S" : "q";
	__wt_cursor_init(cursor, config);

	if (dump)
		__wt_curdump_init(cursor, printable);
	if (raw)
		F_SET(cursor, WT_CURSTD_RAW);

	STATIC_ASSERT(offsetof(CURSOR_STAT, iface) == 0);
	TAILQ_INSERT_HEAD(&session->cursors, cursor, q);
	*cursorp = cursor;

	if (0) {
err:		__wt_free(session, cst);
	}

	return (ret);
}
