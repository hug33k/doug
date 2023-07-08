import json
from fastapi import APIRouter, WebSocket
#from pydantic import BaseModel, Field


router = APIRouter()


@router.websocket("/ws")
async def websocket(websocket: WebSocket):
    await websocket.accept()
    while True:
        data = await websocket.receive_text()
        try:
            data_json = json.loads(data)
            response = {
                "type": "?",
                "data": {
                    "originalData": data_json
                }
            }
        except Exception as e:
            response = {
                "type": "error",
                "data": {
                    "errorMessage": str(e)
                }
            }
        await websocket.send_text(json.dumps(response))
