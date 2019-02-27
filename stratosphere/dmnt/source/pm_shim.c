/*
 * Copyright (c) 2018 Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <switch.h>
#include "pm_shim.h"

/* Atmosphere extension commands. */
Result pmdmntAtmosphereGetProcessHandle(Handle* out, u64 pid) {
    IpcCommand c;
    ipcInitialize(&c);
    Service *s = pmdmntGetServiceSession();

    struct {
        u64 magic;
        u64 cmd_id;
        u64 pid;
    } *raw;

    raw = serviceIpcPrepareHeader(s, &c, sizeof(*raw));

    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 65000;
    raw->pid = pid;

    Result rc = serviceIpcDispatch(s);

    if (R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        struct {
            u64 magic;
            u64 result;
        } *resp;

        serviceIpcParse(s, &r, sizeof(*resp));
        resp = r.Raw;

        rc = resp->result;

        if (R_SUCCEEDED(rc)) {
            *out = r.Handles[0];
        }
    }

    return rc;
}
