#include <pybind11/pybind11.h>
#include "OrderBook.h"
#include "Order.h"
#include "OrderPool.h"

namespace py = pybind11;

PYBIND11_MODULE(trading_engine, m) {
    m.doc() = "High-Frequency Trading Matching Engine Python Bindings";

    // Expose OrderType Enum
    py::enum_<OrderType>(m, "OrderType")
        .value("BUY", OrderType::BUY)
        .value("SELL", OrderType::SELL)
        .export_values();

    // Expose Order class/struct
    py::class_<Order>(m, "Order")
        .def_readwrite("orderId", &Order::orderId)
        .def_readwrite("side", &Order::side)
        .def_readwrite("price", &Order::price)
        .def_readwrite("quantity", &Order::quantity);

    // Expose OrderPool class
    py::class_<OrderPool>(m, "OrderPool")
        .def(py::init<size_t>())
        .def("allocate", &OrderPool::allocate, py::return_value_policy::reference);

    // Expose OrderBook class
    py::class_<OrderBook>(m, "OrderBook")
        .def(py::init<>())
        .def("add_order", &OrderBook::addOrder)
        .def("cancel_order", &OrderBook::cancelOrder)
        .def("print_book", &OrderBook::printBook);
}