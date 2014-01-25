CXXFLAGS = `pkg-config --cflags glib-2.0`
LDFLAGS = `pkg-config --libs glib-2.0`
g_main_loop_run_before_quit: g_main_loop_run_before_quit.cc
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS)
