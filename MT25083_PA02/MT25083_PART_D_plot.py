import matplotlib.pyplot as plt

sizes = [256,512,1024,4096]
threads = [1,2,4,8]

############################################################
# --------- THROUGHPUT vs MESSAGE SIZE ---------------------
############################################################

# A1
A1 = [
    [0.043480,0.040472,0.037136,0.034839],
    [0.083337,0.083375,0.071613,0.069450],
    [0.158700,0.152121,0.143832,0.133553],
    [0.291892,0.281104,0.269761,0.245753]
]

# A2
A2 = [
    [0.031796,0.031664,0.030425,0.028349],
    [0.065695,0.060464,0.058547,0.056818],
    [0.128419,0.121523,0.110969,0.110759],
    [0.244212,0.234719,0.216921,0.206719]
]

# A3
A3 = [
    [0.028028,0.024134,0.026598,0.024715],
    [0.053825,0.050728,0.051128,0.048321],
    [0.104330,0.102467,0.098751,0.092632],
    [0.200727,0.193582,0.190577,0.181373]
]

def plot_throughput(data,title,name):
    plt.figure()
    for i in range(4):
        plt.plot(sizes,data[i],marker='o',label=f"{threads[i]} threads")
    plt.xlabel("Message Size (bytes)")
    plt.ylabel("Throughput (Gbps)")
    plt.title(title)
    plt.legend()
    plt.grid()
    plt.savefig(name)
    plt.close()

plot_throughput(A1,"A1 Two-Copy: Throughput vs Message Size","thr_A1.png")
plot_throughput(A2,"A2 One-Copy: Throughput vs Message Size","thr_A2.png")
plot_throughput(A3,"A3 Zero-Copy: Throughput vs Message Size","thr_A3.png")

############################################################
# --------- LATENCY vs THREAD COUNT ------------------------
############################################################

A1_lat = [
    [376.818,196.599,103.239,56.130],
    [404.826,196.510,107.704,58.285],
    [441.189,228.786,113.911,60.735],
    [470.278,235.910,122.678,66.668]
]

A2_lat = [
    [515.278,249.395,127.582,67.089],
    [517.438,270.973,134.822,69.803],
    [538.503,279.846,147.645,75.530],
    [577.934,288.359,147.925,79.258]
]

A3_lat = [
    [584.556,304.395,157.040,81.623],
    [678.887,322.977,159.895,84.636],
    [615.991,320.451,165.912,85.970],
    [662.910,339.064,176.872,90.333]
]

def plot_latency(data,title,name):
    plt.figure()
    for i in range(4):
        plt.plot(threads,data[i],marker='o',label=f"{sizes[i]} bytes")
    plt.xlabel("Thread Count")
    plt.ylabel("Latency (microseconds)")
    plt.title(title)
    plt.legend()
    plt.grid()
    plt.savefig(name)
    plt.close()

plot_latency(A1_lat,"A1 Two-Copy: Latency vs Threads","lat_A1.png")
plot_latency(A2_lat,"A2 One-Copy: Latency vs Threads","lat_A2.png")
plot_latency(A3_lat,"A3 Zero-Copy: Latency vs Threads","lat_A3.png")

############################################################
# --------- CACHE MISSES vs MESSAGE SIZE -------------------
############################################################

A1_cache = [
    [188045,185732,193617,180002],
    [505040,570080,427409,465641],
    [912908,905861,879686,842712],
    [1743128,1725786,1712420,1589145]
]

A2_cache = [
    [213412,190551,202077,173774],
    [401309,406508,373407,364630],
    [795758,815691,715563,761119],
    [1558537,1493933,1405174,1224236]
]

A3_cache = [
    [181344,194547,200356,151725],
    [365060,337090,361003,344081],
    [682513,648830,646457,624995],
    [1164511,1160311,1112124,1036011]
]

def plot_cache(data,title,name):
    plt.figure()
    for i in range(4):
        plt.plot(sizes,data[i],marker='o',label=f"{threads[i]} threads")
    plt.xlabel("Message Size (bytes)")
    plt.ylabel("Cache Misses")
    plt.title(title)
    plt.legend()
    plt.grid()
    plt.savefig(name)
    plt.close()

plot_cache(A1_cache,"A1 Two-Copy: Cache Misses vs Size","cache_A1.png")
plot_cache(A2_cache,"A2 One-Copy: Cache Misses vs Size","cache_A2.png")
plot_cache(A3_cache,"A3 Zero-Copy: Cache Misses vs Size","cache_A3.png")

############################################################
# --------- CPU CYCLES PER BYTE ----------------------------
############################################################

bytes_per_msg = 4096*8
A1_cycles = 4695440309/bytes_per_msg
A2_cycles = 3893590862/bytes_per_msg
A3_cycles = 3430967983/bytes_per_msg

plt.figure()
plt.bar(["Two-Copy","One-Copy","Zero-Copy"],[A1_cycles,A2_cycles,A3_cycles])
plt.ylabel("CPU Cycles per Byte")
plt.title("CPU Cycles per Byte Transferred")
plt.savefig("cycles_per_byte.png")
plt.close()

print("All graphs generated.")
