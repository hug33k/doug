from typing import List, Union
import aiofiles
from fastapi import APIRouter, status, HTTPException, UploadFile, Depends
from database import get_session, select, Session
from database.models import SoundCreate, SoundRead, SoundUpdate, Sounds


router = APIRouter(
	prefix="/sounds",
	tags=["sounds"],
)


@router.get("", response_model=List[SoundRead])
async def get_sounds(session: Session = Depends(get_session)):
	sounds = session.exec(select(Sounds)).all()
	return sounds


@router.get("/{sound_id}", response_model=Union[SoundRead, None])
async def get_sound(sound_id: int, session: Session = Depends(get_session)):
	sound = session.get(Sounds, sound_id)
	if not sound:
		raise HTTPException(status_code=404, detail="Sound not found")
	return sound


@router.post("", response_model=SoundRead, status_code=status.HTTP_201_CREATED)
async def add_sound(input_sound: SoundCreate, session: Session = Depends(get_session)):
	sound = Sounds.from_orm(input_sound)
	session.add(sound)
	session.commit()
	session.refresh(sound)
	return sound


@router.post("/{sound_id}", response_model=SoundRead)
async def update_sound(input_sound: SoundUpdate,
						sound_id: int,
						session: Session = Depends(get_session)):
	sound = session.get(Sounds, sound_id)
	if not sound:
		raise HTTPException(status_code=404, detail="Sound not found")
	input_sound_dict = input_sound.dict(exclude_unset=True)
	for key, value in input_sound_dict.items():
		setattr(sound, key, value)
	session.add(sound)
	session.commit()
	session.refresh(sound)
	return sound


@router.delete("/{sound_id}", status_code=status.HTTP_204_NO_CONTENT)
async def remove_sound(sound_id: int, session: Session = Depends(get_session)):
	sound = session.get(Sounds, sound_id)
	if not sound:
		raise HTTPException(status_code=404, detail="Sound not found")
	session.delete(sound)
	session.commit()
	return


@router.post("/{sound_id}/upload", response_model=SoundRead)
async def upload_sound(file: UploadFile, sound_id: int, session: Session = Depends(get_session)):
	filetype = file.filename.split(".")[-1]
	sound = session.get(Sounds, sound_id)
	if not sound:
		raise HTTPException(status_code=404, detail="Sound not found")
	# Changer le nom du fichier par un UUID pour éviter les conflits
	# Et check que l'UUID ne soit pas déjà utilisé
	async with aiofiles.open(
		f"/home/pi/Projects/Doug/back/public/sounds/{sound_id}.{filetype}",
		"wb") as file_disk:
		content = await file.read()
		await file_disk.write(content)
	setattr(sound, "path", f"/public/sounds/{sound_id}.{filetype}")
	session.add(sound)
	session.commit()
	session.refresh(sound)
	return sound
