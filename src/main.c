/*
 * This file is part of capataz
 *
 * Copyright (C) 2015 Eric Le Bihan <eric.le.bihan.dev@free.fr>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdlib.h>
#include <glib.h>
#include <glib/gi18n.h>
#include "config.h"

int main(int argc, char *argv[])
{
	static gboolean show_version = FALSE;
	static gboolean forever = FALSE;
	static char *cmd_failure = NULL;
	static char *cmd_success = NULL;
	static gchar **args = NULL;
	static gint n_runs = 1;
	static gint delay = 0;
	static GOptionEntry entries[] = {
		{ "version", 'V', 0, G_OPTION_ARG_NONE, &show_version,
		  N_("Show the program version" ) },
		{ "forever", 'F', 0, G_OPTION_ARG_NONE, &forever,
		  N_("Run the program forever") },
		{ "iterations", 'N', 0, G_OPTION_ARG_INT, &n_runs,
		  N_("Set the number of iterations"), "N" },
		{ "delay", 'd', 0, G_OPTION_ARG_INT, &delay,
		  N_("Set the number of seconds between iterations"), "N" },
		{ "failure", 'f', 0, G_OPTION_ARG_STRING, &cmd_failure,
		  N_("Set the command to run on failure"), "COMMAND" },
		{ "success", 's', 0, G_OPTION_ARG_STRING, &cmd_success,
		  N_("Set the command to run on success"), "COMMAND" },
		{ G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_STRING_ARRAY, &args,
		  N_("PROGRAM [<ARGUMENT>, ...]") },
		{ NULL, },
	};
	GOptionContext *context = NULL;
	GError *error = NULL;
	gboolean status = FALSE;
	gchar *cmd_notify = NULL;
	gint exit_status = 0;
	gint count = 0;
	int rc = 0;

	context = g_option_context_new(_("<program> [<argument>, ...]"));
	g_option_context_add_main_entries(context, entries, NULL);
	if (!g_option_context_parse(context, &argc, &argv, &error)) {
		g_printerr(_("Invalid option (%s)\n"), error->message);
		g_error_free(error);
		exit(1);
	}
	g_option_context_free(context);

	if (show_version) {
		g_print(PACKAGE_VERSION "\n");
		rc = 0;
		goto out;
	}

	if (!args) {
		g_printerr(_("Missing program\n"));
		rc = 1;
		goto out;
	}

	if (forever) n_runs = 0;

	do {
		if (!g_spawn_sync(NULL,
				  args,
				  NULL,
				  G_SPAWN_SEARCH_PATH,
				  NULL,
				  NULL,
				  NULL,
				  NULL,
				  &exit_status,
				  &error)) {
			g_printerr("Failed to run child program (%s)\n",
				   error? error->message: _("Unknown error"));
			g_error_free(error);
			rc = 2;
			break;
		}

		if (WIFEXITED(exit_status)) {
			cmd_notify = WEXITSTATUS(exit_status)? cmd_failure: cmd_success;
		}

		if (cmd_notify) {
			if (!g_spawn_command_line_sync(cmd_notify,
						       NULL,
						       NULL,
						       &exit_status,
						       &error)) {
				g_printerr("Failed to run notification (%s)\n",
					   error? error->message: _("Unknown error"));
				g_error_free(error);
			}
		}

		count++;
		if ((n_runs > 0) && (count >= n_runs)) break;
		if (delay) g_usleep(delay * G_USEC_PER_SEC);

	} while (TRUE);

out:
	g_strfreev(args);

	return rc;
}
