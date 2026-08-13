# ⚡ Low-Latency HFT Limit Order Book & Trading Terminal

A end-to-end, high-performance **High-Frequency Trading (HFT) Execution & Audit System**. Built with an ultra-fast **C++17 Matching Engine** core, an asynchronous **FastAPI Gateway**, persistent **MySQL Audit Logs**, and a modern **React Dark-Theme Trading Terminal**.

Designed around zero-allocation memory principles and deterministic execution pipelines.

---

## 🏛️ System Architecture

┌─────────────────────────┐          HTTP / REST          ┌─────────────────────────┐
│                         │ ────────────────────────────> │                         │
│  React Trading Terminal │                               │   FastAPI Gateway       │
│  (Port 3000)            │ <──────────────────────────── │   (Port 8000)           │
└─────────────────────────┘          JSON Response        └────────────┬────────────┘
│
┌───────────────┴───────────────┐
TCP Sockets                      SQLAlchemy
│                               │
▼                               ▼
┌────────────────────────┐       ┌──────────────────────┐
│  C++ Matching Engine   │       │  MySQL Audit Logs    │
│  (Port 8080)           │       │  (Database)          │
└────────────────────────┘       └──────────────────────┘

---

## 🚀 Key Features

* **C++ Core Engine (Microsecond Latency):**
  * **Price-Time Priority (FIFO):** Execution logic for LIMIT and Stop-Loss orders (`SL_STOP_LIMIT`, `SL_STOP_MARKET`).
  * **$O(1)$ Slab Allocator:** Memory Pool pre-allocates memory for order structures at startup, eliminating runtime OS heap allocations (`new`/`delete`) and preventing latency spikes.
  * **$O(1)$ Order Cancellation:** Intrusive Doubly Linked Lists allow instant node removals from queues without $O(N)$ traversals.
  * **TCP Socket IPC:** Custom binary/TCP socket protocol for instant inter-process communication with the API Gateway.

* **FastAPI Gateway & Persistence:**
  * Asynchronous REST API routing.
  * Auto audit logging to **MySQL** via SQLAlchemy for regulatory-style tracking.
  * Cross-Origin Resource Sharing (CORS) enabled for seamless UI integration.

* **React Dashboard UI:**
  * Sleek, high-contrast Dark Mode Trading Terminal.
  * Real-time order placement form with dynamic field switching (LIMIT, SL-Limit, SL-Market).
  * Scrollable Audit Log table with sticky headers for real-time order history tracking.

---

## 🛠️ Tech Stack

| Layer | Technology |
| :--- | :--- |
| **Engine Core** | C++17, TCP Sockets, Custom Memory Pools |
| **Build System** | CMake, GCC / MinGW |
| **Backend API** | Python 3.10+, FastAPI, Uvicorn, SQLAlchemy |
| **Database** | MySQL Server |
| **Frontend UI** | React.js, Axios, Modern CSS3 |

---

## ⚙️ Project Setup & Installation

### 1. Prerequisites
Ensure you have installed:
* **GCC/MinGW** (C++17 support)
* **CMake** (v3.10+)
* **Python 3.10+** & **MySQL Database**
* **Node.js** (v16+) & **npm**

---

### 2. Build & Run C++ Engine

```bash
# Clone the repository
git clone [https://github.com/codepundit24/HFT-LimitOrderBook.git](https://github.com/codepundit24/HFT-LimitOrderBook.git)
cd HFT-LimitOrderBook

# Create build directory and compile
mkdir build && cd build
cmake ..
cmake --build .

# Launch Engine Socket Server
./LimitOrderBook.exe      # Windows
./LimitOrderBook          # Linux/macOS
```


## 3. Setup FastAPI Backend
In a new terminal (Root directory):

```bash
# Install Python dependencies
pip install fastapi uvicorn sqlalchemy pymysql

# Start FastAPI Server
uvicorn main:app --reload
```

## 4. Setup React Dashboard UI
In a new terminal (Root directory):

```bash
# Navigate to frontend folder
cd frontend

# Install Node modules
npm install

# Start React Terminal
npm start
```
Access the UI Dashboard at http://localhost:3000 

## 📊 Sample Execution Logs

--- CURRENT ORDER BOOK ---
ASKS (Sell Side):
BIDS (Buy Side):
  $148 => [ (ID:101 Qty:50) (ID:102 Qty:100) (ID:103 Qty:75) ]
--------------------------

>>> ORDER PROCESSED BY C++ ENGINE <<<
Order Type: LIMIT | Side: BUY | Price: $148.0 | Qty: 50 | Status: PROCESSED

>>> MATCH EXECUTED <<<
Traded Quantity: 50 @ price: $148 (Buyer ID: 101, Seller ID: 201)
