from contextlib import asynccontextmanager
from fastapi import FastAPI, HTTPException,Depends
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
from sqlalchemy.orm import Session
import socket

from database import SessionLocal, init_db,OrderLog

@asynccontextmanager
async def lifespan(app: FastAPI):
    init_db()
    yield


app = FastAPI(title= "HFT Engine API Gateway", lifespan= lifespan)


#CORS Setup
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"]
)


#Dependency : Databse session
def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()

#Pydantic Schema
class OrderRequest(BaseModel):
    symbol:str = "AAPL"
    order_type :str
    side:str
    price:float
    trigger_price:float = 0
    quantity:int

def send_to_cpp_engine(message:str) -> str:
    CPP_SERVER_HOST = '127.0.0.1'
    CPP_SERVER_PORT = 8080

    try:
        client_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        client_socket.connect((CPP_SERVER_HOST,CPP_SERVER_PORT))

        client_socket.sendall(message.encode('utf-8'))

        response = client_socket.recv(1024).decode('utf-8')
        client_socket.close()

        return response
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"C++ Engine Connection Failed: {str(e)}")


@app.post("/api/v1/order")
def place_order(order: OrderRequest, db:Session = Depends(get_db)):

    if order.order_type.upper() == "STOP_LOSS":
        formatted_msg = (
            f"STOP_LOSS {order.side.upper()} ORDER: {order.symbol}"
            f"@{order.price} Trigger: {order.trigger_price} Qty: {order.quantity}"
        )
    else:
        formatted_msg = (
            f"LIMIT {order.side.upper()} ORDER: {order.symbol} "
            f"@{order.price} Qty: {order.quantity}"
        )

    cpp_response = send_to_cpp_engine(formatted_msg)

    db_order= OrderLog(
        symbol = order.symbol.upper(),
        order_type = order.order_type.upper(),
        side = order.side.upper(),
        price = order.price,
        trigger_price = order.trigger_price,
        quantity = order.quantity,
        status = "PROCESSED",
        engine_response = cpp_response.strip()
    )

    db.add(db_order)
    db.commit()
    db.refresh(db_order)

    return {
        "status": "Success",
        "order_id":db_order.id,
        "sent_message": formatted_msg,
        "engine_response": cpp_response.strip()
    }

#API for get order History
@app.get("/api/v1/orders/history")
def get_order_history(db:Session = Depends(get_db)):
    orders = db.query(OrderLog).order_by(OrderLog.id.desc()).all()
    return {"order":orders}

@app.get("/")
def read_root():
    return {"message": "HFT Trading Engine API Gateway is Online"}