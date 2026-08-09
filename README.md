# Low-Latency Limit Order Book (LOB) Engine

A high-performance, low-latency Limit Order Book matching engine built in modern C++. Designed with High-Frequency Trading (HFT) system design principles to ensure ultra-fast order execution and deterministic memory management.

## 🚀 Core Features & Architecture

* **Price-Time Priority Matching:** Standard FIFO execution for both Bids and Asks. Partial order fills and balance updates are fully supported.
* **$O(1)$ Memory Allocation:** Utilizes a custom **Slab Allocator (Memory Pool)** to pre-allocate memory for orders at startup. This completely bypasses the standard `new`/`delete` keywords, eliminating OS-level latency spikes during runtime.
* **$O(1)$ Order Cancellation:** Implements an **Intrusive Doubly Linked List** within the order queues. This allows orders to be removed from the book instantly without any $O(N)$ loop traversal or search overhead.
* **Fast Lookups:** `std::unordered_map` is used for constant-time $O(1)$ order ID lookups during the cancellation process.

## 🛠️ Tech Stack

* **Language:** C++17
* **Build System:** CMake
* **Environment:** GCC/MinGW (Windows) / Clang (macOS) / GCC (Linux)

## ⚙️ Build and Run Instructions

To compile and run this project on your local machine, ensure you have CMake and a C++17 compatible compiler installed.

```bash
# 1. Clone the repository
git clone [https://github.com/YOUR_USERNAME/HFT-LimitOrderBook.git](https://github.com/YOUR_USERNAME/HFT-LimitOrderBook.git)
cd HFT-LimitOrderBook

# 2. Create a build directory
mkdir build
cd build

# 3. Generate build files and compile
cmake ..
cmake --build .

# 4. Run the engine
./LimitOrderBook.exe    # For Windows
./LimitOrderBook        # For Linux/macOS