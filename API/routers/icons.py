import aiofiles
from typing import List, Union
from fastapi import APIRouter, Query, status, HTTPException, UploadFile
from database import get_session, select
from database.models import IconCreate, IconRead, IconUpdate, Icons

router = APIRouter(
    prefix="/icons",
    tags=["icons"],
)


@router.get("", response_model=List[IconRead])
async def get_icons():
    with get_session() as session:
        icons = session.exec(select(Icons)).all()
        return icons


@router.get("/{icon_id}", response_model=Union[IconRead, None])
async def get_icon(icon_id: int = Query()):
    with get_session() as session:
        icon = session.get(Icons, icon_id)
        if (not icon):
            raise HTTPException(status_code=404, detail="Icon not found")
        return icon


@router.post("", response_model=IconRead, status_code=status.HTTP_201_CREATED)
async def add_icon(input_icon: IconCreate):
    with get_session() as session:
        icon = Icons.from_orm(input_icon)
        session.add(icon)
        session.commit()
        session.refresh(icon)
        return icon


@router.post("/{icon_id}", response_model=IconRead)
async def update_icon(input_icon: IconUpdate, icon_id: int = Query()):
    with get_session() as session:
        icon = session.get(Icons, icon_id)
        if (not icon):
            raise HTTPException(status_code=404, detail="Icon not found")
        input_icon_dict = input_icon.dict(exclude_unset=True)
        for key, value in input_icon_dict.items():
            setattr(icon, key, value)
        session.add(icon)
        session.commit()
        session.refresh(icon)
        return icon


@router.delete("/{icon_id}", status_code=status.HTTP_204_NO_CONTENT)
async def remove_icon(icon_id: int = Query()):
    with get_session() as session:
        icon = session.get(Icons, icon_id)
        if (not icon):
            raise HTTPException(status_code=404, detail="Icon not found")
        session.delete(icon)
        session.commit()
        return


@router.post("/{icon_id}/upload", response_model=IconRead)
async def upload_icon(file: UploadFile, icon_id: int = Query()):
    with get_session() as session:
        filetype = file.filename.split(".")[-1]
        icon = session.get(Icons, icon_id)
        if (not icon):
            raise HTTPException(status_code=404, detail="Icon not found")
        # Changer le nom du fichier par un UUID pour éviter les conflits
        # Et check que l'UUID ne soit pas déjà utilisé
        async with aiofiles.open("/home/pi/Projects/Doug/API/public/icons/{}.{}".format(icon_id, filetype), "wb") as file_disk:
            content = await file.read()
            await file_disk.write(content)
        setattr(icon, "path", "/public/icons/{}.{}".format(icon_id, filetype))
        session.add(icon)
        session.commit()
        session.refresh(icon)
        return icon
