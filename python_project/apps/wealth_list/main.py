# /usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import traceback
import logging
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from routers import search
from log import Log
from database import Base, engine, get_db
from contextlib import asynccontextmanager
from dependencies import get_current_active_user
from pathlib import Path

origins = [
    "*"
]
Log("api") # 记录日志

@asynccontextmanager
async def lifespan(app: FastAPI):
    # 初始化数据库
    Base.metadata.create_all(bind=engine)
    # 获取当前目录
    current_dir = str(Path(__file__).resolve().parent)
    sys.path.append(current_dir)
    yield


try:
    app = FastAPI(lifespan=lifespan)
    # 设置全局依赖项
    app.dependency_overrides[get_db] = get_db
    app.dependency_overrides[get_current_active_user] = get_current_active_user
    app.add_middleware(
        CORSMiddleware,
        allow_origins=origins,
        allow_credentials=True,
        allow_methods=["*"],
        allow_headers=["*"],
    )
    app.include_router(search.router)
except Exception as error:
    logging.error(traceback.format_exc())
    logging.error("error: %s", error)
