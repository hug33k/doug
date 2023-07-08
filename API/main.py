from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from fastapi.staticfiles import StaticFiles
from routers import buttons, sounds, icons, misc, websocket


app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=['*'],
    allow_credentials=False,
    allow_methods=["*"],
    allow_headers=["*"],
)
app.include_router(buttons.router)
app.include_router(sounds.router)
app.include_router(icons.router)
app.include_router(misc.router)
app.include_router(websocket.router)
app.mount("/public", StaticFiles(directory="public"), name="public")
