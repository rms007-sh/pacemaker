/* 
 * Copyright (C) 2004 Andrew Beekhof <andrew@beekhof.net>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef CRM_ATTRD__H
#  define CRM_ATTRD__H

#ifdef __cplusplus
extern "C" {
#endif

#  include <crm/common/ipc.h>

/* attribute options for clients to use with these functions */
#define attrd_opt_none    0x000
#define attrd_opt_remote  0x001
#define attrd_opt_private 0x002

const char *attrd_get_target(const char *name);

int attrd_update_delegate(crm_ipc_t * ipc, char command, const char *host,
                          const char *name, const char *value, const char *section,
                          const char *set, const char *dampen, const char *user_name, int options);
int attrd_clear_delegate(crm_ipc_t *ipc, const char *host, const char *resource,
                         const char *operation, const char *interval,
                         const char *user_name, int options);

#ifdef __cplusplus
}
#endif

#endif
