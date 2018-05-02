#include <rsge/cl/cl.h>
#include <rsge/cl/context.h>
#include <log.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_cl_ctx_create(rsge_cl_ctx_t* ctx,int platformIndex) {
    memset(ctx,0,sizeof(rsge_cl_ctx_t));
    
    cl_uint platformIdCount = 0;
    clGetPlatformIDs(0,nullptr,&platformIdCount);
    if(platformIdCount == 0) {
        log_error("No available OpenCL platforms");
        return RSGE_ERROR_OPENCL;
    }
    
    std::vector<cl_platform_id> platformIds(platformIdCount);
    clGetPlatformIDs(platformIdCount,platformIds.data(),nullptr);
    
    if(platformIndex < 0 || platformIndex >= platformIdCount) return RSGE_ERROR_CL_INVALID_PLATFORM;
    
    cl_uint deviceIdCount = 0;
    clGetDeviceIDs(platformIds[platformIndex],CL_DEVICE_TYPE_ALL,0,nullptr,&deviceIdCount);
    if(deviceIdCount == 0) {
        log_error("No available OpenCL devices");
        return RSGE_ERROR_OPENCL;
    }
    
    std::vector<cl_device_id> deviceIds(deviceIdCount);
    clGetDeviceIDs(platformIds[platformIndex],CL_DEVICE_TYPE_ALL,deviceIdCount,deviceIds.data(),nullptr);
    
    const cl_context_properties contextProperties[] = {
        CL_CONTEXT_PLATFORM,
        reinterpret_cast<cl_context_properties>(platformIds[platformIndex]),
        0,0
    };
    cl_int error;
    ctx->context = clCreateContext(contextProperties,deviceIdCount,deviceIds.data(),nullptr,nullptr,&error);
    if(error != CL_SUCCESS) {
        log_error("OpenCL: got error code %d",error);
        return RSGE_ERROR_OPENCL;
    }
    return RSGE_ERROR_NONE;
}