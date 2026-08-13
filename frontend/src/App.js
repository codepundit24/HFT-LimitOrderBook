import React, { useState, useEffect } from 'react';
import axios from 'axios';
import './App.css';

const API_BASE_URL = "http://127.0.0.1:8000/api/v1";

function App() {
  const [formData, setFormData] = useState({
    symbol: 'AAPL',
    order_type: 'LIMIT',      // LIMIT ya STOP_LOSS
    sl_type: 'STOP_LIMIT',     // STOP_LIMIT ya STOP_MARKET
    side: 'BUY',
    price: 150.0,
    trigger_price: 145.0,
    quantity: 50
  });

  const [orders, setOrders] = useState([]);
  const [loading, setLoading] = useState(false);
  const [statusMsg, setStatusMsg] = useState('');

  const fetchOrders = async () => {
    try {
      const res = await axios.get(`${API_BASE_URL}/orders/history`);
      const historyList = res.data?.order || res.data?.orders || [];
      setOrders(historyList);
    } catch (err) {
      console.error("Failed to fetch order history:", err);
      setOrders([]);
    }
  };

  useEffect(() => {
    fetchOrders();
  }, []);

  const handleChange = (e) => {
    setFormData({ ...formData, [e.target.name]: e.target.value });
  };

  const handleSubmit = async (e) => {
    e.preventDefault();
    setLoading(true);
    setStatusMsg('');

    try {
      // Agar STOP_MARKET hai toh Price ko Trigger Price ke barabar ya 0 set kar sakte hain
      const finalPrice = (formData.order_type === 'STOP_LOSS' && formData.sl_type === 'STOP_MARKET') 
        ? parseFloat(formData.trigger_price) 
        : parseFloat(formData.price);

      const payload = {
        symbol: formData.symbol,
        order_type: formData.order_type === 'STOP_LOSS' ? `SL_${formData.sl_type}` : 'LIMIT',
        side: formData.side,
        price: finalPrice,
        trigger_price: formData.order_type === 'STOP_LOSS' ? parseFloat(formData.trigger_price) : 0.0,
        quantity: parseInt(formData.quantity)
      };

      const res = await axios.post(`${API_BASE_URL}/order`, payload);
      setStatusMsg(`Success: ${res.data.engine_response}`);
      await fetchOrders();
    } catch (err) {
      setStatusMsg(`Error: ${err.response?.data?.detail || "Order Placement Failed"}`);
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="dashboard">
      <header className="header">
        <h2>⚡ HFT Trading Terminal</h2>
        <span className="badge">C++ Matching Engine Active</span>
      </header>

      <div className="main-content">
        {/* Order Form */}
        <div className="card form-card">
          <h3>Place Order</h3>
          <form onSubmit={handleSubmit}>
            <div className="form-group">
              <label>Symbol</label>
              <input type="text" name="symbol" value={formData.symbol} onChange={handleChange} required />
            </div>

            <div className="form-row">
              <div className="form-group">
                <label>Type</label>
                <select name="order_type" value={formData.order_type} onChange={handleChange}>
                  <option value="LIMIT">LIMIT ORDER</option>
                  <option value="STOP_LOSS">STOP LOSS</option>
                </select>
              </div>

              <div className="form-group">
                <label>Side</label>
                <select name="side" value={formData.side} onChange={handleChange}>
                  <option value="BUY">BUY</option>
                  <option value="SELL">SELL</option>
                </select>
              </div>
            </div>

            {/* Dynamic Stop-Loss Options */}
            {formData.order_type === 'STOP_LOSS' && (
              <div className="form-group">
                <label>Stop Loss Mode</label>
                <select name="sl_type" value={formData.sl_type} onChange={handleChange}>
                  <option value="STOP_LIMIT">SL-Limit (Price + Trigger)</option>
                  <option value="STOP_MARKET">SL-Market (Only Trigger)</option>
                </select>
              </div>
            )}

            <div className="form-row">
              {/* Limit Price Input: Visible for LIMIT orders or SL-Limit orders */}
              {(formData.order_type === 'LIMIT' || (formData.order_type === 'STOP_LOSS' && formData.sl_type === 'STOP_LIMIT')) && (
                <div className="form-group">
                  <label>Limit Price ($)</label>
                  <input type="number" step="0.1" name="price" value={formData.price} onChange={handleChange} required />
                </div>
              )}

              {/* Trigger Price Input: Visible ONLY for STOP_LOSS orders */}
              {formData.order_type === 'STOP_LOSS' && (
                <div className="form-group">
                  <label>Trigger Price ($)</label>
                  <input type="number" step="0.1" name="trigger_price" value={formData.trigger_price} onChange={handleChange} required />
                </div>
              )}

              <div className="form-group">
                <label>Quantity</label>
                <input type="number" name="quantity" value={formData.quantity} onChange={handleChange} required />
              </div>
            </div>

            <button type="submit" className={`submit-btn ${formData.side.toLowerCase()}`} disabled={loading}>
              {loading ? "Processing..." : `EXECUTE ${formData.side}`}
            </button>
          </form>

          {statusMsg && <div className="status-msg">{statusMsg}</div>}
        </div>

        {/* Audit Logs Table */}
        <div className="card table-card">
          <div className="table-header">
            <h3>Audit Logs & Order History</h3>
            <button onClick={fetchOrders} className="refresh-btn">🔄 Refresh</button>
          </div>

          <div className="table-wrapper">
            <table>
              <thead>
                <tr>
                  <th>ID</th>
                  <th>Symbol</th>
                  <th>Type</th>
                  <th>Side</th>
                  <th>Price</th>
                  <th>Trigger</th>
                  <th>Qty</th>
                  <th>Status</th>
                </tr>
              </thead>
              <tbody>
                {orders && orders.length > 0 ? (
                  orders.map((o) => (
                    <tr key={o.id}>
                      <td>#{o.id}</td>
                      <td><strong>{o.symbol}</strong></td>
                      <td>{o.order_type}</td>
                      <td className={o.side === 'BUY' ? 'buy-text' : 'sell-text'}>{o.side}</td>
                      <td>${o.price}</td>
                      <td>{o.trigger_price ? `$${o.trigger_price}` : '-'}</td>
                      <td>{o.quantity}</td>
                      <td><span className="status-tag">{o.status}</span></td>
                    </tr>
                  ))
                ) : (
                  <tr><td colSpan="8" style={{ textAlign: 'center', padding: '20px' }}>No orders placed yet</td></tr>
                )}
              </tbody>
            </table>
          </div>
        </div>
      </div>
    </div>
  );
}

export default App;