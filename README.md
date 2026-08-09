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
git clone [https://github.com/codepundit24/HFT-LimitOrderBook.git](https://github.com/codepundit24/HFT-LimitOrderBook.git)
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

```

## 📊 Sample Execution Output

--- CURRENT ORDER BOOK ---
ASKS (Sell Side):
BIDS (Buy Side):
  $148 => [ (ID:101 Qty:50) (ID:102 Qty:100) (ID:103 Qty:75) ]
--------------------------

Cancelling Order ID 102
>>> ORDER CANCELLED: ID 102 <<<

Incoming Sell Order for 80 shares @ $148

>>> MATCH EXECUTED <<<
Traded Quantity: 50 @ price: $148 (Buyer ID: 101, Seller ID: 201)

>>> MATCH EXECUTED <<<
Traded Quantity: 30 @ price: $148 (Buyer ID: 103, Seller ID: 201)