__kernel void sample
(
    __global uint* in, 
    __constant uint* salt, 
    __global uint* out
) 
{
    int num = get_global_id(0);
    int x = in[num] + 42;
    
    out[num] = x;
}