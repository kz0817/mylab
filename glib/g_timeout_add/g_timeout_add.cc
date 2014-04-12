#include <cstdio>
#include <cstdlib>
#include <glib.h>

static gboolean timeoutCb(gpointer user_data)
{
	printf("timed out\n");
	return G_SOURCE_CONTINUE;
}

static gboolean timeoutCb2(gpointer user_data)
{
	printf("timed out2\n");
	return G_SOURCE_CONTINUE;
}

int main(void)
{
	GMainLoop *loop = g_main_loop_new(NULL, FALSE);
	guint interval = 5 * 1000; // ms
	printf("Start interval timer: interval: %u ms\n", interval);
	g_timeout_add(interval, timeoutCb, NULL);
	g_timeout_add(interval*3/5, timeoutCb2, NULL);
	g_main_loop_run(loop);
	return EXIT_SUCCESS;
}
