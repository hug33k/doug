from typing import List, Union
import aiofiles
from fastapi import APIRouter, status, HTTPException, UploadFile, Depends
from database import get_session, select, Session
from database.models import IconCreate, IconRead, IconUpdate, Icons


router = APIRouter(
	prefix="/icons",
	tags=["icons"],
)


@router.get("", response_model=List[IconRead])
async def get_icons(session: Session = Depends(get_session)):
	icons = session.exec(select(Icons)).all()
	return icons


@router.get("/{icon_id}", response_model=Union[IconRead, None])
async def get_icon(icon_id: int, session: Session = Depends(get_session)):
	icon = session.get(Icons, icon_id)
	if not icon:
		raise HTTPException(status_code=404, detail="Icon not found")
	return icon


@router.post("", response_model=IconRead, status_code=status.HTTP_201_CREATED)
async def add_icon(input_icon: IconCreate, session: Session = Depends(get_session)):
	icon = Icons.from_orm(input_icon)
	session.add(icon)
	session.commit()
	session.refresh(icon)
	return icon


@router.post("/{icon_id}", response_model=IconRead)
async def update_icon(input_icon: IconUpdate,
						icon_id: int,
						session: Session = Depends(get_session)):
	icon = session.get(Icons, icon_id)
	if not icon:
		raise HTTPException(status_code=404, detail="Icon not found")
	input_icon_dict = input_icon.dict(exclude_unset=True)
	for key, value in input_icon_dict.items():
		if not value:
			continue
		setattr(icon, key, value)
	session.add(icon)
	session.commit()
	session.refresh(icon)
	return icon


@router.delete("/{icon_id}", status_code=status.HTTP_204_NO_CONTENT)
async def remove_icon(icon_id: int, session: Session = Depends(get_session)):
	icon = session.get(Icons, icon_id)
	if not icon:
		raise HTTPException(status_code=404, detail="Icon not found")
	session.delete(icon)
	session.commit()
	return


@router.post("/{icon_id}/upload", response_model=IconRead)
async def upload_icon(file: UploadFile, icon_id: int, session: Session = Depends(get_session)):
	filetype = file.filename.split(".")[-1]
	icon = session.get(Icons, icon_id)
	if not icon:
		raise HTTPException(status_code=404, detail="Icon not found")
	# Changer le nom du fichier par un UUID pour éviter les conflits
	# Et check que l'UUID ne soit pas déjà utilisé
	async with aiofiles.open(
		f"/home/pi/Projects/Doug/back/public/icons/{icon_id}.{filetype}",
		"wb") as file_disk:
		content = await file.read()
		await file_disk.write(content)
	setattr(icon, "path", f"/public/icons/{icon_id}.{filetype}")
	session.add(icon)
	session.commit()
	session.refresh(icon)
	return icon
