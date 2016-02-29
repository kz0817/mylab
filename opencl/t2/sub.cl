__kernel void sub(uint n, __global float *a, __global float *b,
		  __global float *s) {
	int id = get_global_id(0);
	if (id < n) {
		s[id] = a[id] - b[id];
	}
}
