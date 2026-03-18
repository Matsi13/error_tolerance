# Network.yml

文件包括四个参数：

```yaml
# 3D (Ring_FullyConnected_Switch) topology
topology: [ Ring, FullyConnected, Switch ]

# 2 x 8 x 4 = 64 NPUs
npus_count: [ 2, 8, 4 ]  # number of NPUs

# Bandwidth per each dimension
bandwidth: [ 200.0, 100.0, 50.0 ]  # GB/s

# Latency per each dimension
latency: [ 50.0, 500.0, 2000.0 ]  # ns
```

注意，Congestion_Aware只支持一维网络

详见https://astra-sim.github.io/astra-network-analytical-docs/input-format/npus-count.html