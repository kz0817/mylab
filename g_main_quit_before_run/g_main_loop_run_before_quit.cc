#include <cstdio>
#include <cstdlib>
#include <glib.h>

int main(void)
{
	GMainLoop *loop = g_main_loop_new(NULL, FALSE);
	g_main_quit(loop);
	g_main_loop_run(loop);
	g_main_loop_unref(loop);
	return EXIT_SUCCESS;
}

