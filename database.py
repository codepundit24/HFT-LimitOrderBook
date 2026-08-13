from sqlalchemy import create_engine,Column, Integer, String ,Float, DateTime
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker
from sqlalchemy.sql import func
import urllib.parse

raw_password = "Seekhna@24"
encoded_password = urllib.parse.quote_plus(raw_password)

DATABASE_URL = f"mysql+pymysql://root:{encoded_password}@127.0.0.1:3306/hft_trading_db"

engine = create_engine(DATABASE_URL)
SessionLocal = sessionmaker(autocommit = False,autoflush=False,bind=engine)
Base = declarative_base()

class OrderLog(Base):
    __tablename__ = "order_logs"

    id = Column(Integer,primary_key=True,index=True)
    symbol = Column(String(20),nullable=False)
    order_type = Column(String(20),nullable=False)
    side = Column(String(10), nullable=False)
    price = Column(Float, nullable=False)
    trigger_price = Column(Float, default=0)
    quantity = Column(Integer,nullable=False)
    status = Column(String(50), default="SENT_TO_ENGINE")
    engine_response = Column(String(255))
    timestamp = Column(DateTime, server_default= func.now())


def init_db():
    Base.metadata.create_all(bind = engine)
    