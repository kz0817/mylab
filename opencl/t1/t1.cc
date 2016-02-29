#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>
#include <time.h>
#include <CL/cl.h>
#include <exception>
#include <functional>
using namespace std;

static void checkResult(const cl_int code, const string &method,
                        function<bool()> hook = []{ return false; } )
{
	if (code == CL_SUCCESS)
		return;
	if (hook())
		return;
	fprintf(stderr, "Failed: %s, code: %d\n", method.c_str(), code);
	throw new exception();
}

struct ProgramContext {
	static constexpr size_t NUM_ELEM = 3;
	const size_t   num;
	string         krnl_add_src;
	cl_platform_id platform_id;
	cl_uint        num_platforms;
	float         *mem[NUM_ELEM];
	cl_mem         dev[NUM_ELEM];

	ProgramContext(const size_t n)
	: num(n)
	{
		memForeach([&](float *&m) { m = new float[n]; });
		initInput();
	}

	virtual ~ProgramContext()
	{
		for (int i = 0; i < NUM_ELEM; i++) {
			delete [] mem[i];
			clReleaseMemObject(dev[i]);
		}
	}

	void memForeach(function<void (float *&)> fn)
	{
		for (auto &m: mem)
			fn(m);
	}

	cl_mem &outDev(void)
	{
		return dev[2];
	}

	float *&outMem(void)
	{
		return mem[2];
	}

	void devForeach(function<void (cl_mem &)> fn)
	{
		for (auto &d: dev)
			fn(d);
	}

	void memDevForeach(function<void (float *&mem, cl_mem &dev)> fn)
	{
		for (int i = 0; i < NUM_ELEM; i++)
			fn(mem[i], dev[i]);
	}

	void readKernelSource(void)
	{
		const char *filename = "add.cl";
		ifstream ifs(filename);
		if (!ifs) {
			fprintf(stderr, "Failed to read kernel source: %s\n",
			        filename);
			throw new exception();
		}

		istreambuf_iterator<char> it(ifs);
		istreambuf_iterator<char> last;
		krnl_add_src = string(it, last);
	}

	void initInput(void)
	{
		for (size_t i = 0; i < num; ++i) {
			mem[0][i] = 2.0 * i;
			mem[1][i] = -1.5 * i;
			mem[2][i] = 0; // for output
		}
	}

	void showResult(void)
	{
		for (size_t i = 0; i < num; ++i) {
			printf("i: %zd, %f + %f => %f\n",
			       i, mem[0][i], mem[1][i], mem[2][i]);
		}
	}
};

int main(void)
{
	cl_int ret;
	ProgramContext pcx(20);
	pcx.readKernelSource();

	ret = clGetPlatformIDs(1, &pcx.platform_id, &pcx.num_platforms);
	checkResult(ret, "clGetPlatformIDs");
	printf("Platfrom ID: %p\n", pcx.platform_id);

	cl_uint num_devices;
	cl_device_id device_id;
	ret  = clGetDeviceIDs(pcx.platform_id, CL_DEVICE_TYPE_DEFAULT,
	                      1, &device_id, &num_devices);
	checkResult(ret, "clGetDeviceIDs");
	printf("Device ID: %p\n", device_id);

	cl_context ctx = clCreateContext(NULL, 1u,
	                                 &device_id, NULL, NULL, &ret);
	checkResult(ret, "clCreateContext");

#ifdef CL_EXT_PREFIX__VERSION_2_0_DEPRECATED
	cl_command_queue cmdq = clCreateCommandQueueWithProperties(
	                          ctx, device_id, NULL, &ret);
#else
	cl_command_queue cmdq = clCreateCommandQueue(
	                          ctx, device_id, 0, &ret);
#endif
	checkResult(ret, "clCreateCommandQueueWithProperties");

	const char *src = pcx.krnl_add_src.c_str();
	const size_t size = pcx.krnl_add_src.size();
	cl_program prog = clCreateProgramWithSource(ctx, 1, &src, &size, &ret);
	checkResult(ret, "clCreateProgramWithSource");

	ret = clBuildProgram(prog, 1, &device_id, NULL, NULL, NULL);
	auto showBuildInfo = [&] {
		const size_t bufsize = 0x1000;
		char buf[bufsize];
		size_t actual_size;
		cl_int code = clGetProgramBuildInfo(
		  prog, device_id, CL_PROGRAM_BUILD_LOG,
		  bufsize, buf, &actual_size);
		fprintf(stderr, "[size:%ld] %s", actual_size, buf);
		checkResult(code, "clGetProgramBuildInfo");
		return false;
	};
	checkResult(ret, "clBuildProgram", showBuildInfo);

	cl_kernel krnl = clCreateKernel(prog, "add", &ret);
	checkResult(ret, "clCreateKernel");

	pcx.devForeach([&](cl_mem &dev) {
		dev = clCreateBuffer(ctx, CL_MEM_READ_WRITE,
		                     pcx.num * sizeof(float), NULL, &ret);
		checkResult(ret, "clCreateKernel");
	});

	pcx.memDevForeach([&](float *&mem, cl_mem &dev) {
		ret = clEnqueueWriteBuffer(cmdq, dev, CL_TRUE,
		                           0, pcx.num * sizeof(float), mem,
		                           0, NULL, NULL);
		checkResult(ret, "clEnqueueWriteBuffer");
	});

	int idx = 0;
	clSetKernelArg(krnl, idx++, sizeof(size_t), &pcx.num);
	pcx.devForeach([&](cl_mem &dev) {
		clSetKernelArg(krnl, idx++, sizeof(cl_mem), &dev);
	});

	size_t global_work_size[3] = {pcx.num, 0, 0};
	size_t local_work_size[3] = {pcx.num, 0, 0};
	ret = clEnqueueNDRangeKernel(cmdq, krnl, 1, NULL,
	                             global_work_size, local_work_size,
	                             0, NULL, NULL);
	checkResult(ret, "clEnqueueNDRangeKernel");

	ret = clFinish(cmdq);
	checkResult(ret, "clFinish");

	ret = clEnqueueReadBuffer(cmdq, pcx.outDev(), CL_TRUE, 0,
	                          pcx.num * sizeof(float),
	                          pcx.outMem(), 0, NULL, NULL);
	checkResult(ret, "clEnqueueReadBuffer");
	pcx.showResult();

	checkResult(clReleaseKernel(krnl),       "clRleaseKernel");
	checkResult(clReleaseProgram(prog),      "clRelaseProgram");
	checkResult(clReleaseCommandQueue(cmdq), "clReleaseCommandQueue");
	checkResult(clReleaseContext(ctx),       "clRelaseContext");

	return EXIT_SUCCESS;
}
