# bitray-engine

## Samples

There are some low-level rendering samples available under engine/samples/20_low_level_rendering.
Once compiled you have to copy the rhi libraries you want to use to that directory:
- engine/plugins/rhi_opengl/
- engine/plugins/rhi_vulkan/

And then you can call it as:
```
./20_low_level_rendering --rhi opengl --example Texture
```

Example can be one of:
```
- Empty
- ComputeShader
- CubeTexture
- GeometryShader
- IndirectBuffer
- Instancing
- MeshShader
- Queries
- RenderToTexture
- TessellationShader
- Texture
- Triangle
- VertexBuffer
- Gpgpu
- IcosahedronTessellation
- InstancedCubes
```